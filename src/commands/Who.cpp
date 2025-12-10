/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 15:29:14 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/10 17:53:21 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

void	Server::_handlerClientWho(const std::string &buffer, const int fd)
{
	Client	*client = _getClient(fd);
	if (!client)
		return;

	std::vector<std::string>	tokens = _splitBuffer(buffer, " ");
	if (tokens.size() < 1)
	{
		_sendResponse(fd, ERR_MISSINGPARAMS(_hostname, client->getNname()));
		return;
	}

	std::string	channelName = tokens[0];
	Channel		*channel = _getChannel(channelName);

	if (!channel)
	{
		_sendResponse(fd, ERR_NOSUCHCHANNEL2(_hostname, client->getNname(), channelName));
		return;
	}
 	for (size_t i = 0; i < channel->getChClients().size(); i++)
	{
		Client		*c = channel->getChClients()[i];
		std::string	prefix = channel->isChannelOperator(c->getNname()) ? "@" : "";
		std::string	reply =
			":" + _hostname + " 352 " + client->getNname() + " " +
			channelName + " " + c->getUname() + " " + c->getHostName() +
			" " + _hostname + " " + c->getNname() + " H" + prefix + " :0 " +
			c->getRealName() + CRLF;
		_sendResponse(fd, reply);
	}

	std::string	endReply =
		":" + _hostname + " 315 " + client->getNname() +
		" " + channelName + " :End of WHO list" + CRLF;
	_sendResponse(fd, endReply);
}
