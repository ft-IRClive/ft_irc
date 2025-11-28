/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:16:08 by loruzqui          #+#    #+#             */
/*   Updated: 2025/11/28 15:34:06 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

void Server::_handlerClientKick(const std::string &buffer, const int fd)
{
	Client	*client = _getClient(fd);

	if (!client->getIsLogged())
	{
		_sendResponse(fd, ERR_NOTREGISTERED(client->getNname()));
		_replyCode = 451;
		return ;
	}
	std::vector<std::string> params = _splitBuffer(buffer, SPACE);
	if (params.size() < 3)
	{
		_sendResponse(fd, ERR_MISSINGPARAMS(client->getNname()));
		return ;
	}
	std::string channelName = params[1];
	std::string targetNickname = params[2];
	std::string comment = params[0];

	Channel	*channel = _getChannel(channelName);
	if (!channel)
	{
		_sendResponse(fd, ERR_NOSUCHCHANNEL(channelName));
		return ;
	}
	if (!channel->hasClient(client))
	{
		_sendResponse(fd, ERR_NOTONCHANNEL(channelName));
		return ;
	}
	if (!channel->isChannelOperator(client->getNname()))
	{
		_sendResponse(fd, ERR_CHANOPRIVSNEEDED(channelName));
		return ;
	}

	Client	*targetClient = _getClient(targetNickname);
	if (!targetClient)
	{
		_sendResponse(fd, ERR_NONICKNAME(targetNickname));
		return;
	}
	if (!channel->hasClient(targetClient))
	{
		_sendResponse(fd, ERR_USERNOTINCHANNEL(targetNickname, channelName));
		return ;
	}

	std::string				kickMsg = RPL_KICK(client->getHostName(), channelName, client->getNname(), targetClient->getNname(), comment);
	std::vector<Client*>	members = channel->getChClients();

	for (size_t i = 0; i < members.size(); ++i)
		_sendResponse(members[i]->getFd(), kickMsg);
	channel->kick(targetClient);
}
