/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:17:18 by loruzqui          #+#    #+#             */
/*   Updated: 2025/11/26 16:59:22 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

void Server::_handlerClientPart(const std::string &buffer, const int fd)
{
	Client*						client = _getClient(fd);
	std::vector<std::string>	params = _splitBuffer(buffer, SPACE);

	if (buffer.empty())
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
		std::string	channelName = params[0];
		Channel*	channel = _getChannel(channelName);

		if (!channel)
		{
			_sendResponse(fd, ERR_NOSUCHCHANNEL(channelName));
			_replyCode = 403;
		}
		else if (!channel->hasClient(client))
		{
			_sendResponse(fd, ERR_NOTONCHANNEL(channelName));
			_replyCode = 442;
		}
		else
		{
			channel->part(client);
			_sendResponse(fd, RPL_PART(client->getHostName(), channelName, client->getNname()));
			_replyCode = 200;
		}
	}
}
