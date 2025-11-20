/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:12:25 by loruzqui          #+#    #+#             */
/*   Updated: 2025/11/20 17:03:21 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

Client::Client(int fd)
{
	this->_fd = fd;
	this->_isOp = false;
	this->_isLogged = false;
	this->_isAuthenticated = false;
	this->_nname = "";
	this->_uname = "";
	this->_pswd = "";
	this->_ipAddr = "";
	this->_buffer = "";
	this->_channelsInvited = std::vector<std::string>();
}

int Client::getFd() const
{
	return (this->_fd);
}

bool Client::getIsOp() const
{
	return (this->_isOp);
}

bool Client::getIsLogged() const
{
	return (this->_isLogged);
}

bool Client::getIsAuthenticated() const
{
	return (this->_isAuthenticated);
}

std::string Client:: getNname() const
{
	return (this->_nname);
}

std::string Client:: getUname() const
{
	return (this->_uname);
}

std::string Client:: getPswd() const
{
	return (this->_pswd);
}

std::string Client:: getIpAddr() const
{
	return (this->_ipAddr);
}

std::string Client:: getBuffer() const
{
	return (this->_buffer);
}

std::string Client:: getHostName() const
{
	return (this->_uname + "@" + this->_ipAddr);
}

std::vector<std::string> Client:: getChannelsInvited() const
{
	return (this->_channelsInvited);
}

void Client::setFd(const int fd)
{
	this->_fd = fd;
}

void Client:: setOp(bool isOp)
{
	this->_isOp = isOp;
}

void Client:: setIsLogged(bool isLogged)
{
	this->_isLogged = isLogged;
}

void Client:: setIsAuthenticated(bool isAuthenticated)
{
	this->_isAuthenticated = isAuthenticated;
}

void Client:: setNname(const std::string &nname)
{
	this->_nname = nname;
}

void Client:: setUname(const std::string &uname)
{
	this->_uname = uname;
}
void Client:: setPswd(const std::string &pswd)
{
	this->_pswd = pswd;
}
void Client:: setIpAddr(const std::string &ipAddr)
{
	this->_ipAddr = ipAddr;
}

void Client:: setBuffer(const std::string &buffer)
{
	this->_buffer = buffer;
}

bool Client:: isChannelInvited(const std::string &channel) const
{
	for (size_t i = 0; i < this->_channelsInvited.size(); ++i)
	{
		if (this->_channelsInvited[i] == channel)
			return (true);
	}
	return (false);
}

void Client:: addChannelInvited(const std::string &channel)
{
	if (!isChannelInvited(channel))
		this->_channelsInvited.push_back(channel);
}

void Client::removeChannelInvited(const std::string &channel)
{
	for (std::vector<std::string>::iterator it = this->_channelsInvited.begin(); it != this->_channelsInvited.end(); ++it)
	{
		if (*it == channel)
		{
			this->_channelsInvited.erase(it);
			break;
		}
	}
}

void Client::appendToBuffer(const std::string &data)
{
	this->_buffer += data;
}

void Client::clearBuffer()
{
	this->_buffer.clear();
}
