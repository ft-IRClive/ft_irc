/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:16:24 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/19 10:08:46 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

bool	_processFlagsMode(const std::string& modeFlags, Channel* channel, Client* client, const std::string& arg);
bool	_applyModeFlag(Channel* channel, Client* client, char mode, bool addMode, const std::string& arg);
void	_setInviteOnlyMode(Channel* channel, bool addMode);
void	_setTopicRestrictionMode(Channel* channel, bool addMode);
void	_setChannelKeyMode(Channel* channel, const std::string& key, bool addMode);
bool	_setChannelOperatorMode(Channel* channel, Client* client, bool addMode);
bool	_setChannelLimitMode(Channel* channel, const std::string& limitStr, bool addMode);

/**
 * @brief Handles the MODE command for a channel.
 *
 * Displays or updates channel modes. Validates client state,
 * channel existence and operator privileges, then applies
 * the requested mode flags and broadcasts the change.
 *
 * @param buffer Command parameters (channel, modes and argument).
 * @param fd File descriptor of the requesting client.
 */
void Server::_handlerClientMode(const std::string &buffer, const int fd)
{
	Client				*client = _getClient(fd);
	std::istringstream	iss(buffer);
	std::string			channelName;
	std::string			modeFlags;
	std::string			argument;
	Channel				*channel;

	// Check if the client is correctly logged
	if (!client || !client->getIsLogged())
	{
		_sendResponse(fd, ERR_NOTREGISTERED(_getHostname(), "*"));
		return;
	}

	iss >> channelName >> modeFlags >> argument;

	if (channelName.empty())
	{
		_sendResponse(fd, ERR_SYNTAX_MODE(_getHostname(), client->getNname()));
		return;
	}

	if (channelName[0] != '#')
	{
		_sendResponse(fd, ERR_SYNTAX_MODE(_getHostname(), client->getNname()));
		return;
	}

	// Verify if the channel exists
	channel = _getChannel(channelName);
	if (!channel)
	{
		_sendResponse(fd, ERR_NOSUCHCHANNEL(_getHostname(), channelName));
		return;
	}

	if (modeFlags.empty())
	{
		_sendResponse(fd,
			":" + _getHostname() + " 324 " +
			client->getNname() + " " +
			channelName + " " +
			channel->getModes() + "\r\n"
		);
		return;
	}

	if (!channel->isChannelOperator(client->getNname()))
	{
		_sendResponse(fd,
			ERR_CHANOPRIVSNEEDED(
				_getHostname(),
				client->getNname(),
				channelName
			)
		);
		return;
	}

	if (modeFlags.find('o') != std::string::npos)
	{
		if (argument.empty())
		{
			_sendResponse(fd, ERR_SYNTAX_MODE(_getHostname(), client->getNname()));
			return;
		}

		Client* target = _getClient(argument);
		if (!target)
		{
			_sendResponse(fd, ERR_NOSUCHNICK(_getHostname(), argument));
			return;
		}

		if (!channel->hasClient(target))
		{
			_sendResponse(fd,
				ERR_USERNOTINCHANNEL(
					_getHostname(),
					argument,
					channelName
				)
			);
			return;
		}
	}

	if (!_processFlagsMode(modeFlags, channel, _getClient(argument), argument))
	{
		_sendResponse(fd, ERR_UNKNOWNMODE(_getHostname(), client->getNname(), channelName, modeFlags[1]));
		return;
	}

	std::string argStr = argument.empty() ? "" : argument;
	std::string modeMsg = RPL_CHANGEMODE(
		client->getHostName(),
		channel->getChName(),
		modeFlags,
		argStr
	);

	_broadcastToChannel(channelName, modeMsg, -1);
}

bool _processFlagsMode(const std::string& modeFlags, Channel* channel, Client* client, const std::string& arg)
{
	bool	addMode = false;
	bool	expectMode = false;

	for (size_t i = 0; i < modeFlags.size(); i++)
	{
		char	flag = modeFlags[i];
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
			if (!_setChannelLimitMode(channel, arg, addMode))
				return (false);
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

bool _isValidNumber(const std::string& str)
{
	if (str.empty())
		return (false);

	for (size_t i = 0; i < str.length(); i++)
	{
		if (!std::isdigit(str[i]))
			return (false);
	}
	return (true);
}

bool _setChannelLimitMode(Channel* channel, const std::string& limitStr, bool addMode)
{
	if (addMode)
	{
		if (limitStr.empty())
			return (false);

		if (!_isValidNumber(limitStr))
			return (false);

		int	limit = std::atoi(limitStr.c_str());

		if (limit <= 0)
			return (false);

		channel->setLimit(limit);
	}
	else
		channel->removeLimit();

	return (true);
}
