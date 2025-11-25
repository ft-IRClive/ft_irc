/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claudia <claudia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:11:42 by loruzqui          #+#    #+#             */
/*   Updated: 2025/11/25 13:27:08 by claudia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

Channel::Channel()
{
    _limit = -1; // no limit
    _hasKey = false;
    _invite = false;
    _restrictedTopic = false;
    _key = "";
    _name = "";
    _topic = "";
    //_createdAt = "";
    _clients = std::vector<Client*>();
    _operator_clients = std::vector<Client*>();
}

Channel::Channel(std::string name)
{
    _limit = -1; // no limit
    _hasKey = false;
    _invite = false;
    _restrictedTopic = false;
    _key = "";
    _name = name;
    _topic = "";
    //_createdAt = "";
    _clients = std::vector<Client*>();
    _operator_clients = std::vector<Client*>();
}

Channel::~Channel()
{

    _clients.clear();
    _operator_clients.clear();
}

int Channel::getClientsSize(void) const
{
    return (this->_clients.size());
}

bool Channel::getRestrictedTopic(void) const
{
    return (_restrictedTopic);
}

std::string Channel::getChName(void) const
{
    return (_name);
}

std::string Channel::getChKey(void) const
{
    return (_key);
}

std::string Channel::getChannelNames(void) const
{
    std::string names;
    for(std::vector<Client*>::const_iterator it = this->_clients.begin();
        it != this->_clients.end(); ++it)
        {
            names += (*it)->getNname();
            names += " ";
        }
        return (names);
}

std::string Channel::getChTopic(void) const
{
    return (_topic);
}

std::vector<Client*> Channel::getChClients(void)
{
    return (this->_clients);
}

std::vector<Client*> Channel::getOperatorClients(void)
{
    return (this->_operator_clients);
}

// setters

void Channel::setLimit(int limit)
{
    _limit = limit;
}

void Channel::setInvite()
{
    _invite = true;
}

void Channel::setKey(std::string key)
{
    _key = key;
}

void Channel::removeLimit(void)
{
    _limit = -1;
}

void Channel::removeKey(void)
{
    _key = "";
}
void Channel::removeInvite(void)
{
    _invite = false;
}

void Channel::removeChannelOperator(Client *client)
{
    client->setOp(false);
    for(std::vector<Client*>::iterator it = this->_operator_clients.begin();
        it != this->_operator_clients.end(); ++it)
    {
        if ((*it)->getNname() == client->getNname())
        {
            this->_operator_clients.erase(it);
            return ;
        }
    }
}

bool Channel::hasClient(Client *client)
{
    for (std::vector<Client*>::const_iterator it = this->_clients.begin();
        it != this->_clients.end(); ++it)
        {
            if ((*it)->getNname() == client->getNname())
                return (true);
        }
        return (false);
}

bool Channel::hasKey(void) const
{
    return (_key != "");
}

bool Channel::isChannelOperator(std::string nickname)
{
    for(std::vector<Client*>::iterator it = this->_operator_clients.begin();
    it != this->_operator_clients.end(); ++it)
    {
        if ((*it)->getNname() == nickname)
            return (true);
    }
    return (false);
}

bool Channel::isChannelOperator(const int fd)
{
    for(std::vector<Client*>::iterator it = this->_operator_clients.begin();
    it != this->_operator_clients.end(); ++it)
    {
        if ((*it)->getFd() == fd)
            return (true);
    }
    return (false);
}

void Channel::invite(Client *client)
{
    _clients.push_back(client);
    return ;
}

void Channel::join(Client *client)
{
    _clients.push_back(client);
    return ;
}

/*void Channel::kick(Client *client)
{
  
}*/

void Channel::part(Client *client)
{
    removeChannelOperator(client);
    removeChannelClient(client);
}

void Channel::quit(Client *client)
{
    removeChannelOperator(client);
}

/*void Channel::broadcast(Client *sender, std::string target, std::string msg)
{
    for (std::vector<Client *>::iterator it = this->_clients.begin();
        it != this->_clients.end(); ++it)
        {
            if (*it == sender)
                continue;
            (*it)->broadcast(sender, target, msg);
        }
}*/

bool Channel::isClientInChannel(std::string nickname)
{
    for (std::vector<Client*>::iterator it = this->_clients.begin();
        it != this->_clients.end(); ++it)
        {
            if ((*it)->getNname() == nickname)
                return (true);
        }
        return (false);
}

bool Channel::isChannelComplete(void) const
{
    if (_limit == -1)
        return (false);
    if (_clients.size() >= static_cast<size_t>(_limit))
        return true;
    return (false);
}

bool Channel::isChannelComplete(void) const
{
    return (_invite);
}