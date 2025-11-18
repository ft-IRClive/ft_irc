/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:22:46 by loruzqui          #+#    #+#             */
/*   Updated: 2025/11/18 15:30:56 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>

class Server
{
	private:
		std::string	_port;
		std::string	_passwd;

	public:
		Server(void);
		~Server();

		void init(const std::string &port, const std::string &passwd);
};

#endif
