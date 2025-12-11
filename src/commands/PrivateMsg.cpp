/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivateMsg.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:17:56 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/11 20:32:19 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

void Server::_handlerClientPrivmsg(const std::string &buffer, const int fd)
{
	Client		*sender = _getClient(fd);
	size_t		spacePos;
	std::string	target;
	size_t		textPos;
	std::string	text;
	Channel		*channel;
	std::string	msg;
	Client		*receiver;

	if (!sender)
		return;
	spacePos = buffer.find(' ');
	if (spacePos == std::string::npos)
	{
		_sendResponse(fd, ERR_MISSINGPARAMS(_hostname, sender->getNname()));
		return;
	}
	target = buffer.substr(0, spacePos);
	textPos = buffer.find(":", spacePos);
	if (textPos == std::string::npos)
	{
		_sendResponse(fd, ERR_MISSINGPARAMS(_hostname, sender->getNname()));
		return;
	}
	text = buffer.substr(textPos + 1);
	if (!target.empty() && target[0] == '#')
	{
		channel = _getChannel(target);
		if (!channel)
		{
			_sendResponse(fd, ERR_NOSUCHCHANNEL(_hostname, target));
			return;
		}
		if (!channel->isClientInChannel(sender->getNname()))
		{
			_sendResponse(fd, ERR_NOTONCHANNEL(_hostname, target));
			return;
		}
		msg = RPL_PRIVMSG(sender->getNname(), sender->getHostName(), target, text);
		_broadcastToChannel(target, msg, fd);
	}
	else
	{
		receiver = _getClient(target);
		if (!receiver)
		{
			_sendResponse(fd, ERR_NOSUCHNICK(_hostname, target));
			return;
		}
		msg = RPL_PRIVMSG(sender->getNname(), sender->getHostName(), receiver->getNname(), text);
		_sendResponse(receiver->getFd(), msg);
	}
}
