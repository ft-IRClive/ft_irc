/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 15:29:14 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/12 17:02:48 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/Replies.hpp"

void Server::_handlerClientWho(const std::string &params, const int fd)
{
	Client		*requester = _getClient(fd);
	Channel		*channel;
	std::string	reply;

	if (!requester)
		return;
	if (params.empty())
	{
		_sendResponse(fd, ERR_MISSINGPARAMS(_hostname, requester->getNname()));
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
	for (std::vector<Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it)
	{
		Client *c = *it;
		if (!c)
			continue;

		std::string	prefix = channel->isChannelOperator(c->getNname()) ? "@" : "";
		std::string	status = "H" + prefix;

		reply = RPL_WHOREPLY(
			_hostname,
			requester->getNname(),
			params,
			c->getUname(),
			c->getHostName(),
			_hostname,
			c->getNname(),
			status,
			c->getRealName()
		);

		_sendResponse(fd, reply);
	}
	_sendResponse(fd, RPL_ENDOFWHO(_hostname, requester->getNname(), params));
}
