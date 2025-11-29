/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickName.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:16:41 by loruzqui          #+#    #+#             */
/*   Updated: 2025/11/29 16:08:36 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/Replies.hpp"

/**
 * @brief Stablish or change username
 *
 * @param nickname
 * @param fd
 */
void Server::_handlerClientNickname(const std::string &nickname, const int fd)
{
	Client	*client = _getClient(fd);

	if (nickname.empty())
	{
		_sendResponse(fd, ERR_MISSINGPARAMS(_getHostname(), "NICK"));
		_replyCode = 461;
		return ;
	}
	if (!_isValidNickname(nickname))
	{
		_sendResponse(fd, ERR_INVALIDNICK(_getHostname(), nickname));
		_replyCode = 432;
		return ;
	}
	if (_isNicknameInUse(fd, nickname))
	{
		_sendResponse(fd, ERR_NICKINUSE(_getHostname(), nickname));
		_replyCode = 433;
		return ;
	}
	std::string oldNick = client->getNname();
	client->setNname(nickname);
	if (_clientIsReadyToLogin(fd))
	{
		client->setIsLogged(true);
		_sendWelcome(client);
		return ;
	}
	if (client->getIsLogged())
	{
		std::string nickMsg = ":" + oldNick + " NICK " + nickname + CRLF;
		_sendResponse(fd, nickMsg);
		for (size_t i = 0; i < this->_channels.size(); i++)
		{
			Channel	*ch = this->_channels[i];
			if (ch && ch->hasClient(client))
				_broadcastToChannel(ch->getChName(), nickMsg, fd);
		}
	}
	_replyCode = 0;
}
