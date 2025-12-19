/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 15:29:14 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/19 09:53:14 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/Replies.hpp"

/**
 * @brief Handles the WHO command for a specific channel.
 *
 * Verifies client state and channel existence, then sends WHO replies
 * for each client in the channel.
 *
 * @param buffer Command parameters containing the channel name.
 * @param fd File descriptor of the requesting client.
 */
void Server::_handlerClientWho(const std::string &buffer, const int fd)
{
	Client				*requester = _getClient(fd);
	std::istringstream	iss(buffer);
	std::string			channelName;
	Channel				*channel;

	//Verify if the client is logged correctly
	if (!requester || !requester->getIsLogged())
	{
		_sendResponse(fd, ERR_NOTREGISTERED(_getHostname(), "*"));
		return;
	}

	//Extract channel name
	iss >> channelName;

	if (channelName.empty())
	{
		_sendResponse(fd, ERR_SYNTAX_WHO(_getHostname(), requester->getNname()));
		return;
	}

	//Verify if the channel exists
	channel = _getChannel(channelName);
	if (!channel)
	{
		_sendResponse(fd, ERR_NOSUCHCHANNEL(_getHostname(), channelName));
		return;
	}

	//Get all the clients in the channel
	const std::vector<Client*> &clients = channel->getChClients();
	for (std::vector<Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it)
	{
		Client *c = *it;
		if (!c)
			continue;

		std::string prefix = channel->isChannelOperator(c->getNname()) ? "@" : "";
		std::string status = "H" + prefix;

		std::string reply = RPL_WHOREPLY(
			_getHostname(),
			requester->getNname(),
			channelName,
			c->getUname(),
			c->getHostName(),
			_getHostname(),
			c->getNname(),
			status,
			c->getRealName()
		);
		_sendResponse(fd, reply);
	}

	_sendResponse(fd, RPL_ENDOFWHO(_getHostname(), requester->getNname(), channelName));
}
