/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui < >                               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:12:39 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/19 18:07:29 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

bool Server::_signal = false;

const Server::commandHandler Server::_commandList[_commandListSize] = {
	{"CAP", &Server::_handlerClientCap},
	{"INVITE", &Server::_handlerClientInvite},
	{"JOIN", &Server::_handlerClientJoin},
	{"KICK", &Server::_handlerClientKick},
	{"MODE", &Server::_handlerClientMode},
	{"NICK", &Server::_handlerClientNickname},
	{"PART", &Server::_handlerClientPart},
	{"PRIVMSG", &Server::_handlerClientPrivmsg},
	{"PASS", &Server::_handlerClientPassword},
	{"QUIT", &Server::_handlerClientQuit},
	{"TOPIC", &Server::_handlerClientTopic},
	{"USER", &Server::_handlerClientUsername},
	{"WHO", &Server::_handlerClientWho},
};

Server::Server(void)
{
	this->_port = 0;
	this->_passwd = "";
	this->_fdPpalSocket = -1;
	this->_hostname = "ft_irc.42.fr";
	this->_clients = std::vector<Client *>();
	this->_channels = std::vector<Channel *>();
	this->_fdsPoll = std::vector<struct pollfd>();
	memset(&_confSocket, 0, sizeof(_confSocket));
	this->_replyCode = 0;
}

Server::~Server()
{
	std::vector<Client *>::iterator		i;
	std::vector<Channel *>::iterator	j;

	for (i = this->_clients.begin(); i != this->_clients.end(); i++)
	{
		delete *i;
	}
	this->_clients.clear();
	for (j = this->_channels.begin(); j != this->_channels.end(); j++)
	{
		delete *j;
	}
	this->_channels.clear();
}

/**
 * @brief This function initializes the server with the given port and password,
 * sets up the socket and signal handling, and starts the main server loop.
 *
 * @param port
 * @param passwd
 */
void Server::init(const std::string &port, const std::string &passwd)
{
	_isValidPort(port);

	this->_port = std::atoi(port.c_str()); //c_str() converts a std::string to a const char * (needed for the atoi())
	this->_passwd = passwd;

	try
	{
		_addServerSignal();
		_setServerSocket();

		std::cout << GREEN << "Waiting to accept a connection...\n" << RESET;

		_serverLoop();
		_closeFds();
	}
	catch (const std::exception& e)
	{
		_closeFds();
		std::cerr << e.what() << std::endl; //e.what() show the message of the exception
	}
}

//-----------

std::string	toupper(const std::string &str)
{
	std::string	s;

	s.reserve(str.size());
	for (size_t i = 0; i < str.size(); i++)
		s.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(str[i]))));
	return (s);
}

std::string Server::_getHostname()
{
	return (this->_hostname);
}

//-----------

/**
 * @brief Checks whether the given port string contains only digits and falls within the valid range (1024–65535),
 * throwing an exception if it is invalid.
 *
 * @param port
 */
void Server::_isValidPort(const std::string &port)
{
	int	int_port = std::atoi(port.c_str());

	//std::string::npos -> nothing was found
	//find_first_not-of() -> finds the first character that does not match any of the characters passed as a parameter
	if (!(port.find_first_not_of("0123456789") == std::string::npos && int_port >= 1024 && int_port <= 65535))
		throw std::invalid_argument("The port is invalid because have non-numeric characters or because is out of range");
}

bool Server::_isClientInAnyChannel(const int fd)
{
	Client	*client = this->_getClient(fd);

	for (std::vector<Channel*>::iterator i = this->_channels.begin(); i != this->_channels.end(); i++)
	{
		if ((*i)->hasClient(client))
		{
			return (true);
		}
	}
	return (false);
}

bool Server::_clientIsReadyToLogin(const int fd)
{
	Client	*client = this->_getClient(fd);

	if (!client)
		return (false);
	if (!client->getUname().empty() && !client->getNname().empty()
			&& client->getIsAuthenticated() && !client->getIsLogged())
		return (true);
	return (false);
}

