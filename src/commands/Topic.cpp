/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:18:18 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/11 20:35:19 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

void Server::_handlerClientTopic(const std::string &buffer, const int fd)
{
	Client						*client = _getClient(fd);
	std::vector<std::string>	tokens;
	std::string					channelName;
	Channel						*channel;
	std::string					newTopic;
	std::string					msg;

	if (!client)
		return;
	tokens = _splitBuffer(buffer, " ");
	if (tokens.size() < 1)
	{
		_sendResponse(fd, ERR_MISSINGPARAMS(_hostname, client->getNname()));
		return;
	}
	channelName = tokens[0];
	channel = _getChannel(channelName);
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
	newTopic = buffer.substr(buffer.find(":", 1) + 1);
	channel->setTopic(newTopic);
	msg =
		":" + client->getNname() + "!" + client->getHostName() +
		" TOPIC " + channelName + " :" + newTopic + CRLF;
	_broadcastToChannel(channelName, msg);
}
