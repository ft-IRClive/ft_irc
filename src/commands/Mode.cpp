/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:16:24 by loruzqui          #+#    #+#             */
/*   Updated: 2025/11/29 16:47:56 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

bool	_processFlagsMode(const std::string& modeFlags, Channel* channel, Client* client, const std::string& arg);
bool	_applyModeFlag(Channel* channel, Client* client, char mode, bool addMode, const std::string& arg);
void	_setInviteOnlyMode(Channel* channel, bool addMode);
void	_setTopicRestrictionMode(Channel* channel, bool addMode);
void	_setChannelKeyMode(Channel* channel, const std::string& key, bool addMode);
void	_setChannelOperatorMode(Channel* channel, Client* client, bool addMode);
void	_setChannelLimitMode(Channel* channel, const std::string& limitStr, bool addMode);

void Server::_handlerClientMode(const std::string &buffer, const int fd)
{
	std::istringstream	iss(buffer);
	std::string			channelName, modeFlags, argument;

	iss >> channelName >> modeFlags;
	iss >> argument;

	Client*		client = _getClient(fd);
	Channel*	channel = _getChannel(channelName);

	if (modeFlags.empty())
	{
		_replyCode = 461;
		return;
	}

	if (channelName.empty() || modeFlags.empty())
	{
		_sendResponse(fd, ERR_MISSINGPARAMS(_getHostname(), client->getNname()));
		_replyCode = 461;
	}
	else if (!channel)
	{
		_sendResponse(fd, ERR_NOSUCHCHANNEL(_getHostname(), channelName));
		_replyCode = 403;
	}
	else if (!channel->isChannelOperator(client->getNname()))
	{
		_sendResponse(fd, ERR_CHANOPRIVSNEEDED(_getHostname(), channelName));
		_replyCode = 482;
	}
	else if (!_processFlagsMode( modeFlags, channel, _getClient(argument), argument))
	{
		_sendResponse(fd, ERR_UNKNOWNMODE(_getHostname(), client->getNname(), channel->getChName(), modeFlags[1]));
		_replyCode = 472;
	}
	else
	{
		std::string argStr = argument.empty() ? "" : argument;
		std::string modeMsg = RPL_CHANGEMODE(
			client->getHostName(),
			channel->getChName(),
			modeFlags,
			argStr
		);

		_broadcastToChannel(channelName, modeMsg, -1);
		_replyCode = 200;
	}
}

bool _processFlagsMode(const std::string& modeFlags, Channel* channel, Client* client, const std::string& arg)
{
	bool	addMode = false;
	char	mode = 0;
	char	flag;

	for (size_t i = 0; i < modeFlags.size(); ++i)
	{
		flag = modeFlags[i];
		if (flag == '+' || flag == '-')
			addMode = (flag == '+');
		else
		{
			mode = flag;
			if (!_applyModeFlag(channel, client, mode, addMode, arg))
				return (false);
		}
	}
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
			_setChannelOperatorMode(channel, client, addMode);
			break;
		case 'l':
			_setChannelLimitMode(channel, arg, addMode);
			break;
		default:
			return (false);
	}
	return(true);
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

void _setChannelOperatorMode(Channel* channel, Client* client, bool addMode)
{
	if (addMode && client)
		channel->setChannelOperator(client);
	else if (client)
		channel->removeChannelOperator(client);
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
