/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:22:17 by loruzqui          #+#    #+#             */
/*   Updated: 2025/11/20 17:03:25 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <fcntl.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <vector>

class Client
{
	private:
		int							_fd;
		bool						_isOp;
		bool						_isLogged;
		bool						_isAuthenticated;
		std::string					_nname;
		std::string					_uname;
		std::string					_pswd;
		std::string					_ipAddr;
		std::string					_buffer;
		std::vector<std::string>	_channelsInvited;

	public:
;		Client(int fd);
		~Client();

		int							getFd() const;
		bool						getIsOp() const;
		bool						getIsLogged() const;
		bool						getIsAuthenticated() const;
		std::string					getNname() const;
		std::string					getUname() const;
		std::string					getPswd() const;
		std::string					getIpAddr() const;
		std::string					getBuffer() const;
		std::string					getHostName() const;
		std::vector<std::string>	getChannelsInvited() const;

		void	setFd(const int fd);
		void	setOp(bool isOp);
		void	setIsLogged(bool isLogged);
		void	setIsAuthenticated(bool isAuthenticated);
		void	setNname(const std::string &nname);
		void	setUname(const std::string &uname);
		void	setPswd(const std::string &pswd);
		void	setIpAddr(const std::string &ipAddr);
		void	setBuffer(const std::string &buffer);

		bool	isChannelInvited(const std::string &channel) const;
		void	addChannelInvited(const std::string &channel);
		void	removeChannelInvited(const std::string &channel);
		void	appendToBuffer(const std::string &data);
		void	clearBuffer();
};

#endif
