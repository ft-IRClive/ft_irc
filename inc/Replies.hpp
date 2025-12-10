/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmaccha- <gmaccha-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 10:08:55 by claudia           #+#    #+#             */
/*   Updated: 2025/12/10 16:06:00 by gmaccha-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
#define REPLIES_HPP

#include <string>

#define CRLF "\r\n"

#define RPL_WELCOME(server, nick, user, host, realname) \
    (std::string(":") + server + " 001 " + nick + \
    " :Welcome to the IRC server! " + nick + "!" + user + "@" + host + \
    " (" + realname + ")" + CRLF)

#define RPL_UMODEIS(nick, host, channel, set, mode, arg) \
	(std::string(":") + nick + "!" + host + " MODE " + channel + " " + set + mode + " " + arg + CRLF)

#define RPL_CREATIONTIME(server, nick, channel, time) \
	(std::string(":") + server + " 329 " + nick + " " + channel + " " + time + CRLF)


#define RPL_CHANNELMODES(server, nick, channel, modes) \
	(std::string(":") + server + " 324 " + nick + " " + channel + " " + modes + CRLF)


#define RPL_CHANGEMODE(host, channel, mode, args) \
	(std::string(":") + host + " MODE " + channel + " " + mode + " " + args + CRLF)


#define RPL_NICKCHANGE(oldnick, newnick) \
	(std::string(":") + oldnick + " NICK " + newnick + CRLF)


#define RPL_JOINMSG(nick, host, channel) \
	(std::string(":") + nick + "!" + host + " JOIN " + channel + CRLF)


#define RPL_NAMREPLY(server, nick, channel, list) \
	(std::string(":") + server + " 353 " + nick + " @ " + channel + " :" + list + CRLF)


#define RPL_ENDOFNAMES(server, nick, channel) \
	(std::string(":") + server + " 366 " + nick + " " + channel + " :END of /NAMES list" CRLF)


#define RPL_TOPIC(server, nick, channel, topic) \
	(std::string(":") + server + " 332 " + nick + " " + channel + " :" + topic + CRLF)


#define RPL_NOTOPIC(server, nick, channel) \
	(std::string(":") + server + " 331 " + nick + " " + channel + " :No topic is set" CRLF)


#define RPL_QUITMESSAGE(server, nick) \
	(std::string(":") + server + " 301 " + nick + " :You have quit the server." CRLF)


#define RPL_PART(host, channel, nick) \
	(std::string(":") + host + " PART " + channel + " :" + nick + " has left the channel" CRLF)


#define RPL_KICK(host, channel, kicker, kicked, comment) \
	(std::string(":") + host + " KICK " + channel + " " + kicked + " :" + kicker + " " + comment + CRLF)


#define RPL_INVITING(host, channel, inviter, invited) \
	(std::string(":") + host + " INVITE " + invited + " " + channel + CRLF)


#define RPL_PRIVMSG(nick, host, receiver, text) \
	(std::string(":") + nick + "!" + host + " PRIVMSG " + receiver + " :" + text + CRLF)


#define ERR_NEEDMODEPARM(server, channel, mode) \
	(std::string(":") + server + " 696 " + channel + " * You must specify a parameter for the key mode. " + mode + CRLF)


#define ERR_INVALIDMODEPARM(server, channel, mode) \
	(std::string(":") + server + " 696 " + channel + " Invalid mode parameter. " + mode + CRLF)


#define ERR_KEYSET(server, channel) \
	(std::string(":") + server + " 467 " + channel + " Channel key already set." CRLF)


#define ERR_UNKNOWNMODE(server, nick, channel, mode) \
	(std::string(":") + server + " 472 " + nick + " " + channel + " " + mode + " :is not a recognised channel mode" CRLF)


#define ERR_MISSINGPARAMS(server, nick) \
	(std::string(":") + server + " 461 " + nick + " :Not enough parameters." CRLF)


#define ERR_CHANNELNOTFOUND(server, nick, channel) \
	(std::string(":") + server + " 403 " + nick + " " + channel + " :No such channel" CRLF)


#define ERR_NOTOPERATOR(server, channel) \
	(std::string(":") + server + " 482 " + channel + " :You're not a channel operator" CRLF)


#define ERR_INVALIDNICK(server, nick) \
	(std::string(":") + server + " 432 " + nick + " :Erroneous nickname" CRLF)


#define ERR_INCORPASS(server, nick) \
	(std::string(":") + server + " 464 " + nick + " :Password incorrect !" CRLF)


#define ERR_NONICKNAME(server, nick) \
	(std::string(":") + server + " 431 " + nick + " :No nickname given" CRLF)


#define ERR_NICKINUSE(server, nick) \
	(std::string(":") + server + " 433 " + nick + " :Nickname is already in use" CRLF)


#define ERR_ALREADYREGISTERED(server, nick) \
	(std::string(":") + server + " 462 " + nick + " :You may not reregister !" CRLF)


#define ERR_NOTREGISTERED(server, nick) \
	(std::string(":") + server + " 451 " + nick + " :You have not registered!" CRLF)


#define ERR_BADCHANNELKEY(server, nick, channel) \
	(std::string(":") + server + " 475 " + nick + " " + channel + " :Cannot join channel (incorrect key)" CRLF)


#define ERR_INVITEONLYCHAN(server, nick, channel) \
	(std::string(":") + server + " 473 " + nick + " " + channel + " :Cannot join channel (+i)" CRLF)


#define ERR_CHANNELISFULL(server, nick, channel) \
	(std::string(":") + server + " 471 " + nick + " " + channel + " :Cannot join channel (+l)" CRLF)


#define ERR_NOSUCHCHANNEL(server, channel) \
	(std::string(":") + server + " 403 " + channel + " :No such channel" CRLF)


#define ERR_USERNOTINCHANNEL(server, nick, channel) \
	(std::string(":") + server + " 441 " + nick + " " + channel + " :They aren't on that channel" CRLF)


#define ERR_NOTONCHANNEL(server, channel) \
	(std::string(":") + server + " 442 " + channel + " :You're not on that channel" CRLF)


#define ERR_USERONCHANNEL(server, user, channel) \
	(std::string(":") + server + " 443 " + user + " " + channel + " :is already on channel" CRLF)

#define ERR_CHANOPRIVSNEEDED(server, nick, channel) \
	(std::string(":") + server + " 482 " + nick + " " + channel + " :You're not a channel operator" CRLF)
	
#define ERR_NOPRIVILEGES(server, nick) \
	(std::string(":") + server + " 481 " + nick + " :Permission Denied- You're not an IRC operator" CRLF)


#define ERR_ERRONEUSNICK(server, nick) \
	(std::string(":") + server + " 432 " + nick + " :Erroneus nickname" CRLF)


#define ERR_BADCHANMASK(server, nick, channel) \
	(std::string(":") + server + " 476 " + nick + " " + channel + " :Bad Channel Mask" CRLF)


#define ERR_CMDNOTFOUND(server, command) \
	(std::string(":") + server + " 421 " + command + " :Unknown command" CRLF)


#define ERR_NOSUCHNICK(server, nick) \
	(std::string(":") + server + " 401 * " + nick + " :No such nick/channel" CRLF)

#define ERR_NOSUCHCHANNEL2(server, nick, channel) \
    ":" + server + " 403 " + nick + " " + channel + " :No such channel\r\n"

#endif
