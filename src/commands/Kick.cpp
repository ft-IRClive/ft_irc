/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:16:08 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/11 21:09:15 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

void Server::_handlerClientKick(const std::string &buffer, const int fd)
{
	Client				*client = _getClient(fd);
	std::istringstream	iss(buffer);
	std::string			prefix, command, channelName, targetNickname, reason;
	Channel				*channel;
	Client				*targetClient;
	std::string			kickMsg;

	//Verify if the client is logged correctly
	if (!client || !client->getIsLogged())
	{
		std::string nick = (client ? client->getNname() : "*");
		_sendResponse(fd, ERR_NOTREGISTERED(_getHostname(), nick));
		return;
	}
	if (!buffer.empty() && buffer[0] == ':')
		iss >> prefix;

	//Extract the name of the command
	iss >> command;

	//Verify the name of the channel and the nickname are correct
	iss >> channelName >> targetNickname;
	if (!targetNickname.empty() && targetNickname[0] == ':')
		targetNickname.erase(0, 1);

	//Extract the reason (if exists)
	std::getline(iss >> std::ws, reason);
	if (!reason.empty() && reason[0] == ':')
		reason = reason.substr(1);
	if (reason.empty())
		reason = "No reason given";
	if (channelName.empty() || targetNickname.empty())
	{
		_sendResponse(fd, ERR_MISSINGPARAMS(_getHostname(), "KICK"));
		return;
	}

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
