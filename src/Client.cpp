/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claudia <claudia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:12:25 by loruzqui          #+#    #+#             */
/*   Updated: 2025/11/20 13:18:09 by claudia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

Client:: Client(int fd)
{
    _fd = fd;
    _isOp = false;
    _isLogged = false;
    _isAuthenticated = false;
    _nname = "";
    _uname = "";
    _pswd = "";
    _ipAddr = "";
    _buffer = "";
    _channelsInvited = std::vector<std::string>();
}

Client::~Client()
{
    
}

int Client::getFd() const
{
    return (_fd);
}

bool Client::getIsOp() const
{
    return (_isOp);
}

bool Client::getIsLogged() const
{
    return (_isLogged);
}

bool Client::getIsAuthenticated() const
{
    return (_isAuthenticated);
}

std::string Client:: getNname() const
{
    return (_nname);   
}

std::string Client:: getUname() const
{
    return (_uname);
}

std::string Client:: getPswd() const
{
    return (_pswd);
}

std::string Client:: getIpAddr() const
{
    return (_ipAddr);
}

std::string Client:: getBuffer() const
{
    return (_buffer);
}

std::string Client:: getHostName() const
{
    return (_uname + "@" + _ipAddr);
}

std::vector<std::string> Client:: getChannelsInvited() const
{
    return (_channelsInvited);
}

void Client:: setOp(bool isOp)
{
    _isOp = isOp;
}

void Client:: setIsLogged(bool isLogged)
{
    _isLogged = isLogged;
}

void Client:: setIsAuthenticated(bool isAuthenticated)
{
    _isAuthenticated = isAuthenticated;
}

void Client:: setNname(const std::string &nname)
{
    _nname = nname;
}

void Client:: setUname(const std::string &uname)
{
    _uname = uname;
}
void Client:: setPswd(const std::string &pswd)
{
    _pswd = pswd;
}
void Client:: setIpAddr(const std::string &ipAddr)
{
    _ipAddr = ipAddr;
}

void Client:: setBuffer(const std::string &buffer)
{
    _buffer = buffer;
}

bool Client:: isChannelInvited(const std::string &channel) const
{
    for (size_t i = 0; i < _channelsInvited.size(); ++i)
    {
        if (_channelsInvited[i] == channel)
            return (true);
    }
    return (false);
}

void Client:: addChannelInvited(const std::string &channel)
{
    if (!isChannelInvited(channel))
        _channelsInvited.push_back(channel);
}

void Client::removeChannelInvited(const std::string &channel)
{
    for (std::vector<std::string>::iterator it = _channelsInvited.begin(); it != _channelsInvited.end(); ++it)
    {
        if (*it == channel)
        {
            _channelsInvited.erase(it);
            break;
        }
    }
}

void Client::appendToBuffer(const std::string &data)
{
    _buffer += data;
}

void Client::clearBuffer()
{
    _buffer.clear();
}