/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserName.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:18:27 by loruzqui          #+#    #+#             */
/*   Updated: 2025/11/24 15:38:32 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

/**
 * @brief Registers user basic info and indicates wether the username has been set
 *
 * @param username
 * @param fd
 */
void Server::_handlerClientUsername(const std::string &username, const int fd)
{
	Client	*client = _getClient(fd);

	if (username.empty())
	{
		_sendResponse(fd, ERR_MISSINGPARAMS("USER"));
		_replyCode = 461;
		return ;
	}
	if (!client || !client->getIsAuthenticated())
	{
		_sendResponse(fd, ERR_NOTREGISTERED("USER"));
		_replyCode = 451;
		return ;
	}
	if (!client->getUname().empty())
	{
		_sendResponse(fd, ERR_ALREADYREGISTERED(client->getNname()));
		_replyCode = 462;
		return ;
	}
	client->setUname(username);
	if (_clientIsReadyToLogin(fd))
	{
		client->setIsLogged(true);
		_sendResponse(fd, RPL_WELCOME(client->getNname(), client->getUname(), client->getHostName()));
		_replyCode = 001;
	}
	else
	{
		_replyCode = 200;
	}
}
