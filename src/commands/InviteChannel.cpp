/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteChannel.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:15:41 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/19 09:52:38 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/Replies.hpp"

/**
 * @brief Handles the INVITE command.
 *
 * Allows a channel operator to invite a client to a channel.
 * Validates client state, channel existence, operator privileges
 * and ensures the invited client is not already in the channel.
 *
 * @param buffer Command parameters (nickname and channel name).
 * @param fd File descriptor of the requesting client.
 */
void Server::_handlerClientInvite(const std::string &buffer, const int fd)
{
	Client				*client = _getClient(fd);
	std::istringstream	iss(buffer);
	std::string			targetNickname, channelName;
	Channel				*channel;
	Client				*invitedClient;
	std::string			inviteConfirm;
	std::string			inviteMsg;

	if (!client || !client->getIsLogged())
	{
		_sendResponse(fd, ERR_NOTREGISTERED(_getHostname(), "*"));
		return;
	}

	//Extract nickname and the name of the channel
	iss >> targetNickname >> channelName;
	if (targetNickname.empty() || channelName.empty())
	{
		_sendResponse(fd, ERR_SYNTAX_JOIN(_getHostname(), "INVITE"));
		return;
	}

	//Verify that the channel and the client exist
	channel = _getChannel(channelName);
	if (!channel)
	{
		_sendResponse(fd, ERR_NOSUCHCHANNEL(_getHostname(), channelName));
		return;
	}
	invitedClient = _getClient(targetNickname);
	if (!invitedClient)
	{
		_sendResponse(fd, ERR_NOSUCHNICK(_getHostname(), targetNickname));
		return;
	}

	//If the channel doesn't have this client
	if (!channel->hasClient(client))
	{
		_sendResponse(fd, ERR_NOTONCHANNEL(_getHostname(), channelName));
		return;
	}

	//If this client isn't operator
	if (!channel->isChannelOperator(client->getNname()))
	{
		_sendResponse(fd, ERR_CHANOPRIVSNEEDED(_getHostname(), client->getNname(), channelName));
		return;
	}

	//Verify that the invited client is in the channel
	if (channel->hasClient(invitedClient))
	{
		_sendResponse(fd, ERR_USERONCHANNEL(_getHostname(), targetNickname, channelName));
		return;
	}

	//Add the invited client + reply
	invitedClient->addChannelInvited(channelName);
	_sendResponse(fd, RPL_INVITING(client->getHostName(), channelName, client->getNname(), targetNickname));
	_sendResponse(invitedClient->getFd(), RPL_INVITE_MSG(client->getNname(), client->getHostName(), targetNickname, channelName));
}
