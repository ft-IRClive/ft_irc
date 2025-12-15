/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgil <cgil@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:18:18 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/15 16:40:55 by cgil             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

void Server::_handlerClientTopic(const std::string &buffer, const int fd)
{
	Client						*client = _getClient(fd);
	std::vector<std::string>	tokens;
	std::string					channelName;
	Channel						*channel;
	std::string					newTopic;
	std::string					msg;
	size_t						pos;

	if (!client)
		return;

	tokens = _splitBuffer(buffer, " ");

	// Missing channel
	if (tokens.size() < 1 || tokens[0].empty())
	{
		_sendResponse(fd, ERR_MISSINGPARAMS(_hostname, client->getNname()));
		return;
	}

	channelName = tokens[0];
	channel = _getChannel(channelName);
	if (!channel)
	{
		_sendResponse(fd, ERR_NOSUCHCHANNEL(_hostname, channelName));
		return;
	}

	// Only show topic (no ':' means no modification)
	if (buffer.find(':') == std::string::npos)
	{
		if (channel->getChTopic().empty())
			_sendResponse(fd,
				RPL_NOTOPIC(_hostname, client->getNname(), channelName));
		else
			_sendResponse(fd,
				RPL_TOPIC(_hostname, client->getNname(),
					channelName, channel->getChTopic()));
		return;
	}

	// Topic restricted to channel operators
	if (channel->getRestrictedTopic()
		&& !channel->isChannelOperator(client->getNname()))
	{
		_sendResponse(fd,
			ERR_CHANOPRIVSNEEDED(_hostname,
				client->getNname(), channelName));
		return;
	}

	// Extract new topic (allows empty topic)
	pos = buffer.find(':');
	newTopic = buffer.substr(pos + 1);

	channel->setTopic(newTopic);

	msg = ":" + client->getNname() + "!" + client->getHostName()
		+ " TOPIC " + channelName + " :" + newTopic + CRLF;

	_broadcastToChannel(channelName, msg);
}




