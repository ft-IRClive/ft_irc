/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgil <cgil@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:18:18 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/18 18:14:13 by cgil             ###   ########.fr       */
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
	Client      *client;
	Channel     *channel;
	std::string channelName;
	std::string newTopic;
	std::string msg;
	size_t      pos;
	size_t      start;

	client = _getClient(fd);
	if (!client)
		return;

	if (!client->getIsLogged())
	{
		_sendResponse(fd, ERR_NOTREGISTERED(_hostname, "*"));
		return;
	}

	// extract channel name 
	std::istringstream iss(buffer);
	iss >> channelName;

	if (channelName.empty())
	{
		_sendResponse(fd,
			ERR_SYNTAX_TOPIC(_hostname, client->getNname()));
		return;
	}

	channel = _getChannel(channelName);
	if (!channel)
	{
		_sendResponse(fd,
			ERR_NOSUCHCHANNEL(_hostname, channelName));
		return;
	}

	// GET topic 
	start = buffer.find(channelName) + channelName.length();
	if (start >= buffer.length() - 1)
	{
		if (channel->getChTopic().empty())
			_sendResponse(fd,
				RPL_NOTOPIC(_hostname,
					client->getNname(), channelName));
		else
			_sendResponse(fd,
				RPL_TOPIC(_hostname,
					client->getNname(),
					channelName, channel->getChTopic()));
		return;
	}

	//  SET topic (permission check) 
	if (channel->getRestrictedTopic()
		&& !channel->isChannelOperator(client->getNname()))
	{
		_sendResponse(fd,
			ERR_CHANOPRIVSNEEDED(_hostname,
				client->getNname(), channelName));
		return;
	}

	// extract topic 
	pos = buffer.find(':');
	if (pos != std::string::npos)
		newTopic = buffer.substr(pos + 1);
	else
	{
		while (start < buffer.length() && buffer[start] == ' ')
			start++;
		newTopic = buffer.substr(start);
	}

	channel->setTopic(newTopic);

	msg = ":" + client->getNname() + "!" + client->getHostName()
		+ " TOPIC " + channelName + " :" + newTopic + CRLF;

	_broadcastToChannel(channelName, msg);
}

