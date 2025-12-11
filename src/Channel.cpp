/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:11:42 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/11 20:44:30 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

Channel::Channel()
{
	this->_limit = -1;
	this->_hasKey = false;
	this->_invite = false;
	this->_restrictedTopic = false;
	this->_key = "";
	this->_name = "";
	this->_topic = "";
	this->_clients = std::vector<Client*>();
	this->_operatorClients = std::vector<Client*>();
}

Channel::Channel(std::string name)
{
	this->_limit = -1;
	this->_hasKey = false;
	this->_invite = false;
	this->_restrictedTopic = false;
	this->_key = "";
	this->_name = name;
	this->_topic = "";
	this->_clients = std::vector<Client*>();
	this->_operatorClients = std::vector<Client*>();
}

Channel::~Channel()
{
	this->_clients.clear();
	this->_operatorClients.clear();
}

int Channel::getClientsSize(void) const
{
	return (this->_clients.size());
}

bool Channel::getRestrictedTopic(void) const
{
	return (this->_restrictedTopic);
}

std::string Channel::getChName(void) const
{
	return (this->_name);
}

std::string Channel::getChKey(void) const
{
	return (this->_key);
}

std::string Channel::getChannelNames(void) const
{
	std::string	names;
	bool		first = true;

	for (std::vector<Client*>::const_iterator i = this->_clients.begin(); i != this->_clients.end(); i++)
	{
		if (!first)
			names += " ";
		first = false;

		if (isChannelOperator((*i)->getFd()))
			names += "@";

		names += (*i)->getNname();
	}

	return (names);
}

std::string Channel::getChTopic(void) const
{
	return (this->_topic);
}

std::vector<Client*> Channel::getChClients(void)
{
	return (this->_clients);
}

std::vector<Client*> Channel::getOperatorClients(void)
{
	return (this->_operatorClients);
}

//----------- SETTERS

void Channel::setLimit(int limit)
{
	this->_limit = limit;
}

void Channel::setInvite()
{
	this->_invite = true;
}

void Channel::setRestrictedTopic()
{
	this->_restrictedTopic = true;
}

void Channel::setKey(std::string key)
{
	this->_key = key;
}

void Channel::setChannelOperator(Client* client)
{
	client->setOp(true);
	this->_operatorClients.push_back(client);
	return;
}

void Channel::removeLimit(void)
{
	this->_limit = -1;
}

void Channel::removeKey(void)
{
	this->_key = "";
}

void Channel::removeInvite(void)
{
	this->_invite = false;
}

void Channel::removeRestrictedTopic(void)
{
	this->_restrictedTopic = false;
}

void Channel::removeChannelOperator(Client *client)
{
	client->setOp(false);
	for(std::vector<Client*>::iterator i = this->_operatorClients.begin(); i != this->_operatorClients.end(); i++)
	{
		if ((*i)->getNname() == client->getNname())
		{
			this->_operatorClients.erase(i);
			return ;
		}
	}
}

void Channel::removeChannelClient(Client *client)
{
	int	fd;

	if (!client)
		return;
	fd = client->getFd();
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i]->getFd() == fd)
		{
			this->_clients.erase(this->_clients.begin() + i);
			break;
		}
	}
}

bool Channel::hasClient(Client *client)
{
	for (std::vector<Client*>::const_iterator i = this->_clients.begin(); i != this->_clients.end(); i++)
	{
		if ((*i)->getNname() == client->getNname())
			return (true);
	}
	return (false);
}

bool Channel::hasKey(void) const
{
	return (this->_key != "");
}

bool Channel::isChannelOperator(std::string nickname) const
{
	for(std::vector<Client*>::const_iterator i = this->_operatorClients.begin(); i != this->_operatorClients.end(); i++)
	{
		if ((*i)->getNname() == nickname)
			return (true);
	}
	return (false);
}

bool Channel::isChannelOperator(const int fd) const
{
	for(std::vector<Client*>::const_iterator i = this->_operatorClients.begin(); i != this->_operatorClients.end(); i++)
	{
		if ((*i)->getFd() == fd)
			return (true);
	}
	return (false);
}

void Channel::invite(Client *client)
{
	this->_clients.push_back(client);
	return ;
}

void Channel::join(Client *client)
{
	this->_clients.push_back(client);
	return ;
}

void Channel::kick(Client* client)
{
	client->removeChannelInvited(this->getChName());
	removeChannelOperator(client);
	removeChannelClient(client);
}

void Channel::part(Client *client)
{
	removeChannelOperator(client);
	removeChannelClient(client);
}

void Channel::quit(Client *client)
{
	removeChannelOperator(client);
}

bool Channel::isClientInChannel(std::string nickname)
{
	for (std::vector<Client*>::iterator i = this->_clients.begin(); i != this->_clients.end(); i++)
	{
		if ((*i)->getNname() == nickname)
			return (true);
	}
	return (false);
}

bool Channel::isChannelComplete(void) const
{
	if (this->_limit == -1)
		return (false);
	if (this->_clients.size() >= static_cast<size_t>(this->_limit))
		return true;
	return (false);
}

bool Channel::isChannelInviteOnly(void) const
{
	return (this->_invite);
}

void Channel::setTopic(std::string topic)
{
	this->_topic = topic;
}
