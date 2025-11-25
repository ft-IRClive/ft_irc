/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteChannel.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:15:41 by loruzqui          #+#    #+#             */
/*   Updated: 2025/11/25 17:50:56 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/Replies.hpp"

void Server::_handlerClientInvite(const std::string &buffer, const int fd)
{
	Client*						client = _getClient(fd);
	std::vector<std::string>	params = _splitBuffer(buffer, SPACE);

	if (params.size() < 2)
	{
		_sendResponse(fd, ERR_MISSINGPARAMS(client->getNname()));
		_replyCode = 461;
	}
	else if (!client->getIsLogged())
	{
		_sendResponse(fd, ERR_NOTREGISTERED(client->getNname()));
		_replyCode = 451;
	}
	else
	{
		std::string	targetNName = params[0];
		std::string	targetChannel = params[1];

		Client*		invitedClient = _getClient(targetChannel);
		Channel*	channel = _getChannel(targetChannel);

		if (!channel)
		{
			_sendResponse(fd, ERR_NOSUCHCHANNEL(targetChannel));
			_replyCode = 403;
		}
		else if (!channel->hasClient(client))
		{
			_sendResponse(fd, ERR_NOTONCHANNEL(targetChannel));
			_replyCode = 442;
		}
		else if (!channel->isChannelOperator(client->getNname()))
		{
			_sendResponse(fd, ERR_NOPRIVILEGES(client->getNname()));
			_replyCode = 481;
		}
		else if (!invitedClient)
		{
			_sendResponse(fd, ERR_NONICKNAME(targetNName));
			_replyCode = 401;
		}
		else if (channel->hasClient(invitedClient))
		{
			_sendResponse(fd, ERR_USERONCHANNEL(targetNName, targetChannel));
			_replyCode = 443;
		}
		else
		{
			invitedClient->addChannelInvited(targetChannel);
			_sendResponse(fd, RPL_INVITING(client->getHostName(), targetChannel, client->getNname(), targetNName));
			_replyCode = 200;
		}
	}
}
