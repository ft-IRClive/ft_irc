/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:12:39 by loruzqui          #+#    #+#             */
/*   Updated: 2025/11/20 17:10:07 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

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
	this->_signal = false;
}

Server::~Server()
{
	for (std::vector<Client *>::iterator i = this->_clients.begin(); i != this->_clients.end(); i++)
	{
		delete *i;
	}
	this->_clients.clear();
	for (std::vector<Channel *>::iterator i = this->_channels.begin(); i != this->_channels.end(); i++)
	{
		delete *i;
	}
	this->_channels.clear();
}

void Server::init(const std::string &port, const std::string &passwd)
{
	_isValidPort(port);

	this->_port = std::atoi(port.c_str());
	this->_passwd = passwd;

	try
	{
		_addServerSignal();
		_setServerSocket();

		std::cout << "Waiting to accept a connection...\n";

		_serverLoop();
		_closeFds();
	}
	catch (const std::exception& e)
	{
		_closeFds();
		std::cerr << e.what() << '\n';
	}
}

//-----------

std::string	toupper(const std::string &str)
{
	std::string	s;

	for (int i = 0; i < str.size(); i++)
	{
		s[i] = std::toupper(str[i]);
	}
	return (s);
}

std::string Server::_getHostname()
{
	return (this->_hostname);
}

//-----------

void Server::_isValidPort(const std::string& port)
{
	if (!(port.find_first_not_of("0123456789") == std::string::npos && std::atoi(port.c_str()) >= 1024 && std::atoi(port.c_str()) <= 65535))
		throw std::invalid_argument("The port is invalid because have non-numeric characters or because is out of range");
}

bool Server::_isClientInAnyChannel(const int fd)
{

}

bool Server::_clientIsReadyToLogin(const int fd)
{
	Client	*client = this->_getClient(fd);

	if (!client->getUname().empty() && !client->getNname().empty() && !client->getIsLogged())
		return (true);
	return (false);
}

bool Server::_isValidNickname(const std::string& nickname)
{

}

bool Server::_isNicknameInUse(const int fd, const std::string& nickname)
{

}

//-----------

void Server::_signalHandler(const int signum)
{

}

//-----------

void Server::_closeFds()
{

}

void Server::_serverLoop()
{
	while (Server::_signal == false)
	{
		if ((poll(&this->_fdsPoll[0], this->_fdsPoll.size(), -1) == -1) && Server::_signal == false)
			throw(std::runtime_error("Poll() failed"));
		for (size_t i = 0; i < this->_fdsPoll.size(); i++)
		{
			if (this->_fdsPoll[i].revents & POLLIN) //POLLIN -> Data is available to read without blocking
			{
				if (this->_fdsPoll[i].fd == this->_fdPpalSocket)
					_acceptNewClient();
				else
					_receiveNewData(this->_fdsPoll[i].fd);
			}
		}
	}
}

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
}

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
	client->setIpAddr(inet_ntoa((cliAddr.sin_addr)));
	this->_clients.push_back(client);
	this->_fdsPoll.push_back(newPoll);
	std::cout << "Client <" << fd << "> Connected" << std::endl;
}

void Server::_clearClient(const int fd)
{

}

void Server::_receiveNewData(const int fd)
{

}

void Server::_sendResponse(const int fd, const std::string &response)
{

}

//-----------

void Server::_executeCommand(const std::string buffer, const int fd)
{

}

std::string Server::_cleanseBuffer(const std::string &buffer, const std::string &chars_to_remove)
{

}

//-----------

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
		//Esta función debe estar en el canal
		/*if (channel_name == this->_clients[i]->get_channel_name())
		{
			return (this->_channels[i]);
		}*/
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
	Client	*client = _getClient(fd);

	//Esta función debe estar en el canal
	/*for(std::vector<Channel *>::iterator i = this->_channels.begin(); i != this->_channels.end(); i++)
	{
		(*i)->removeChannelClient(client);
	}*/
}

void Server::_removeClientFromServer(const int fd)
{
	for (std::vector<Client *>::iterator i = this->_clients.begin(); i != this->_clients.end(); ++i)
	{
		if ((*i)->getFd() == fd)
		{
			delete *i;
			this->_clients.erase(i);
			break;
		}
	}
}
