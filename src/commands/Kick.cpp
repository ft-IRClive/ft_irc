/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:16:08 by loruzqui          #+#    #+#             */
/*   Updated: 2025/11/29 17:19:47 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

void Server::_handlerClientKick(const std::string &buffer, const int fd)
{
	Client	*client = _getClient(fd);

	if (!client || !client->getIsLogged())
	{
		_sendResponse(fd, ERR_NOTREGISTERED(_getHostname(), client->getNname()));
		return;
	}

	std::istringstream iss(buffer);
	std::string channelName, targetNickname, reason;

	iss >> channelName >> targetNickname;
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

	Channel	*channel = _getChannel(channelName);
	if (!channel)
	{
		_sendResponse(fd, ERR_NOSUCHCHANNEL(_getHostname(), channelName));
		return;
	}

	if (!channel->hasClient(client))
	{
		_sendResponse(fd, ERR_NOTONCHANNEL(_getHostname(), channelName));
		return;
	}

	if (!channel->isChannelOperator(client->getNname()))
	{
		_sendResponse(fd, ERR_CHANOPRIVSNEEDED(_getHostname(), channelName));
		return;
	}

	Client *targetClient = _getClient(targetNickname);
	if (!targetClient)
	{
		_sendResponse(fd, ERR_NOSUCHNICK(_getHostname(), targetNickname));
		return;
	}

	if (!channel->hasClient(targetClient))
	{
		_sendResponse(fd, ERR_USERNOTINCHANNEL(_getHostname(), targetNickname, channelName));
		return;
	}

	std::string kickMsg = RPL_KICK(
		client->getHostName(),
		channelName,
		client->getNname(),
		targetClient->getNname(),
		reason
	);

	_broadcastToChannel(channelName, kickMsg, -1);

	channel->kick(targetClient);
}
