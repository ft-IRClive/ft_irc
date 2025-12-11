/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 17:43:14 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/11 20:57:26 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Server.hpp"

void Server::_handlerClientCap(const std::string &buffer, const int fd)
{
	std::istringstream	iss(buffer);
	std::string			subcommand;
	std::string			caps;

	//Extract the name of the command
	iss >> subcommand;

	//Convert to uppercase
	for (size_t i = 0; i < subcommand.size(); i++)
		subcommand[i] = std::toupper(subcommand[i]);

	//Verify subcommand
	if (subcommand == "LS")
		_sendResponse(fd, ":" + _getHostname() + " CAP * LS :");
	else if (subcommand == "REQ")
	{
		//Extract the parameters of the command
		iss >> caps;
		if (!caps.empty())
			_sendResponse(fd, ":" + _getHostname() + " CAP * NAK :" + caps);
	}
	else if (subcommand == "LIST")
		_sendResponse(fd, ":" + _getHostname() + " CAP * LIST :");
}
