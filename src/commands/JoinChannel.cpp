/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinChannel.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:15:56 by loruzqui          #+#    #+#             */
/*   Updated: 2025/11/26 18:28:35 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

void Server::_handlerClientJoin(const std::string &buffer, const int fd)
{
	Client						*client = _getClient(fd);
	std::vector<std::string>	params = _splitBuffer(buffer, SPACE);

	if (params.empty())
	{
		_sendResponse(fd, ERR_MISSINGPARAMS(client->getNname()));
		return;
	}
	if (!client->getIsLogged())
	{
		_sendResponse(fd, ERR_NOTREGISTERED(client->getNname()));
		return;
	}

	std::string	channelName = params[0];
	std::string	key = (params.size() > 1) ? params[1] : "";

	if (channelName.empty() || channelName[0] != '#')
	{
		_sendResponse(fd, ERR_BADCHANMASK(_getHostname(), client->getNname(), channelName));
		return;
	}

	Channel	*channel = _getChannel(channelName);

	if (!channel)
	{
		channel = new Channel(channelName);
		_addChannel(channel);

		channel->join(client);
		channel->setChannelOperator(client);

		/*channel->broadcast(
			RPL_JOINMSG(client->getNname(), client->getHostName(), channelName),
			NULL
		);*/

		if (channel->getChTopic().empty())
			_sendResponse(fd, RPL_NOTOPIC(client->getNname(), channelName));
		else
			_sendResponse(fd, RPL_TOPIC(client->getNname(), channelName, channel->getChTopic()));

		_sendResponse(fd, RPL_NAMREPLY(client->getNname(), channelName, channel->getChannelNames()));
		_sendResponse(fd, RPL_ENDOFNAMES(client->getNname(), channelName));
		return;
	}

	if (channel->hasClient(client))
		return;

	if (channel->isChannelComplete())
	{
		_sendResponse(fd, ERR_CHANNELISFULL(client->getNname(), channelName));
		return;
	}

	if (channel->isChannelInviteOnly() && !client->isChannelInvited(channelName))
	{
		_sendResponse(fd, ERR_INVITEONLYCHAN(client->getNname(), channelName));
		return;
	}

	if (channel->hasKey() && key != channel->getChKey())
	{
		_sendResponse(fd, ERR_BADCHANNELKEY(client->getNname(), channelName));
		return;
	}

	channel->join(client);
	client->removeChannelInvited(channelName);

	/*channel->broadcast(
		RPL_JOINMSG(client->getNname(), client->getHostName(), channelName),
		NULL
	);*/

	if (channel->getChTopic().empty())
		_sendResponse(fd, RPL_NOTOPIC(client->getNname(), channelName));
	else
		_sendResponse(fd, RPL_TOPIC(client->getNname(), channelName, channel->getChTopic()));

	_sendResponse(fd, RPL_NAMREPLY(client->getNname(), channelName, channel->getChannelNames()));
	_sendResponse(fd, RPL_ENDOFNAMES(client->getNname(), channelName));
}
