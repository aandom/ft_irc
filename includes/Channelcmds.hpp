/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channelcmds.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aandom <aandom@student.abudhabi42.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 16:38:06 by aandom            #+#    #+#             */
/*   Updated: 2023/12/07 16:38:06 by aandom           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef CHANNELCMDS_HPP
# define CHANNELCMDS_HPP


# include "Server.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include "error_codes.hpp"

class Server;
class Client;
class Channel;

// typedef struct Channelcmds
// {
//     /* data */
// };

static std::string chcmds[8];


void    sendMsg(int fd, std::string const & msg);


void privMsgchannel(Server &server, Client *client, std::vector<std::string> &input);
void join(Server &server, Client *client, std::vector<std::string> &input);
void part(Server &server, Client *client, std::vector<std::string> &input);
void topic(Server &server, Client *client, std::vector<std::string> &input);
void names(Server &server, Client *client, Channel *channel);
void invite(Server &server, Client *client, std::vector<std::string> &input);
void kick(Server &server, Client *client, std::vector<std::string> &input);

#endif