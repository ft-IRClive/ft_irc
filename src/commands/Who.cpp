/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claudia <claudia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 15:29:14 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/11 11:59:20 by claudia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

void Server::_handlerClientWho(const std::string &params, const int fd)
{
    Client *client = _getClient(fd);
    if (!client)
        return;
    if (params.empty())
    {
        _sendResponse(fd, ERR_MISSINGPARAMS(_hostname, client->getNname()));
        return;
    }
    Channel *channel = _getChannel(params);
    if (!channel)
    {
        _sendResponse(fd, ERR_NOSUCHCHANNEL2(_hostname, client->getNname(), params));
        return;
    }
    const std::vector<Client*> &clients = channel->getChClients();
    for (std::vector<Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it)
    {
        Client *c = *it;
        if (!c) 
            continue;

        std::string prefix = channel->isChannelOperator(c->getNname()) ? "@" : "";
        std::string reply = ":" + _hostname + " 352 " + client->getNname() + " " +
                            params + " " + c->getUname() + " " + c->getHostName() +
                            " " + _hostname + " " + c->getNname() + " H" + prefix +
                            " :0 " + c->getRealName() + CRLF;

        _sendResponse(fd, reply);
    }
    std::string endReply = ":" + _hostname + " 315 " + client->getNname() + " " +
                           params + " :End of WHO list" + CRLF;
    _sendResponse(fd, endReply);
}