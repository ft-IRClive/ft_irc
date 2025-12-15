/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinChannel.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:15:56 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/15 16:17:25 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

void Server::_handlerClientJoin(const std::string &buffer, const int fd)
{
	Client				*client = _getClient(fd);
	std::istringstream	iss(buffer);
	std::string			channelName;
	std::string			key;
	Channel				*channel;

	if (!client || !client->getIsLogged())
	{
		_sendResponse(fd, ERR_NOTREGISTERED(_getHostname(), "*"));
		return;
	}

	//Extract the name of the channel and the key
	iss >> channelName >> key;
	if (channelName.empty())
	{
		_sendResponse(fd, ERR_SYNTAX_JOIN(_getHostname(), client->getNname()));
		return;
	}

	//Verify that the channel is correct
	if (channelName.empty() || channelName[0] != '#')
	{
		_sendResponse(fd, ERR_BADCHANMASK(_getHostname(), client->getNname(), channelName));
		return;
	}
	channel = _getChannel(channelName);
	//If the channel doesn't exists, we create it and we join the client
	if (!channel)
	{
		channel = new Channel(channelName);
		_addChannel(channel);
		channel->join(client);
		channel->setChannelOperator(client);
		_broadcastToChannel(channelName,
			RPL_JOINMSG(client->getNname(), client->getHostName(), channelName),
			-1);

		//Verify if the channel has topic or not
		if (channel->getChTopic().empty())
			_sendResponse(fd, RPL_NOTOPIC(_getHostname(), client->getNname(), channelName));
		else
			_sendResponse(fd, RPL_TOPIC(_getHostname(), client->getNname(), channelName, channel->getChTopic()));

		_sendResponse(fd, RPL_NAMREPLY(_getHostname(), client->getNname(), channelName, channel->getChannelNames()));
		_sendResponse(fd, RPL_ENDOFNAMES(_getHostname(), client->getNname(), channelName));
		return;
	}

	//If the client is already in the channel
	if (channel->hasClient(client))
		return;

	//If the channel is complete
	if (channel->isChannelComplete())
	{
		_sendResponse(fd, ERR_CHANNELISFULL(_getHostname(), client->getNname(), channelName));
		return;
	}

	//If the channel is "invite only"
	if (channel->isChannelInviteOnly() && !client->isChannelInvited(channelName))
	{
		_sendResponse(fd, ERR_INVITEONLYCHAN(_getHostname(), client->getNname(), channelName));
		return;
	}

	//If the channel has password
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

	//Verify if the channel has topic or not
	if (channel->getChTopic().empty())
		_sendResponse(fd, RPL_NOTOPIC(_getHostname(), client->getNname(), channelName));
	else
		_sendResponse(fd, RPL_TOPIC(_getHostname(), client->getNname(), channelName, channel->getChTopic()));

	_sendResponse(fd, RPL_NAMREPLY(_getHostname(), client->getNname(), channelName, channel->getChannelNames()));
	_sendResponse(fd, RPL_ENDOFNAMES(_getHostname(), client->getNname(), channelName));
}
