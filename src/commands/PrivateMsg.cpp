/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivateMsg.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmaccha- <gmaccha-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:17:56 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/10 16:05:40 by gmaccha-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

void Server::_handlerClientPrivmsg(const std::string &buffer, const int fd)
{
	Client *sender = _getClient(fd);
    if (!sender) return;

    std::vector<std::string> tokens = _splitBuffer(buffer, " ");

    if (tokens.size() < 3)
    {
        _sendResponse(fd, ERR_MISSINGPARAMS(_hostname, sender->getNname()));
        return;
    }

    std::string target = tokens[1];
    std::string text = buffer.substr(buffer.find(":", 1) + 1);

    // Mensaje a canal
    if (target[0] == '#')
    {
        Channel *channel = _getChannel(target);
        if (!channel)
        {
            _sendResponse(fd, ERR_NOSUCHCHANNEL(_hostname, target));
            return;
        }

        if (!channel->isClientInChannel(sender->getNname()))
        {
            _sendResponse(fd, ERR_NOTONCHANNEL(_hostname, target));
            return;
        }

        std::string msg = RPL_PRIVMSG(sender->getNname(), sender->getHostName(), target, text);
        _broadcastToChannel(target, msg, fd);
    }
    else
    {
        // Mensaje a usuario
        Client *receiver = _getClient(target);
        if (!receiver)
        {
            _sendResponse(fd, ERR_NOSUCHNICK(_hostname, target));
            return;
        }

        std::string msg = RPL_PRIVMSG(sender->getNname(), sender->getHostName(), receiver->getNname(), text);
        _sendResponse(receiver->getFd(), msg);
    }
}
