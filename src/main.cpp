/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgil <cgil@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:10:33 by loruzqui          #+#    #+#             */
/*   Updated: 2025/12/12 13:46:15 by cgil             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "../inc/Server.hpp"

int	main(int argc, char **argv)
{
	Server	server;

	if (argc == 3)
	{
		std::cout << YELLOW << "Welcome to our chat!!" << RESET << std::endl;
		server.init(argv[1], argv[2]);
		std::cout << YELLOW << "Byeee!" << RESET << std::endl;
		return (0);
	}
	else
	{
		std::cout << RED << "Use: ./ircserve <port> <password>" << RESET << std::endl;
		return (1);
	}
}
