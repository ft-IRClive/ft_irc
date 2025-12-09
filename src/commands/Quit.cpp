/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:18:08 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/09 17:22:15 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

void Server::_handlerClientQuit(const std::string &params, const int fd)
{
	Client*		client = _getClient(fd);
	std::string	reason;

	if (!client)
		return;
	if (!params.empty())
	{
		if (params[0] == ':')
			reason = params.substr(1);
		else
			reason = params;
	}
	if (reason.empty())
		reason = "Client Quit";

	std::string quitMsg = ":" + client->getNname() + " QUIT :" + reason;

	for (size_t i = 0; i < _channels.size(); ++i)
	{
		Channel* ch = _channels[i];
		if (!ch)
			continue;
		if (ch->hasClient(client))
		{
			_broadcastToAll(quitMsg, fd);
			ch->quit(client);
		}
	}

	std::cout << "Client <" << fd << "> Disconnected (" << reason << ")" << std::endl;
	_clearClient(fd);
}
