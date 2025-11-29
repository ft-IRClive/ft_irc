/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinChannel.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:15:56 by loruzqui          #+#    #+#             */
/*   Updated: 2025/11/29 15:43:19 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

void Server::_handlerClientJoin(const std::string &buffer, const int fd)
{
	Client						*client = _getClient(fd);
	std::vector<std::string>	params = _splitBuffer(buffer, SPACE);

	if (params.empty())
	{
		_sendResponse(fd, ERR_MISSINGPARAMS(_getHostname(), client->getNname()));
		return;
	}
	if (!client->getIsLogged())
	{
		_sendResponse(fd, ERR_NOTREGISTERED(_getHostname(), client->getNname()));
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

		//When you create the channel and the client join the channel for first time
		_broadcastToChannel(channelName,
			RPL_JOINMSG(client->getNname(), client->getHostName(), channelName),
			-1);

		if (channel->getChTopic().empty())
			_sendResponse(fd, RPL_NOTOPIC(_getHostname(), client->getNname(), channelName));
		else
			_sendResponse(fd, RPL_TOPIC(_getHostname(), client->getNname(), channelName, channel->getChTopic()));

		_sendResponse(fd, RPL_NAMREPLY(_getHostname(), client->getNname(), channelName, channel->getChannelNames()));
		_sendResponse(fd, RPL_ENDOFNAMES(_getHostname(), client->getNname(), channelName));
		return;
	}

	if (channel->hasClient(client))
		return;

	if (channel->isChannelComplete())
	{
		_sendResponse(fd, ERR_CHANNELISFULL(_getHostname(), client->getNname(), channelName));
		return;
	}

	if (channel->isChannelInviteOnly() && !client->isChannelInvited(channelName))
	{
		_sendResponse(fd, ERR_INVITEONLYCHAN(_getHostname(), client->getNname(), channelName));
		return;
	}

	if (channel->hasKey() && key != channel->getChKey())
	{
		_sendResponse(fd, ERR_BADCHANNELKEY(_getHostname(), client->getNname(), channelName));
		return;
	}

	channel->join(client);
	client->removeChannelInvited(channelName);

	//When a client join an existing channel
	_broadcastToChannel(channelName,
		RPL_JOINMSG(client->getNname(), client->getHostName(), channelName),
		-1);

	if (channel->getChTopic().empty())
		_sendResponse(fd, RPL_NOTOPIC(_getHostname(), client->getNname(), channelName));
	else
		_sendResponse(fd, RPL_TOPIC(_getHostname(), client->getNname(), channelName, channel->getChTopic()));

	_sendResponse(fd, RPL_NAMREPLY(_getHostname(), client->getNname(), channelName, channel->getChannelNames()));
	_sendResponse(fd, RPL_ENDOFNAMES(_getHostname(), client->getNname(), channelName));
}
