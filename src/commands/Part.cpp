/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:17:18 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/19 09:47:48 by loruzqui         ###   ########.fr       */
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

	//Check if this client is an operator and count total operators BEFORE removing
	bool	wasOperator = channel->isChannelOperator(client->getNname());

	//Count how many operators are in the channel
	int							operatorCount = 0;
	const std::vector<Client*>&	allClients = channel->getChClients();
	for (size_t j = 0; j < allClients.size(); j++)
	{
		if (allClients[j] && channel->isChannelOperator(allClients[j]->getNname()))
			operatorCount++;
	}

	//If this is the last operator, find a replacement BEFORE removing them
	Client*	newOp = NULL;
	if (wasOperator && operatorCount == 1)
	{
		//Get the oldest client that is NOT the one leaving
		for (size_t j = 0; j < allClients.size(); j++)
		{
			if (allClients[j] && allClients[j] != client)
			{
				newOp = allClients[j];
				break;
			}
		}
	}

	//Remove the client from the channel
	channel->part(client);

	//Promote new operator if needed
	if (newOp)
	{
		channel->setChannelOperator(newOp);
		std::string modeMsg =
			":" + _getHostname() +
			" MODE " + channelName +
			" +o " + newOp->getNname() + CRLF;
		_broadcastToChannel(channelName, modeMsg, -1);
	}
}
