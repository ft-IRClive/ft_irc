/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claudia <claudia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:17:18 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/03 12:12:38 by claudia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

void Server::_handlerClientPart(const std::string &parameters, const int fd)
{
    Client* client = _getClient(fd);
    std::string server = _getHostname();
    std::string nick = client ? client->getNname() : "";

    if (!client || !client->getIsLogged())
    {
        _sendResponse(fd, ERR_NOTREGISTERED(server, nick));
        _replyCode = 451;
        return;
    }

    if (parameters.empty())
    {
        _sendResponse(fd, ERR_MISSINGPARAMS(server, nick));
        _replyCode = 461;
        return;
    }

    std::string channelName;
    std::string partMsg;
    std::istringstream iss(parameters);

    iss >> channelName;                 
    std::getline(iss, partMsg);        

    if (!partMsg.empty() && partMsg[0] == ' ')
        partMsg.erase(0, 1);
    if (!partMsg.empty() && partMsg[0] == ':')
        partMsg.erase(0, 1);

    Channel* channel = _getChannel(channelName);

    if (!channel)
    {
        _sendResponse(fd, ERR_NOSUCHCHANNEL(server, channelName));
        _replyCode = 403;
        return;
    }

    if (!channel->hasClient(client))
    {
        _sendResponse(fd, ERR_NOTONCHANNEL(server, channelName));
        _replyCode = 442;
        return;
    }

    std::string msg = ":" + client->getNname() + "!" +
                      client->getUname() + "@" +
                      client->getHostName() +
                      " PART " + channelName;

    if (!partMsg.empty())
        msg += " :" + partMsg;

    msg += CRLF;

    _broadcastToChannel(channelName, msg, -1);
    channel->part(client);
    _replyCode = 200;
}