bool Server::_isValidNickname(const std::string &nickname)
{
	if (nickname.size() < 3)
		return (false);
	for (std::string::const_iterator i = nickname.begin(); i != nickname.end(); i++)
	{
		if (!std::isalnum(static_cast<unsigned char>(*i)))
			return false;
	}
	return (true);
}

bool Server::_isNicknameInUse(const int fd, const std::string &username)
{
	for (std::vector<Client *>::iterator i = this->_clients.begin(); i != this->_clients.end(); i++)
	{
		//If the fd is different, it means they are two different users but with the same name
		if ((*i)->getNname() == username && (*i)->getFd() != fd)
			return (true);
	}
	return (false);
}

//-----------

void Server::_signalHandler(const int signum)
{
	(void)signum;

	std::cout << "Signal received!" << std::endl;
	Server::_signal = true;
}

//-----------

void Server::_closeFds()
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		std::cout << "Client [" << _clients[i]->getFd() << "] Disconnected" << std::endl;
		close(_clients[i]->getFd());
	}
	if (this->_fdPpalSocket != -1)
	{
		std::cout << RED << "Server [" << this->_fdPpalSocket << "] Disconnected" << RESET << std::endl;
		close(this->_fdPpalSocket);
	}
}

/**
 * @brief Runs the main server loop using poll(), accepting new clients
 * and processing incoming data until a shutdown signal is received.
 *
 */
void Server::_serverLoop()
{
	while (Server::_signal == false) //Loop until it receives a signal
	{
		if ((poll(&this->_fdsPoll[0], this->_fdsPoll.size(), -1) == -1) && Server::_signal == false)
			throw(std::runtime_error("Poll() failed"));
		for (size_t i = 0; i < this->_fdsPoll.size(); i++)
		{
			if (this->_fdsPoll[i].revents & POLLIN) //POLLIN -> Data is available to read without blocking
			{
				//If fd is the server's primary socket → new client connecting
				if (this->_fdsPoll[i].fd == this->_fdPpalSocket)
					_acceptNewClient();
				//If the fd is from a client already connected → receive message
				else
					_receiveNewData(this->_fdsPoll[i].fd);
			}
		}
	}
}

/**
 * @brief Creates and configures the main server socket (non-blocking, reusable address),
 * binds it to the specified port, starts listening
 * for incoming connections, and registers it in the poll set.
 *
 */
