/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmaccha- <gmaccha-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:18:08 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/18 01:37:46 by gmaccha-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

/**
 * @brief Handles the QUIT command.
 *
 * Disconnects a client from the server, removes it from all joined
 * channels and broadcasts the quit message with an optional reason.
 *
 * @param params Optional quit message.
 * @param fd File descriptor of the quitting client.
 */

void Server::_handlerClientQuit(const std::string &params, const int fd)
{
	Client*		client = _getClient(fd);
	std::string	reason;
	std::string	quitMsg;
	Channel*	ch;

	if (!client)
		return;

	//Quit with a reason
	if (!params.empty())
	{
		if (params[0] == ':')
			reason = params.substr(1);
		else
			reason = params;
	}
	//Quit without a reason
	if (reason.empty())
		reason = "Client Quit";

	quitMsg = ":" + client->getNname() + " QUIT :" + reason;
	for (size_t i = 0; i < _channels.size(); i++)
	{
		ch = _channels[i];
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
