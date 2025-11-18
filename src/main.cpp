/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:10:33 by loruzqui          #+#    #+#             */
/*   Updated: 2025/11/18 15:24:41 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "../inc/Server.hpp"

int	main(int argc, char **argv)
{
	Server	server;

	if (argc == 3)
	{
		std::cout << "Welcome to our chat!!" << std::endl;
		server.init(argv[1], argv[2]);
		std::cout << "Byeee!" << std::endl;
		return (0);
	}
	else
	{
		std::cout << "Use: ./ircserve <port> <password>" << std::endl;
		return (1);
	}
}
