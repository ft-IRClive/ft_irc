/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui < >                               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:16:08 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/19 17:53:49 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

/**
 * @brief Handles the KICK command.
 *
 * Allows a channel operator to remove a client from a channel.
 * Validates parameters, channel existence, operator privileges
 * and sends a kick notification to all channel members.
 *
 * @param buffer Command parameters (channel, nickname and reason).
 * @param fd File descriptor of the requesting client.
 */
void Server::_handlerClientKick(const std::string &buffer, const int fd)
{
	Client				*client = _getClient(fd);
	std::istringstream	iss(buffer);
	std::string			channelName, targetNickname, reason;
	Channel				*channel;
	Client				*targetClient;
	std::string			kickMsg;

	//Verify if the client is logged correctly
	if (!client || !client->getIsLogged())
	{
		_sendResponse(fd, ERR_NOTREGISTERED(_getHostname(), "*"));
		return;
	}

	//Extract channel name and target nickname
	iss >> channelName >> targetNickname;
	if (channelName.empty() || targetNickname.empty())
	{
		_sendResponse(fd, ERR_SYNTAX_KICK(_getHostname(), client->getNname()));
		return;
	}

	//Extract the reason (if exists)
	std::getline(iss >> std::ws, reason);
	if (!reason.empty() && reason[0] == ':')
		reason = reason.substr(1);
	if (reason.empty())
		reason = "No reason given";

	//Get the channel
	channel = _getChannel(channelName);
	if (!channel)
	{
		_sendResponse(fd, ERR_NOSUCHCHANNEL(_getHostname(), channelName));
		return;
	}

	//Verify that the principal client is in the channel
	if (!channel->hasClient(client))
	{
		_sendResponse(fd, ERR_NOTONCHANNEL(_getHostname(), channelName));
		return;
	}

	//Verify that the principal client is operator
	if (!channel->isChannelOperator(client->getNname()))
	{
		_sendResponse(fd, ERR_CHANOPRIVSNEEDED(_getHostname(), client->getNname(), channelName));
		return;
	}

	//Verify that the client to kick out exists
	targetClient = _getClient(targetNickname);
	if (!targetClient)
	{
		_sendResponse(fd, ERR_NOSUCHNICK(_getHostname(), targetNickname));
		return;
	}

	//Verify that the client to kick out is in the channel
	if (!channel->hasClient(targetClient))
	{
		_sendResponse(fd, ERR_USERNOTINCHANNEL(_getHostname(), targetNickname, channelName));
		return;
	}
	kickMsg = RPL_KICK(
		client->getHostName(),
		channelName,
		client->getNname(),
		targetClient->getNname(),
		reason
	);
	_broadcastToChannel(channelName, kickMsg, -1);
	channel->kick(targetClient);
}
