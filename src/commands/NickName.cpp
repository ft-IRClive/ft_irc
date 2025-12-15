/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickName.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:16:41 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/13 13:53:24 by loruzqui         ###   ########.fr       */
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
	Client		*client = _getClient(fd);
	std::string	oldNick;
	Channel		*channel;
	std::string	prefix;

	if (nickname.empty())
	{
		_sendResponse(fd, ERR_SYNTAX_NICK(_getHostname(), "NICK"));
		_replyCode = 461;
		return ;
	}

	//Verify that the nickname is valid
	if (!_isValidNickname(nickname))
	{
		_sendResponse(fd, ERR_INVALIDNICK(_getHostname(), nickname));
		_replyCode = 432;
		return ;
	}

	//Verify that the nickname is not in use
	if (_isNicknameInUse(fd, nickname))
	{
		_sendResponse(fd, ERR_NICKINUSE(_getHostname(), nickname));
		_replyCode = 433;
		return ;
	}
	oldNick = client->getNname();
	client->setNname(nickname);
	if (_clientIsReadyToLogin(fd))
	{
		client->setIsLogged(true);
		_sendWelcome(client);
		return ;
	}
	if (client->getIsLogged())
	{
		prefix = oldNick + "!" + client->getHostName();
		_sendResponse(fd, RPL_NICK(prefix, nickname));
		for (size_t i = 0; i < this->_channels.size(); i++)
		{
			channel = this->_channels[i];
			if (channel && channel->hasClient(client))
				_broadcastToChannel(channel->getChName(), RPL_NICK(prefix, nickname), fd);
		}
	}
	_replyCode = 0;
}