void Server::_setServerSocket()
{
	struct pollfd		newPoll;
	struct sockaddr_in	serverAddr;
	int					enable = 1;

	serverAddr.sin_family = AF_INET; //Protocol Ipv4
	serverAddr.sin_addr.s_addr = INADDR_ANY; //Listen on any network interface
	serverAddr.sin_port = htons(this->_port); //Converts port number to network byte order (Big Endian - first the byte more significative)

	//Creates socket TCP
	this->_fdPpalSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_fdPpalSocket == -1)
		throw(std::runtime_error("Failed to create socket"));
	//It allows reusing the port even if it is in TIME_WAIT
	if (setsockopt(this->_fdPpalSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) == -1)
		throw(std::runtime_error("Failed to set option (SO_REUSEADDR) on socket"));
	//Non-block socket
	if (fcntl(this->_fdPpalSocket, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("Failed to set option (O_NONBLOCK) on socket"));
	//Assign socket to one direction
	if (bind(this->_fdPpalSocket,(struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
		throw(std::runtime_error("Failed to bind socket"));
	//Socket is listening
	if (listen(this->_fdPpalSocket, SOMAXCONN) == -1)
		throw(std::runtime_error("Listen() failed"));

	//Add socket to poll
	newPoll.fd = this->_fdPpalSocket;
	newPoll.events = POLLIN;
	newPoll.revents = 0;
	this->_fdsPoll.push_back(newPoll);
}

void Server::_addServerSignal()
{
	signal(SIGINT, Server::_signalHandler);
	signal(SIGQUIT, Server::_signalHandler);
	signal(SIGPIPE, SIG_IGN);
}

/**
 * @brief Accepts a new incoming client connection, sets the socket to non-blocking mode, creates a Client instance,
 * and registers the client in both the poll set and the server’s client list.
 *
 */
void Server::_acceptNewClient()
{
	struct pollfd		newPoll;
	struct sockaddr_in	cliAddr;
	int					fd;
	socklen_t			len;

	memset(&cliAddr, 0, sizeof(cliAddr));
	len = sizeof(cliAddr);

	//Accept the socket connection
	fd = accept(this->_fdPpalSocket, (sockaddr*)&(cliAddr), &len);
	if (fd == -1)
	{
		std::cout << "Accept() failed" << std::endl;
		return ;
	}
	//Put the new socket in mode non block
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cout << "Fcntl() failed" << std::endl;
		close(fd);
		return;
	}

	newPoll.fd = fd;
	newPoll.events = POLLIN;
	newPoll.revents = 0;

	Client *client = new Client(fd);
	//inet_ntoa() converts the IP address into a human-readable format
	client->setIpAddr(inet_ntoa((cliAddr.sin_addr))); //Save the client IP
	this->_clients.push_back(client);
	this->_fdsPoll.push_back(newPoll);
	std::cout << "Client <" << fd << "> Connected" << std::endl;
}

void Server::_clearClient(const int fd)
{
	_removeClientFromChannels(fd);
	_removeClientFromServer(fd);
	_removeClientFd(fd);
}

/**
 * @brief Receives incoming data from a client, handles disconnection, buffers partial messages,
 * and executes a command once a complete IRC message is detected.
 *
 * @param fd
 */
void Server::_receiveNewData(const int fd)
{
	char	buffer[1024];
	Client*	cli;
	ssize_t	bytes;

	std::memset(buffer, 0, sizeof(buffer));
	cli = _getClient(fd);
	if (!cli)
		return;

	bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
	if (bytes <= 0)
	{
		std::cout << "Client <" << fd << "> Disconnected" << std::endl;
		_clearClient(fd);
		return;
	}

	buffer[bytes] = '\0';
	cli->appendToBuffer(buffer);

	std::string	buf = cli->getBuffer();
	size_t		pos;

	while ((pos = buf.find('\n')) != std::string::npos)
	{
		std::string line = buf.substr(0, pos);
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1, 1);

		if (!line.empty())
		{
			_executeCommand(line, fd);
			cli = _getClient(fd);
			if (!cli)
				return;
		}

		buf.erase(0, pos + 1);
	}

	cli = _getClient(fd);
	if (!cli)
		return;

	cli->clearBuffer();
	if (!buf.empty())
		cli->appendToBuffer(buf);
}

void Server::_sendResponse(const int fd, const std::string &response)
{
	if (response.empty())
	{
		std::cerr << "[_sendResponse] WARNING: trying to send empty response to fd " << fd << std::endl;
		return;
	}

	std::string	out = response;
	if (out.size() < 2 || out.substr(out.size() - 2) != "\r\n")
		out += "\r\n";

	if (send(fd, out.c_str(), out.size(), 0) == -1)
		std::cerr << "Response send() failed" << std::endl;
}

//-----------

/**
 * @brief This function divide the command from the parameters.
 *
 * @param buffer
 * @param delimiter
 * @return std::vector<std::string>
 */
std::vector<std::string> Server::_splitBuffer(const std::string &buffer, const std::string &delimiter)
{
	std::string					command;
	std::string					parameters;
	std::vector<std::string>	tokens;
	std::istringstream			iss(buffer);

	iss >> command;
	tokens.push_back(command); //Save the command
	std::getline(iss >> std::ws, parameters); //std::ws delete the initial spaces
	parameters.erase(0, parameters.find_first_not_of(delimiter));
	tokens.push_back(parameters); //save the parameters
	return (tokens);
}

/**
 * @brief Parses a complete IRC message, extracts the command and parameters,
 * and invokes the corresponding command handler if it exists.
 *
 * @param buffer
 * @param fd
 */

std::string Server::_cleanseBuffer(const std::string &buffer, const std::string &chars_to_remove)
{
	size_t pos = buffer.find_first_of(chars_to_remove);
	if (pos != std::string::npos)
		return buffer.substr(0, pos);
	return buffer;
}

void Server::_executeCommand(const std::string buffer, const int fd)
{
	std::string					clean_buffer;
	std::vector<std::string>	splitted_buffer;
	std::string					command;
	std::string					parameters;

	if (buffer.empty())
		return;

	clean_buffer = _cleanseBuffer(buffer, CRLF);
	splitted_buffer = _splitBuffer(clean_buffer, SPACE);

	command = splitted_buffer.empty() ? "" : splitted_buffer[0];
	parameters = splitted_buffer.size() > 1 ? splitted_buffer[1] : "";

	if (!command.empty() && command[0] == '/')
		command.erase(0, 1);

	command = toupper(command);

	for (size_t i = 0; i < this->_commandListSize; i++)
	{
		if (command == this->_commandList[i].command)
		{
			(this->*_commandList[i].handler)(parameters, fd);
			return;
		}
	}

	_sendResponse(fd, ERR_CMDNOTFOUND(_getHostname(), command));
}

Client* Server::_getClient(const int fd)
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (fd == this->_clients[i]->getFd())
		{
			return (this->_clients[i]);
		}
	}
	return (NULL);
}

