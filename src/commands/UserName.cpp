/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserName.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:18:27 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/11 21:19:01 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include <sstream>

/**
 * @brief Registers user basic info and indicates wether the username has been set
 *
 * @param username
 * @param fd
 */
void Server::_handlerClientUsername(const std::string &parameters, const int fd)
{
	Client				*client = _getClient(fd);
	std::istringstream	iss(parameters);
	std::string			username, mode, unused, realname;

	if (parameters.empty())
	{
		_sendResponse(fd, ERR_MISSINGPARAMS(_getHostname(), "USER"));
		_replyCode = 461;
		return;
	}
	if (!client || !client->getIsAuthenticated())
	{
		_sendResponse(fd, ERR_NOTREGISTERED(_getHostname(), "USER"));
		_replyCode = 451;
		return;
	}

	//The client doesn't have username
	if (!client->getUname().empty())
	{
		_sendResponse(fd, ERR_ALREADYREGISTERED(_getHostname(), client->getNname()));
		_replyCode = 462;
		return;
	}

	//Extract the username
	iss >> username;
	iss >> mode >> unused;
	std::getline(iss >> std::ws, realname);
	if (!realname.empty() && realname[0] == ':')
		realname = realname.substr(1);

	//Validate username
	if (username.empty())
	{
		_sendResponse(fd, ERR_MISSINGPARAMS(_getHostname(), "USER"));
		_replyCode = 461;
		return;
	}

	//Save username
	client->setUname(username);

	//Save realname if exists
	if (!realname.empty())
		client->setRealName(realname);

	if (_clientIsReadyToLogin(fd))
	{
		client->setIsLogged(true);
		_sendWelcome(client);
	}
	else
		_replyCode = 200;
}
