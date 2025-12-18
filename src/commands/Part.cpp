/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmaccha- <gmaccha-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:17:18 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/18 01:38:47 by gmaccha-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

/**
 * @brief Handles the PART command.
 *
 * Removes a client from a channel and broadcasts the PART message
 * with an optional reason to all channel members.
 *
 * @param buffer Command parameters (channel name and optional message).
 * @param fd File descriptor of the requesting client.
 */

void Server::_handlerClientPart(const std::string &buffer, const int fd)
{
	Client*				client = _getClient(fd);
	std::istringstream	iss(buffer);
	std::string			channelName;
	std::string			partMsg;
	Channel*			channel;
	std::string			msg;

	//Verify that is logged correctly
	if (!client || !client->getIsLogged())
	{
		_sendResponse(fd, ERR_NOTREGISTERED(_getHostname(), "*"));
		return;
	}

	//Extract the name of the channel
	iss >> channelName;
	if (channelName.empty())
	{
		_sendResponse(fd, ERR_SYNTAX_PART(_getHostname(), client->getNname()));
		return;
	}

	//Extract the message (if exists)
	std::getline(iss >> std::ws, partMsg);
	if (!partMsg.empty() && partMsg[0] == ':')
		partMsg = partMsg.substr(1);

	//Verify that the channel exists
	channel = _getChannel(channelName);
	if (!channel)
	{
		_sendResponse(fd, ERR_NOSUCHCHANNEL(_getHostname(), channelName));
		return;
	}

	//Verify that the channel has this client
	if (!channel->hasClient(client))
	{
		_sendResponse(fd, ERR_NOTONCHANNEL(_getHostname(), channelName));
		return;
	}

	//Build and broadcast PART message
	msg = ":" + client->getNname() + "!" +
		client->getUname() + "@" +
		client->getHostName() +
		" PART " + channelName;
	if (!partMsg.empty())
		msg += " :" + partMsg;
	msg += CRLF;
	_broadcastToChannel(channelName, msg, -1);
	channel->part(client);
}
