/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:12:39 by loruzqui          #+#    #+#             */
/*   Updated: 2025/11/18 18:38:12 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

Server::Server(void)
{

}

Server::~Server()
{

}

void Server::init(const std::string &port, const std::string &passwd)
{

}

//-----------

std::string	toupper(const std::string &str)
{

}

std::string Server::_get_hostname()
{

}

//-----------

void Server::_is_valid_port(const std::string& port)
{

}

bool Server::_is_client_in_any_channel(const int fd)
{

}

bool Server::_client_is_ready_to_login(const int fd)
{

}

bool Server::_is_valid_nickname(const std::string& nickname)
{

}

bool Server::_is_nickname_in_use(const int fd, const std::string& nickname)
{

}

//-----------

void Server::_handler_client_join(const std::string &buffer, const int fd)
{

}

void Server::_handler_client_quit(const std::string &buffer, const int fd)
{

}

void Server::_handler_client_part(const std::string &buffer, const int fd)
{

}

void Server::_handler_client_mode(const std::string &buffer, const int fd)
{

}

void Server::_handler_client_kick(const std::string &buffer, const int fd)
{

}

void Server::_handler_client_topic(const std::string &buffer, const int fd)
{

}

void Server::_handler_client_invite(const std::string &buffer, const int fd)
{

}

void Server::_handler_client_privmsg(const std::string &buffer, const int fd)
{

}

void Server::_handler_client_nickname(const std::string &nickname, const int fd)
{

}

void Server::_handler_client_username(const std::string &username, const int fd)
{

}

void Server::_handler_client_password(const std::string &password, const int fd)
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

}

Client* Server::_get_client(const std::string nickname)
{

}

//-----------

void Server::_add_channel(Channel *channel)
{

}

Channel _get_channel(const std::string &channel_name)
{

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
