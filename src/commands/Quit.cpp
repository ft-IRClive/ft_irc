/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:18:08 by loruzqui          #+#    #+#             */
/*   Updated: 2025/11/26 17:03:41 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

void Server::_handlerClientQuit(const std::string &buffer, const int fd)
{
	Client*	client = _getClient(fd);
	Channel	*channel;

	for (std::vector<Channel*>::iterator i = _channels.begin(); i != _channels.end(); i++)
	{
		channel = *i;
		if (channel->hasClient(client))
		{
			channel->quit(client);
		}
	}

	_sendResponse(fd, RPL_QUITMESSAGE(client->getNname()));
	_replyCode = 301;

	std::cout << "Client <" << fd << "> Disconnected" << std::endl;
	_clearClient(fd);
}
