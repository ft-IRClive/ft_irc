/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:18:08 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/19 09:49:27 by loruzqui         ###   ########.fr       */
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

	if (!client)
		return;

	//Obtain reason
	if (!params.empty())
	{
		if (params[0] == ':')
			reason = params.substr(1);
		else
			reason = params;
	}
	if (reason.empty())
		reason = "Client Quit";

	quitMsg = ":" + client->getNname() + "!" +
		client->getUname() + "@" +
		client->getHostName() +
		" QUIT :" + reason + CRLF;

	for (size_t i = 0; i < _channels.size(); )
	{
		Channel*	ch = _channels[i];
		if (!ch)
		{
			++i;
			continue;
		}

		if (ch->hasClient(client))
		{
			//Broadcast quit message
			_broadcastToChannel(ch->getChName(), quitMsg, fd);

			//Check if this client is an operator and count total operators BEFORE removing
			bool	wasOperator = ch->isChannelOperator(client->getNname());

			//Count how many operators are in the channel
			int							operatorCount = 0;
			const std::vector<Client*>&	allClients = ch->getChClients();
			for (size_t j = 0; j < allClients.size(); j++)
			{
				if (allClients[j] && ch->isChannelOperator(allClients[j]->getNname()))
					operatorCount++;
			}

			//If this is the last operator, find a replacement BEFORE removing them
			Client*	newOp = NULL;
			if (wasOperator && operatorCount == 1)
			{
				//Get the oldest client that is NOT the one leaving
				for (size_t j = 0; j < allClients.size(); j++)
				{
					if (allClients[j] && allClients[j] != client)
					{
						newOp = allClients[j];
						break;
					}
				}
			}

			//Remove the client from the channel
			ch->quit(client);

			//Promote new operator if needed
			if (newOp)
			{
				ch->setChannelOperator(newOp);
				std::string modeMsg =
					":" + _getHostname() +
					" MODE " + ch->getChName() +
					" +o " + newOp->getNname() + CRLF;
				_broadcastToChannel(ch->getChName(), modeMsg, -1);
			}
		}
		++i;
	}

	std::cout << "Client <" << fd << "> Disconnected (" << reason << ")" << std::endl;
	_clearClient(fd);
}
