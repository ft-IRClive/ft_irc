/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claudia <claudia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:22:03 by loruzqui          #+#    #+#             */
/*   Updated: 2025/11/26 10:47:15 by claudia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include "Replies.hpp"

class Channel
{
	private:

	public:
		Channel();
		Channel(std::string name);
		~Channel();

		int						getClientsSize(void) const;
		bool					getRestrictedTopic(void) const;
		std::string				getChName(void) const;
		std::string				getChKey(void) const;
		std::string				getChannelNames(void) const;
		std::string				getChTopic(void) const;
		std::vector<Client*>	getChClients(void);
		std::vector<Client*>	getOperatorClients(void);

		void	setLimit(int limit);
		void	setInvite(void);
		void	setRestrtictedTopic();
		void	setTopic(std::string topic);
		void	setKey(std::string pwsd);
		void	setChannelOperator(Client *client);

		void	removeKey(void);
		void	removeLimit(void);
		void	removeInvite(void);
		void	removeRestrictedTopic(void);
		void	removeChannelOperator(Client *client);
		void	removeChannelClient(Client *client);

		void	join(Client *client);
		void	kick(Client *client);
		void	part(Client *client);
		void	quit(Client *client);
		void	invite(Client *client);
		// void	broadcast(Client *sender, std::string target, std::string msg); -> donde esta

		bool	hasKey(void) const;
		bool	hasClient(Client *client);
		bool	isChannelComplete(void) const;
		bool	isChannelOperator(const int fd);
		bool	isChannelInviteOnly(void) const;
		bool	isChannelOperator(std::string nickname);
		bool	isClientInChannel(std::string nickname);

	private:
		int						_limit; //User limit
		bool					_hasKey; //Mode +k
		bool					_invite; //Mode +i
		bool					_restrictedTopic; //Mode +t -> permission to change topic
		std::string				_key; //Key del modo +k
		std::string				_name;
		std::string				_topic;
		std::vector<Client*>	_clients;
		std::vector<Client*>	_operator_clients; //Kick, invite
};

#endif
