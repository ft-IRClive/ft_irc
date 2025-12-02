/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claudia <claudia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:16:08 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/02 11:33:24 by claudia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

void Server::_handlerClientKick(const std::string &buffer, const int fd)
{
    Client *client = _getClient(fd);
    if (!client || !client->getIsLogged())
    {
        std::string nick = (client ? client->getNname() : "*");
        _sendResponse(fd, ERR_NOTREGISTERED(_getHostname(), nick));
        return;
    }

    std::istringstream iss(buffer);
    std::string prefix, command, channelName, targetNickname, reason;
    if (!buffer.empty() && buffer[0] == ':')
        iss >> prefix;

    iss >> command;
    iss >> channelName >> targetNickname;

    if (!targetNickname.empty() && targetNickname[0] == ':')
        targetNickname.erase(0, 1);

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
