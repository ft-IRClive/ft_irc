/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 10:08:55 by claudia           #+#    #+#             */
/*   Updated: 2025/12/17 16:45:31 by loruzqui         ###   ########.fr       */
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


#define RPL_CHANGEMODE(host, channel, mode, args) \
	(std::string(":") + host + " MODE " + channel + " " + mode + " " + args + CRLF)


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


	#define RPL_KICK(host, channel, kicker, kicked, comment) \
	(std::string(":") + host + " KICK " + channel + " " + kicked + " :" + comment + CRLF)


#define RPL_INVITING(host, channel, inviter, invited) \
	(std::string(":") + host + " INVITE " + invited + " " + channel + CRLF)


#define RPL_INVITE_MSG(inviterNick, inviterHost, invitedNick, channel) \
	(std::string(":") + inviterNick + "!" + inviterHost + \
	" INVITE " + invitedNick + " " + channel + CRLF)


#define RPL_PRIVMSG(nick, host, receiver, text) \
	(std::string(":") + nick + "!" + host + " PRIVMSG " + receiver + " :" + text + CRLF)


#define RPL_NICK(prefix, newnick) \
	(std::string(":") + (prefix) + " NICK " + (newnick) + CRLF)


#define RPL_WHOREPLY(server, requester, channel, user, host, server2, nick, status, realname) \
	(std::string(":") + (server) + " 352 " + (requester) + " " + (channel) + " " + (user) + " " + (host) + " " + (server2) + " " + (nick) + " " + (status) + " :0 " + (realname) + CRLF)


#define RPL_ENDOFWHO(server, requester, channel) \
	(std::string(":") + (server) + " 315 " + (requester) + " " + (channel) + " :End of WHO list" + CRLF)


#define ERR_UNKNOWNMODE(server, nick, channel, mode) \
	(std::string(":") + server + " 472 " + nick + " " + channel + " " + mode + " :is not a recognised channel mode" CRLF)


#define ERR_SYNTAX_PASS(server, nick) \
	(std::string(":") + server + " 461 " + nick + " PASS" + \
	" :Not enough parameters. Syntax: PASS <password>" + CRLF)


#define ERR_SYNTAX_NICK(server, nick) \
	(std::string(":") + server + " 461 " + nick + " NICK" + \
	" :Not enough parameters. Syntax: NICK <nick>" + CRLF)


#define ERR_SYNTAX_USER(server, nick) \
	(std::string(":") + server + " 461 " + nick + " USER" + \
	" :Not enough parameters. Syntax: USER <username> <hostname> <server> :<realname>" + CRLF)


#define ERR_SYNTAX_JOIN(server, nick) \
	(std::string(":") + server + " 461 " + nick + " JOIN" + \
	" :Not enough parameters. Syntax: JOIN <#canal> [<clave>]" + CRLF)


#define ERR_SYNTAX_PART(server, nick) \
	(std::string(":") + server + " 461 " + nick + " PART" + \
	" :Not enough parameters. Syntax: PART <#canal> [<mensaje>]" + CRLF)


#define ERR_SYNTAX_TOPIC(server, nick) \
	(std::string(":") + server + " 461 " + nick + " TOPIC" + \
	" :Not enough parameters. Syntax: TOPIC <#canal> [:<nuevo topic>]" + CRLF)


#define ERR_SYNTAX_PRIVMSG(server, nick) \
	(std::string(":") + server + " 461 " + nick + " PRIVMSG" + \
	" :Not enough parameters. Syntax: PRIVMSG <destino> :<mensaje>" + CRLF)


#define ERR_SYNTAX_WHO(server, nick) \
	(std::string(":") + server + " 461 " + nick + " WHO" + \
	" :Not enough parameters. Syntax: WHO <#channel>" + CRLF)


#define ERR_SYNTAX_QUIT(server, nick) \
	(std::string(":") + server + " 461 " + nick + " QUIT" + \
	" :Not enough parameters. Syntax: QUIT [:<mensaje>]" + CRLF)


#define ERR_SYNTAX_KICK(server, nick) \
	(std::string(":") + server + " 461 " + nick + " KICK" + \
	" :Not enough parameters. Syntax: KICK <#canal> <nick> [<razón>]" + CRLF)


#define ERR_SYNTAX_INVITE(server, nick) \
	(std::string(":") + server + " 461 " + nick + " INVITE" + \
	" :Not enough parameters. Syntax: INVITE <nick> <#canal>" + CRLF)


#define ERR_SYNTAX_MODE(server, nick) \
	(std::string(":") + server + " 461 " + nick + " MODE" + \
	" :Not enough parameters. Syntax: MODE <target> <modes> [parameters...]" + CRLF)


#define ERR_INVALIDNICK(server, nick) \
	(std::string(":") + server + " 432 " + nick + " :Erroneous nickname" CRLF)


#define ERR_INCORPASS(server, nick) \
	(std::string(":") + server + " 464 " + nick + " :Password incorrect !" CRLF)


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


#define ERR_BADCHANMASK(server, nick, channel) \
	(std::string(":") + server + " 476 " + nick + " " + channel + " :Bad Channel Mask" CRLF)


#define ERR_CMDNOTFOUND(server, command) \
	(std::string(":") + server + " 421 " + command + " :Unknown command" CRLF)


#define ERR_NOSUCHNICK(server, nick) \
	(std::string(":") + server + " 401 * " + nick + " :No such nick/channel" CRLF)


#define RPL_CAP_LS(server) \
	(std::string(":") + server + " CAP * LS :" + CRLF)


#define RPL_CAP_NAK(server, caps) \
	(std::string(":") + server + " CAP * NAK :" + caps + CRLF)


#define RPL_CAP_LIST(server) \
	(std::string(":") + server + " CAP * LIST :" + CRLF)


#endif
