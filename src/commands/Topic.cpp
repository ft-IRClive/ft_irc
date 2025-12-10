/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmaccha- <gmaccha-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:18:18 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/10 16:06:09 by gmaccha-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

void Server::_handlerClientTopic(const std::string &buffer, const int fd)
{
	Client *client = _getClient(fd);
    if (!client) return;

    std::vector<std::string> tokens = _splitBuffer(buffer, " ");

    if (tokens.size() < 2)
    {
        _sendResponse(fd, ERR_MISSINGPARAMS(_hostname, client->getNname()));
        return;
    }

    std::string channelName = tokens[1];
    Channel *channel = _getChannel(channelName);

    if (!channel)
    {
        _sendResponse(fd, ERR_NOSUCHCHANNEL2(_hostname, client->getNname(), channelName));
        return;
    }

    // Mostrar topic
    if (tokens.size() == 2)
    {
        if (channel->getChTopic().empty())
            _sendResponse(fd, RPL_NOTOPIC(_hostname, client->getNname(), channelName));
        else
            _sendResponse(fd, RPL_TOPIC(_hostname, client->getNname(), channelName, channel->getChTopic()));
        return;
    }

    // Cambiar topic
    if (channel->getRestrictedTopic() && !channel->isChannelOperator(client->getFd()))
    {
        _sendResponse(fd, ERR_CHANOPRIVSNEEDED(_hostname, client->getNname(), channelName));
        return;
    }

    std::string newTopic = buffer.substr(buffer.find(":", 1) + 1);
    channel->setTopic(newTopic);

    std::string msg =
        ":" + client->getNname() + "!" + client->getHostName() +
        " TOPIC " + channelName + " :" + newTopic + CRLF;

    _broadcastToChannel(channelName, msg);
}
