/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:22:46 by loruzqui          #+#    #+#             */
/*   Updated: 2025/11/23 13:26:32 by loruzqui         ###   ########.fr       */
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
		static const int				_commandListSize = 16;
		static const command_handler	_commandList[_commandListSize];

		int							_port;
		std::string					_passwd;
		int							_fdPpalSocket;
		std::string					_hostname;
		std::vector<Client *>		_clients;
		std::vector<Channel *>		_channels;
		std::vector<struct pollfd>	_fdsPoll;
		struct sockaddr_in			_confSocket;
		int							_replyCode;
		static bool					_signal;

		std::string	_getHostname();

		void	_isValidPort(const std::string &port); //Between 1 and 65535
		bool	_isClientInAnyChannel(const int fd);
		bool	_clientIsReadyToLogin(const int fd); //If PASS + NICK + USER was send
		bool	_isValidNickname(const std::string &nickname); //IRC rules
		bool	_isNicknameInUse(const int fd, const std::string &username);

		//Command handler
		void	_handlerClientJoin(const std::string &buffer, const int fd);
		void	_handlerClientQuit(const std::string &buffer, const int fd);
		void	_handlerClientPart(const std::string &buffer, const int fd);
		void	_handlerClientMode(const std::string &buffer, const int fd);
		void	_handlerClientKick(const std::string &buffer, const int fd);
		void	_handlerClientTopic(const std::string &buffer, const int fd);
		void	_handlerClientInvite(const std::string &buffer, const int fd);
		void	_handlerClientPrivmsg(const std::string &buffer, const int fd);
		void	_handlerClientNickname(const std::string &nickname, const int fd);
		void	_handlerClientUsername(const std::string &username, const int fd);
		void	_handlerClientPassword(const std::string &password, const int fd);

		//Signal handler
		static void	_signalHandler(const int signum);

		void	_closeFds();
		void	_serverLoop();
		void	_setServerSocket();
		void	_addServerSignal();
		void	_acceptNewClient();
		void	_clearClient(const int fd);
		void	_receiveNewData(const int fd);
		void	_sendResponse(const int fd, const std::string &response);

		void						_executeCommand(const std::string buffer, const int fd);
		std::string					_cleanseBuffer(const std::string &buffer, const std::string &chars_to_remove);
		std::vector<std::string>	_splitBuffer(const std::string &buffer, const std::string &delimiter);

		Client*		_getClient(const int fd);
		Client*		_getClient(const std::string nickname);

		void		_addChannel(Channel *channel);
		Channel*	_getChannel(const std::string &channel_name);

		void		_removeClientFd(const int fd);
		void		_removeClientFromChannels(const int fd);
		void		_removeClientFromServer(const int fd);

	public:
		Server(void);
		~Server();

		void init(const std::string &port, const std::string &passwd);
};

std::string	toupper(const std::string &str);

#endif
