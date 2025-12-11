/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivateMsg.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claudia <claudia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:17:56 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/11 15:06:34 by claudia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

void Server::_handlerClientPrivmsg(const std::string &buffer, const int fd)
{
    Client *sender = _getClient(fd);
    if (!sender)
        return;
    std::cout << "[DEBUG] Mensaje recibido de " << sender->getNname() << ": " << buffer << std::endl;
    size_t space_pos = buffer.find(' ');
    if (space_pos == std::string::npos)
    {
        _sendResponse(fd, ERR_MISSINGPARAMS(_hostname, sender->getNname()));
        return;
    }
    std::string target = buffer.substr(0, space_pos);
    size_t text_pos = buffer.find(":", space_pos);
    if (text_pos == std::string::npos)
    {
        _sendResponse(fd, ERR_MISSINGPARAMS(_hostname, sender->getNname()));
        return;
    }
    std::string text = buffer.substr(text_pos + 1);

    std::cout << "[DEBUG] Target=" << target << ", Text=" << text << std::endl;
    if (!target.empty() && target[0] == '#')
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
        std::cout << "[DEBUG] Enviando mensaje al canal " << target << ": " << msg;
        _broadcastToChannel(target, msg, fd);
    }
    else
    {
        Client *receiver = _getClient(target);
        if (!receiver)
        {
            _sendResponse(fd, ERR_NOSUCHNICK(_hostname, target));
            return;
        }
        std::string msg = RPL_PRIVMSG(sender->getNname(), sender->getHostName(), receiver->getNname(), text);
        std::cout << "[DEBUG] Enviando mensaje a usuario " << target << ": " << msg;
        _sendResponse(receiver->getFd(), msg);
    }
}



