/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pwsd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claudia <claudia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 15:09:01 by claudia           #+#    #+#             */
/*   Updated: 2025/11/24 15:11:46 by claudia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Replies.hpp"
#include "../../inc/Server.hpp"

void Server::_handlerClientPassword(const std::string &password, const int fd)
{
    Client *client = _getClient(fd);
    if (password.empty())
    {
        _sendResponse(fd, ERR_MISSINGPARAMS("PASSWORD"));
        _replyCode = 461;
        return ;
    } 
    if (client->getIsAuthenticated())
    {
        _sendResponse(fd, ERR_ALREADYREGISTERED("*"));
        _replyCode = 462; return ;
    } 
    if (_passwd != password) 
    {
         _sendResponse(fd, ERR_INCORPASS("*"));
        _replyCode = 464;
        return ;
    } 
    client->setIsAuthenticated(true);
    _replyCode = 200;
}