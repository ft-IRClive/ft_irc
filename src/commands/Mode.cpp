/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:16:24 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/15 16:53:26 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

bool	_processFlagsMode(const std::string& modeFlags, Channel* channel, Client* client, const std::string& arg);
bool	_applyModeFlag(Channel* channel, Client* client, char mode, bool addMode, const std::string& arg);
void	_setInviteOnlyMode(Channel* channel, bool addMode);
void	_setTopicRestrictionMode(Channel* channel, bool addMode);
void	_setChannelKeyMode(Channel* channel, const std::string& key, bool addMode);
bool	_setChannelOperatorMode(Channel* channel, Client* client, bool addMode);
void	_setChannelLimitMode(Channel* channel, const std::string& limitStr, bool addMode);

void Server::_handlerClientMode(const std::string &buffer, const int fd)
{
	std::istringstream	iss(buffer);
	std::string			channelName, modeFlags, argument;
	Client*				client;
	Channel*			channel;
	std::string			argStr;
	std::string			modeMsg;

	client = _getClient(fd);

	//Verify if the client is logged correctly
	if (!client || !client->getIsLogged())
	{
		_sendResponse(fd, ERR_NOTREGISTERED(_getHostname(), "*"));
		return;
	}

	//Extract the name of the channel, the flags and the argument
	iss >> channelName >> modeFlags >> argument;

	if (channelName.empty() || modeFlags.empty())
	{
		_sendResponse(fd, ERR_SYNTAX_MODE(_getHostname(), client->getNname()));
		return;
	}

	channel = _getChannel(channelName);
	if (!channel)
	{
		_sendResponse(fd, ERR_NOSUCHCHANNEL(_getHostname(), channelName));
		return;
	}

	//Verify that is operator
	if (!channel->isChannelOperator(client->getNname()))
	{
		_sendResponse(fd, ERR_CHANOPRIVSNEEDED(_getHostname(), client->getNname(), channelName));
		return;
	}

	if (!_processFlagsMode(modeFlags, channel, _getClient(argument), argument))
	{
		_sendResponse(fd, ERR_UNKNOWNMODE(_getHostname(), client->getNname(), channel->getChName(), modeFlags[1]));
		return;
	}

	argStr = argument.empty() ? "" : argument;
	modeMsg = RPL_CHANGEMODE(
		client->getHostName(),
		channel->getChName(),
		modeFlags,
		argStr
	);
	_broadcastToChannel(channelName, modeMsg, -1);
}

bool _processFlagsMode(const std::string& modeFlags, Channel* channel, Client* client, const std::string& arg)
{
	bool addMode = false;
	bool expectMode = false;

	for (size_t i = 0; i < modeFlags.size(); i++)
	{
		char flag = modeFlags[i];
		if (flag == '+' || flag == '-')
		{
			if (expectMode)
				return (false);
			addMode = (flag == '+');
			expectMode = true;
		}
		else
		{
			expectMode = false;
			if (!_applyModeFlag(channel, client, flag, addMode, arg))
				return (false);
		}
	}
	if (expectMode)
		return (false);
	return (true);
}

bool _applyModeFlag(Channel* channel, Client* client, char mode, bool addMode, const std::string& arg)
{
	switch (mode)
	{
		case 'i':
			_setInviteOnlyMode(channel, addMode);
			break;
		case 't':
			_setTopicRestrictionMode(channel, addMode);
			break;
		case 'k':
			_setChannelKeyMode(channel, arg, addMode);
			break;
		case 'o':
			if (!_setChannelOperatorMode(channel, client, addMode))
				return (false);
			break;
		case 'l':
			_setChannelLimitMode(channel, arg, addMode);
			break;
		default:
			return (false);
	}
	return (true);
}

void _setInviteOnlyMode(Channel* channel, bool addMode)
{
	if (addMode)
		channel->setInvite();
	else
		channel->removeInvite();
}

void _setTopicRestrictionMode(Channel* channel, bool addMode)
{
	if (addMode)
		channel->setRestrictedTopic();
	else
		channel->removeRestrictedTopic();
}

void _setChannelKeyMode(Channel* channel, const std::string& key, bool addMode)
{
	if (addMode)
		channel->setKey(key);
	else
		channel->removeKey();
}

bool _setChannelOperatorMode(Channel* channel, Client* client, bool addMode)
{
	// Verificar que el cliente existe
	if (!client)
		return (false);

	// Verificar que el cliente está en el canal
	if (!channel->hasClient(client))
		return (false);

	if (addMode)
		channel->setChannelOperator(client);
	else
		channel->removeChannelOperator(client);

	return (true);
}

void _setChannelLimitMode(Channel* channel, const std::string& limitStr, bool addMode)
{
	if (addMode)
	{
		if (!limitStr.empty())
			channel->setLimit(std::atoi(limitStr.c_str()));
	}
	else
		channel->removeLimit();
}
