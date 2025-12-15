/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:18:18 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/15 17:10:39 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/Replies.hpp"

void Server::_handlerClientTopic(const std::string &buffer, const int fd)
{
	Client						*client = _getClient(fd);
	std::istringstream	iss(buffer);
	std::string					channelName;
	Channel						*channel;
	std::string					newTopic;
	std::string					msg;
	size_t						pos;

	if (!client)
		return;

	if (!client || !client->getIsLogged())
	{
		_sendResponse(fd, ERR_NOTREGISTERED(_getHostname(), "*"));
		return;
	}

	// Missing channel
	iss >> channelName >> newTopic;
	if (channelName.empty())
	{
		_sendResponse(fd, ERR_SYNTAX_TOPIC(_hostname, client->getNname()));
		return;
	}

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
