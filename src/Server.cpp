/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:12:39 by loruzqui          #+#    #+#             */
/*   Updated: 2025/11/19 18:00:22 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

Server::Server(void)
{
	this->_port = 0;
	this->_passwd = "";
	this->_fd_ppal_socket = -1;
	this->_hostname = "ft_irc.42.fr";
	this->_clients = std::vector<Client *>();
	this->_channels = std::vector<Channel *>();
	this->_fds_poll = std::vector<struct pollfd>();
	memset(&_conf_socket, 0, sizeof(_conf_socket));
	this->_reply_code = 0;
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

std::string Server::_get_hostname()
{
	return (this->_hostname);
}

//-----------

void Server::_is_valid_port(const std::string& port)
{
	if (!(port.find_first_not_of("0123456789") == std::string::npos && std::atoi(port.c_str()) >= 1024 && std::atoi(port.c_str()) <= 65535))
		throw std::invalid_argument("The port is invalid because have non-numeric characters or because is out of range");
}

bool Server::_is_client_in_any_channel(const int fd)
{

}

bool Server::_client_is_ready_to_login(const int fd)
{
	Client	*client = this->_get_client(fd);

	//Esta función debe estar en el cliente
	/*if (!client->get_username().empty() && !client->get_nickname().empty() && !client->get_is_logged())
		return (true);*/
	return (false);
}

bool Server::_is_valid_nickname(const std::string& nickname)
{

}

bool Server::_is_nickname_in_use(const int fd, const std::string& nickname)
{

}

//-----------

void Server::_signal_handler(const int signum)
{

}

//-----------

void Server::_close_fds()
{

}

void Server::_server_loop()
{

}

void Server::_set_server_socket()
{

}

void Server::_add_server_signal()
{
	signal(SIGINT, Server::_signal_handler);
	signal(SIGQUIT, Server::_signal_handler);
}

void Server::_accept_new_client()
{

}

void Server::_clear_client(const int fd)
{

}

void Server::_receive_new_data(const int fd)
{

}

void Server::_send_response(const int fd, const std::string &response)
{

}

//-----------

void Server::_execute_command(const std::string buffer, const int fd)
{

}

std::string Server::_cleanse_buffer(const std::string &buffer, const std::string &chars_to_remove)
{

}

//-----------

Client* Server::_get_client(const int fd)
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		//Esta función debe estar en el cliente
		/*if (fd == this->_clients[i]->get_fd())
		{
			return (this->_clients[i]);
		}*/
	}
	return (NULL);
}

Client* Server::_get_client(const std::string nickname)
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		//Esta función debe estar en el cliente
		/*if (nickname == this->_clients[i]->get_nickname())
		{
			return (this->_clients[i]);
		}*/
	}
	return (NULL);
}

//-----------

void Server::_add_channel(Channel *channel)
{
	this->_channels.push_back(channel);
}

Channel *Server::_get_channel(const std::string &channel_name)
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

void Server::_remove_client_fd(const int fd)
{

}

void Server::_remove_client_from_channels(const int fd)
{

}

void Server::_remove_client_from_server(const int fd)
{

}