Client* Server::_getClient(const std::string nickname)
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (nickname == this->_clients[i]->getNname())
		{
			return (this->_clients[i]);
		}
	}
	return (NULL);
}

//-----------

void Server::_addChannel(Channel *channel)
{
	this->_channels.push_back(channel);
}

Channel *Server::_getChannel(const std::string &channel_name)
{
	for (size_t i = 0; i < this->_channels.size(); i++)
	{
		if (channel_name == this->_channels[i]->getChName())
		{
			return (this->_channels[i]);
		}
	}
	return (NULL);
}

//-----------

void Server::_removeClientFd(const int fd)
{
	for (size_t i = 0; i < this->_fdsPoll.size(); i++)
	{
		if (fd == this->_fdsPoll[i].fd)
		{
			this->_fdsPoll.erase(this->_fdsPoll.begin() + i);
			break;
		}
	}
	close(fd);
}

void Server::_removeClientFromChannels(const int fd)
{
	Client					*client = _getClient(fd);
	std::vector<Channel*>	channelsCopy = this->_channels;

	if (!client)
		return;
	for (size_t i = 0; i < channelsCopy.size(); i++)
	{
		if (channelsCopy[i])
			channelsCopy[i]->removeChannelClient(client);
	}
}

void Server::_removeClientFromServer(const int fd)
{
	for (std::vector<Client *>::iterator i = this->_clients.begin(); i != this->_clients.end(); i++)
	{
		if ((*i)->getFd() == fd)
		{
			delete *i;
			this->_clients.erase(i);
			break;
		}
	}
}

void Server::_sendWelcome(Client* client)
{
	if (!client)
		return;

	std::string nick     = client->getNname().empty() ? "*" : client->getNname();
	std::string user     = client->getUname().empty() ? "*" : client->getUname();
	std::string host     = client->getIpAddr();
	std::string realname = client->getRealName().empty() ? "*" : client->getRealName();

	_sendResponse(client->getFd(), RPL_WELCOME(_getHostname(), nick, user, host, realname));
}

/**
 * @brief Function used to send messages to everyone.
 *
 * @param msg
 * @param exclude_fd
 */
void Server::_broadcastToAll(const std::string &msg, int exclude_fd)
{
	Client	*client;
	int		fd_client;

	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		client = this->_clients[i];
		if (!client)
			continue;
		fd_client = client->getFd();
		if (fd_client == exclude_fd)
			continue;
		_sendResponse(fd_client, msg);
	}
}

/**
 * @brief Function used for JOIN and PRIVATEMMSG.
 *
 * @param channelName
 * @param msg
 * @param exclude_fd
 */
void Server::_broadcastToChannel(const std::string &channelName, const std::string &msg, int exclude_fd)
{
	Channel	*channel = _getChannel(channelName);
	Client	*client;
	int		fd_client;

	if (!channel)
		return;
	std::vector<Client*> clients = channel->getChClients();
	for (size_t i = 0; i < clients.size(); i++)
	{
		client = clients[i];
		if (!client)
			continue;
		fd_client = client->getFd();
		if (exclude_fd >= 0 && fd_client == exclude_fd)
			continue;
		_sendResponse(fd_client, msg);
	}
}
