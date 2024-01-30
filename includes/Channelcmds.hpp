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

typedef struct s_val
{
    std::string key;
    std::string value;
    std::string nick;
} t_val;

typedef struct s_ctr
{
    size_t c;
    size_t idx;
} t_ctr;

// chcmdsutils
std::string                 intToStr(int nbr);
std::string                 trimChars(std::string str, const std::string tobetrimed);
std::vector<std::string>    splitStringTwo(std::string str, char delim);
std::string                 getErrmsg(int code, Server &server );
std::string                 getNewTopic(std::vector<std::string> & input);
std::string                 getInviteMessage(Client * client, std::vector<std::string> & input);
void                        sendMessage(std::string const &msg, Channel * channel);
std::string                 getReason(std::vector<std::string> & input, size_t minpar);
std::string                 getMofchannel(Channel * channel);
std::string                 getRespMsg(Client * client, std::string cmd, std::string paramOne, std::string paramTwo);


int checkChannelName(std::string & chname, int *status);
int checkChInput(std::vector<std::string> & input, size_t minpars);
int checkModes(Channel * channel, Client * client, std::vector<std::string> & input, char checkfor);

void privMsgchannel(Server &server, Client *client, std::vector<std::string> &input);
void join(Server &server, Client *client, std::vector<std::string> &input);
void part(Server &server, Client *client, std::vector<std::string> &input);
void topic(Server &server, Client *client, std::vector<std::string> &input);
void names(Server &server, Client *client, std::vector<std::string> &input);
void invite(Server &server, Client *client, std::vector<std::string> &input);
void kick(Server &server, Client *client, std::vector<std::string> &input);
void mode(Server &server, Client *client, std::vector<std::string> &input);
void noticechannel(Server &server, Client *client, std::vector<std::string> &input);
void quit(Server &server, Client *client);
void sendMessageTwo(std::string const &msg, Channel * channel, Client *sender, std::string cmd);

void removeChannelMembers(Channel * channel);
void popElementsOfVector(std::vector<std::string> &input);
void removeChannelIfnoMember(Server &server, Channel *channel);

bool allDigits(std::string str);

#endif