/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claudia <claudia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 10:08:55 by claudia           #+#    #+#             */
/*   Updated: 2025/11/24 11:27:31 by claudia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
#define REPLIES_HPP

#include <string>

#define CRLF "\r\n"

#define RPL_WELCOME(server, nick, host) \
    (std::string(":") + server + " 001 " + nick + \
    " :Welcome to the IRC server! " + nick + "!" + host + CRLF)

#define RPL_UMODEIS(nick, host, channel, set, mode, arg) \
    (std::string(":") + nick + "!" + host + " MODE " + channel + " " + set + mode + " " + arg + CRLF)

#define RPL_CREATIONTIME(nick, channel, time) \
    (std::string(":329 ") + nick + " " + channel + " " + time + CRLF)

#define RPL_CHANNELMODES(nick, channel, modes) \
    (std::string(":324 ") + nick + " " + channel + " " + modes + CRLF)

#define RPL_CHANGEMODE(host, channel, mode, args) \
    (std::string(":") + host + " MODE " + channel + " " + mode + " " + args + CRLF)

#define RPL_NICKCHANGE(oldnick, newnick) \
    (std::string(":") + oldnick + " NICK " + newnick + CRLF)

#define RPL_JOINMSG(nick, host, channel) \
    (std::string(":") + nick + "!" + host + " JOIN " + channel + CRLF)

#define RPL_NAMREPLY(nick, channel, list) \
    (std::string(":353 ") + nick + " @ " + channel + " :" + list + CRLF)

#define RPL_ENDOFNAMES(nick, channel) \
    (std::string(":366 ") + nick + " " + channel + " :END of /NAMES list" CRLF)

#define RPL_TOPIC(nick, channel, topic) \
    (std::string(":332 ") + nick + " " + channel + " :" + topic + CRLF)

#define RPL_NOTOPIC(nick, channel) \
    (std::string(":331 ") + nick + " " + channel + " :No topic is set" CRLF)

#define RPL_QUITMESSAGE(nick) \
    (std::string(":301 ") + nick + " :You have quit the server." CRLF)

#define RPL_PART(host, channel, nick) \
    (std::string(":") + host + " PART " + channel + " :" + nick + " has left the channel" CRLF)

#define RPL_KICK(host, channel, kicker, kicked, comment) \
    (std::string(":") + host + " KICK " + channel + " " + kicked + " :" + kicker + " " + comment + CRLF)

#define RPL_INVITING(host, channel, inviter, invited) \
    (std::string(":") + host + " INVITE " + invited + " " + channel + CRLF)

#define RPL_PRIVMSG(nick, host, receiver, text) \
    (std::string(":") + nick + "!" + host + " PRIVMSG " + receiver + " :" + text + CRLF)

#define ERR_NEEDMODEPARM(channel, mode) \
    (std::string(":696 ") + channel + " * You must specify a parameter for the key mode. " + mode + CRLF)

#define ERR_INVALIDMODEPARM(channel, mode) \
    (std::string(":696 ") + channel + " Invalid mode parameter. " + mode + CRLF)

#define ERR_KEYSET(channel) \
    (std::string(":467 ") + channel + " Channel key already set." CRLF)

#define ERR_UNKNOWNMODE(nick, channel, mode) \
    (std::string(":472 ") + nick + " " + channel + " " + mode + " :is not a recognised channel mode" CRLF)

#define ERR_MISSINGPARAMS(nick) \
    (std::string(":461 ") + nick + " :Not enough parameters." CRLF)

#define ERR_CHANNELNOTFOUND(nick, channel) \
    (std::string(":403 ") + nick + " " + channel + " :No such channel" CRLF)

#define ERR_NOTOPERATOR(channel) \
    (std::string(":482 ") + channel + " :You're not a channel operator" CRLF)

	#define ERR_INVALIDNICK(nick) \
    (std::string(":432 ") + nick + " :Erroneous nickname" CRLF)

#define ERR_INCORPASS(nick) \
    (std::string(":464 ") + nick + " :Password incorrect !" CRLF)

#define ERR_NONICKNAME(nick) \
    (std::string(":431 ") + nick + " :No nickname given" CRLF)

#define ERR_NICKINUSE(nick) \
    (std::string(":433 ") + nick + " :Nickname is already in use" CRLF)

#define ERR_ALREADYREGISTERED(nick) \
    (std::string(":462 ") + nick + " :You may not reregister !" CRLF)

#define ERR_NOTREGISTERED(nick) \
    (std::string(":451 ") + nick + " :You have not registered!" CRLF)

#define ERR_BADCHANNELKEY(nick, channel) \
    (std::string(":475 ") + nick + " " + channel + " :Cannot join channel (incorrect key)" CRLF)

#define ERR_INVITEONLYCHAN(nick, channel) \
    (std::string(":473 ") + nick + " " + channel + " :Cannot join channel (+i)" CRLF)

#define ERR_CHANNELISFULL(nick, channel) \
    (std::string(":422 ") + nick + " " + channel + " :Cannot join channel (+l)" CRLF)

#define ERR_NOSUCHCHANNEL(channel) \
    (std::string(":403 ") + channel + " :No such channel" CRLF)

#define ERR_USERNOTINCHANNEL(nick, channel) \
    (std::string(":441 ") + nick + " " + channel + " :They aren't on that channel" CRLF)

#define ERR_NOTONCHANNEL(channel) \
    (std::string(":442 ") + channel + " :You're not on that channel" CRLF)

#define ERR_USERONCHANNEL(user, channel) \
    (std::string(":443 ") + user + " " + channel + " :is already on channel" CRLF)

#define ERR_CHANOPRIVSNEEDED(channel) \
    (std::string(":482 ") + channel + " :You're not a channel operator" CRLF)

#define ERR_NOPRIVILEGES(nick) \
    (std::string(":481 ") + nick + " :Permission Denied- You're not an IRC operator" CRLF)

#define ERR_ERRONEUSNICK(nick) \
    (std::string(":432 ") + nick + " :Erroneus nickname" CRLF)

#define ERR_BADCHANMASK(server, nick, channel) \
    (std::string(":") + server + " 476 " + nick + " " + channel + " :Bad Channel Mask" CRLF)

#define ERR_CMDNOTFOUND(command) \
    (std::string(":421 ") + command + " :Unknown command" CRLF)

#endif