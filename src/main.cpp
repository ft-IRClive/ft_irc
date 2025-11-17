/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loruzqui <loruzqui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 15:10:33 by loruzqui          #+#    #+#             */
/*   Updated: 2025/11/17 16:38:29 by loruzqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

int	main(int argc, char **argv)
{
	if (argc == 3)
	{
		return (0);
	}
	else
	{
		std::cout << "Use: ./ircserve <port> <password>" << std::endl;
		return (1);
	}
}
