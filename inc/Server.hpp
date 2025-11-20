/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:22:46 by loruzqui          #+#    #+#             */
/*   Updated: 2025/11/18 18:33:38 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sstream>

//For sockets
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

//To non-blocking sockets
#include <fcntl.h>

//To use poll()
#include <poll.h>

//For signals
#include <csignal>

//For some C functions
#include <cstdlib>

//For strings
#include <cstring>
#include <string>

//Extra
#include <iomanip>
#include <map>
#include <utility>
#include <vector>

//Our libraries
#include "../inc/Channel.hpp"
#include "../inc/Client.hpp"
#include "../inc/Replies.hpp"

class Server
{
	private:
		//To join the command and the function than handler this command
		struct command_handler
		{
			std::string	command;
			void (Server::*handler)(const std::string&, const int);
		};
		static const int				_command_list_size = 16;
		static const command_handler	_command_list[_command_list_size];

		int							_port;
		std::string					_passwd;
		int							_fd_ppal_socket;
		std::string					_hostname;
		std::vector<Client *>		_clients;
		std::vector<Channel *>		_channels;
		std::vector<struct pollfd>	_fds_poll;
		struct sockaddr_in			_conf_socket;
		int							_reply_code;
		static bool					_signal;

		std::string	_get_hostname();

		void	_is_valid_port(const std::string& port); //Between 1 and 65535
		bool	_is_client_in_any_channel(const int fd);
		bool	_client_is_ready_to_login(const int fd); //If PASS + NICK + USER was send
		bool	_is_valid_nickname(const std::string& nickname); //IRC rules
		bool	_is_nickname_in_use(const int fd, const std::string& nickname);

		//Command handler
		void	_handler_client_join(const std::string &buffer, const int fd);
		void	_handler_client_quit(const std::string &buffer, const int fd);
		void	_handler_client_part(const std::string &buffer, const int fd);
		void	_handler_client_mode(const std::string &buffer, const int fd);
		void	_handler_client_kick(const std::string &buffer, const int fd);
		void	_handler_client_topic(const std::string &buffer, const int fd);
		void	_handler_client_invite(const std::string &buffer, const int fd);
		void	_handler_client_privmsg(const std::string &buffer, const int fd);
		void	_handler_client_nickname(const std::string &nickname, const int fd);
		void	_handler_client_username(const std::string &username, const int fd);
		void	_handler_client_password(const std::string &password, const int fd);

		//Signal handler
		static void	_signal_handler(const int signum);

		void	_close_fds();
		void	_server_loop();
		void	_set_server_socket();
		void	_add_server_signal();
		void	_accept_new_client();
		void	_clear_client(const int fd);
		void	_receive_new_data(const int fd);
		void	_send_response(const int fd, const std::string &response);

		void						_execute_command(const std::string buffer, const int fd);
		std::string					_cleanse_buffer(const std::string &buffer, const std::string &chars_to_remove);

		Client*		_get_client(const int fd);
		Client*		_get_client(const std::string nickname);

		void		_add_channel(Channel *channel);
		Channel*	_get_channel(const std::string &channel_name);

		void		_remove_client_fd(const int fd);
		void		_remove_client_from_channels(const int fd);
		void		_remove_client_from_server(const int fd);

	public:
		Server(void);
		~Server();

		void init(const std::string &port, const std::string &passwd);
};

std::string	toupper(const std::string &str);

#endif
