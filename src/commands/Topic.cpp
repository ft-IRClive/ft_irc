/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmaccha- <gmaccha-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:18:18 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/18 01:20:24 by gmaccha-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/Replies.hpp"

/**
 * @brief Handles the TOPIC command.
 * 
 * Displays or updates a channel topic. If no new topic is provided,
 * the current topic is sent to the requesting client. If topic changes
 * are restricted, only channel operators are allowed to modify it.
 * 
 * @param buffer Command parameters (channel and optional topic).
 * @param fd File descriptor of the requesting client.
 */
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
