/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:18:18 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/10 17:12:57 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

void Server::_handlerClientTopic(const std::string &buffer, const int fd)
{
	Client	*client = _getClient(fd);
	if (!client)
		return;
	std::vector<std::string> tokens = _splitBuffer(buffer, " ");
	if (tokens.size() < 1)
	{
		_sendResponse(fd, ERR_MISSINGPARAMS(_hostname, client->getNname()));
		return;
	}
	std::string	channelName = tokens[0];
	Channel		*channel = _getChannel(channelName);
	if (!channel)
	{
		_sendResponse(fd, ERR_NOSUCHCHANNEL2(_hostname, client->getNname(), channelName));
		return;
	}
	if (tokens[1].empty())
	{
		if (channel->getChTopic().empty())
			_sendResponse(fd, RPL_NOTOPIC(_hostname, client->getNname(), channelName));
		else
			_sendResponse(fd, RPL_TOPIC(_hostname, client->getNname(), channelName, channel->getChTopic()));
		return;
	}
	if (channel->getRestrictedTopic() && !channel->isChannelOperator(client->getNname()))
	{
		_sendResponse(fd, ERR_CHANOPRIVSNEEDED(_hostname, client->getNname(), channelName));
		return;
	}
	std::string	newTopic = buffer.substr(buffer.find(":", 1) + 1);
	channel->setTopic(newTopic);
	std::string	msg =
		":" + client->getNname() + "!" + client->getHostName() +
		" TOPIC " + channelName + " :" + newTopic + CRLF;
	_broadcastToChannel(channelName, msg);
}
