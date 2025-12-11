/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 15:29:14 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/11 21:21:54 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

void Server::_handlerClientWho(const std::string &params, const int fd)
{
	Client		*client = _getClient(fd);
	Channel		*channel;
	std::string	prefix;
	std::string	reply;
	std::string	endReply;

	if (!client)
		return;
	if (params.empty())
	{
		_sendResponse(fd, ERR_MISSINGPARAMS(_hostname, client->getNname()));
		return;
	}

	//Verify if the channel exists
	channel = _getChannel(params);
	if (!channel)
	{
		_sendResponse(fd, ERR_NOSUCHCHANNEL(_hostname, params));
		return;
	}

	//Get all the clients
	const std::vector<Client*> &clients = channel->getChClients();
	for (std::vector<Client*>::const_iterator i = clients.begin(); i != clients.end(); i++)
	{
		client = *i;
		if (!client)
			continue;

		prefix = channel->isChannelOperator(client->getNname()) ? "@" : "";
		reply = ":" + _hostname + " 352 " + client->getNname() + " " +
							params + " " + client->getUname() + " " + client->getHostName() +
							" " + _hostname + " " + client->getNname() + " H" + prefix +
							" :0 " + client->getRealName() + CRLF;

		_sendResponse(fd, reply);
	}
	endReply = ":" + _hostname + " 315 " + client->getNname() + " " +
						params + " :End of WHO list" + CRLF;
	_sendResponse(fd, endReply);
}
