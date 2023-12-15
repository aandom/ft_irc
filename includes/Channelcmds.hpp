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


// chcmdsutils
std::string trimChars(std::string str, const std::string tobetrimed);
void   printchannelmembers(Channel * channel);
void   printVector(std::vector<std::string> & input);
std::vector<std::string> splitStringTwo(std::string str, char delim);
std::string getErrmsg(int code, Server &server );
int checkChannelName(std::string & chname, int *status);
int checkChInput(std::vector<std::string> & input, size_t minpars);
int checkModes(Channel * channel, Client * client, std::vector<std::string> & input, char checkfor);

std::string getJoinMessage(Client * client, std::vector<std::string> & input);
std::string getNewTopic(std::vector<std::string> & input);
std::string getPartMessage(Client * client, std::vector<std::string> & input);
std::string getTopicMessage(Client * client, std::vector<std::string> & input);
std::string getInviteMessage(Client * client, std::vector<std::string> & input);
void    sendMessage(std::string const &msg, Channel * channel);
std::string getReason(std::vector<std::string> & input);
std::string getKickMessage(Client * client, std::vector<std::string> & input);
std::string getModeMessage(Client * client, std::vector<std::string> & input);



void    sendMsg(int fd, std::string const & msg);

void joinUtil(Server &server, Client *client, std::vector<std::string> &input);

void privMsgchannel(Server &server, Client *client, std::vector<std::string> &input);
void join(Server &server, Client *client, std::vector<std::string> &input);
void part(Server &server, Client *client, std::vector<std::string> &input);
void topic(Server &server, Client *client, std::vector<std::string> &input);
// void names(Server &server, Client *client, Channel *channel);
void names(Server &server, Client *client, std::vector<std::string> &input);
void invite(Server &server, Client *client, std::vector<std::string> &input);
void kick(Server &server, Client *client, std::vector<std::string> &input);
void mode(Server &server, Client *client, std::vector<std::string> &input);

#endif