/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivateMsg.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:17:56 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/17 11:46:52 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"
#include "../../inc/Replies.hpp"

void Server::_handlerClientPrivmsg(const std::string &buffer, const int fd)
{
	Client				*sender = _getClient(fd);
	std::istringstream	iss(buffer);
	std::string			target;
	std::string			text;
	Channel				*channel;
	Client				*receiver;
	std::string			msg;

	//Verify if the client is logged correctly
	if (!sender || !sender->getIsLogged())
	{
		_sendResponse(fd, ERR_NOTREGISTERED(_getHostname(), "*"));
		return;
	}

	//Extract target and text
	iss >> target;
	std::getline(iss >> std::ws, text);

	//Remove ':' if present at the beginning
	if (!text.empty() && text[0] == ':')
		text = text.substr(1);

	//Validate parameters
	if (target.empty() || text.empty())
	{
		_sendResponse(fd, ERR_SYNTAX_PRIVMSG(_getHostname(), sender->getNname()));
		return;
	}

	//Private message to a channel
	if (target[0] == '#')
	{
		channel = _getChannel(target);
		if (!channel)
		{
			_sendResponse(fd, ERR_NOSUCHCHANNEL(_getHostname(), target));
			return;
		}

		if (!channel->hasClient(sender))
		{
			_sendResponse(fd, ERR_NOTONCHANNEL(_getHostname(), target));
			return;
		}

		msg = RPL_PRIVMSG(sender->getNname(), sender->getHostName(), target, text);
		_broadcastToChannel(target, msg, fd);
	}
	//Private message to a client
	else
	{
		receiver = _getClient(target);
		if (!receiver)
		{
			_sendResponse(fd, ERR_NOSUCHNICK(_getHostname(), target));
			return;
		}

		msg = RPL_PRIVMSG(sender->getNname(), sender->getHostName(), receiver->getNname(), text);
		_sendResponse(receiver->getFd(), msg);
	}
}
