/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteChannel.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:15:41 by loruzqui          #+#    #+#             */
/*   Updated: 2025/11/30 18:18:41 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/Replies.hpp"

void Server::_handlerClientInvite(const std::string &buffer, const int fd)
{
	Client	*client = _getClient(fd);

	if (!client || !client->getIsLogged())
	{
		_sendResponse(fd, ERR_NOTREGISTERED(_getHostname(), "*"));
		return;
	}

	std::istringstream iss(buffer);
	std::string targetNickname, channelName;

	iss >> targetNickname >> channelName;

	if (targetNickname.empty() || channelName.empty())
	{
		_sendResponse(fd, ERR_MISSINGPARAMS(_getHostname(), "INVITE"));
		return;
	}

	Channel *channel = _getChannel(channelName);
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

	Client	*invitedClient = _getClient(targetNickname);
	if (!invitedClient)
	{
		_sendResponse(fd, ERR_NOSUCHNICK(_getHostname(), targetNickname));
		return;
	}

	if (channel->hasClient(invitedClient))
	{
		_sendResponse(fd, ERR_USERONCHANNEL(_getHostname(), targetNickname, channelName));
		return;
	}

	invitedClient->addChannelInvited(channelName);
	std::string inviteConfirm = RPL_INVITING(
		client->getHostName(),
		channelName,
		client->getNname(),
		targetNickname
	);
	_sendResponse(fd, inviteConfirm);

	std::string inviteMsg = ":" + client->getNname() + "!" + client->getHostName() +
						" INVITE " + targetNickname + " " + channelName + CRLF;
	_sendResponse(invitedClient->getFd(), inviteMsg);
}
