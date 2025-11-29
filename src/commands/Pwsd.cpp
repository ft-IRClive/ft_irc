/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pwsd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 15:09:01 by claudia           #+#    #+#             */
/*   Updated: 2025/11/29 15:44:47 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Replies.hpp"
#include "../../inc/Server.hpp"

void Server::_handlerClientPassword(const std::string &password, const int fd)
{
	Client	*client = _getClient(fd);

	if (password.empty())
	{
		_sendResponse(fd, ERR_MISSINGPARAMS(_getHostname(), "PASSWORD"));
		_replyCode = 461;
		return ;
	}
	if (client->getIsAuthenticated())
	{
		_sendResponse(fd, ERR_ALREADYREGISTERED(_getHostname(), "*"));
		_replyCode = 462; return ;
	}
	if (_passwd != password)
	{
		_sendResponse(fd, ERR_INCORPASS(_getHostname(), "*"));
		_replyCode = 464;
		return ;
	}
	client->setIsAuthenticated(true);
	_replyCode = 200;
}
