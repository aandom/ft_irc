#pragma once

#include <map>
#include <string>
#include <algorithm>

#define ERR_NOSUCHNICK          " 401 "
#define ERR_NOSUCHSERVER        " 402 "
#define ERR_NOSUCHCHANNEL       " 403 "
#define ERR_CANNOTSENDTOCHAN    " 404 "
#define ERR_TOOMANYCHANNELS     " 405 "
#define ERR_WASNOSUCHNICK       " 406 "
#define ERR_TOOMANYTARGETS      " 407 "
#define ERR_UNKNOWNCOMMAND      " 421 "
#define ERR_NOMOTD              " 422 "
#define ERR_NONICKNAMEGIVEN     " 431 "
#define ERR_ERRONEUSNICKNAME    " 432 "
#define ERR_NICKNAMEINUSE       " 433 "
#define ERR_NICKCOLLISON        " 436 "
#define ERR_USERNOTINCHANNEL    " 441 "
#define ERR_NOTONCHANNEL        " 442 "
#define ERR_USERONCHANNEL       " 443 "
#define ERR_NOTREGISTERED       " 451 "
#define ERR_NEEDMOREPARAMS      " 461 "
#define ERR_ALREADYREGISTERED   " 462 "
#define ERR_PASSWDMISMATCH      " 464 "
#define ERR_CHANNELISFULL       " 471 "
#define ERR_UNKNOWNMODE         " 472 "
#define ERR_INVITEONLYCHAN      " 473 "
#define ERR_BADCHANNELKEY       " 475 "
#define ERR_BADCHANMASK         " 476 "
#define ERR_NOPRIVILEGES        " 481 "
#define ERR_CHANOPRIVSNEEDED    " 482 "
#define ERR_UMODEUNKOWNFLAG     " 501 "
#define ERR_USERSDONTMATCH      " 502 "
#define ERR_KEYSET              " 467 "
#define ERR_INVALIDMODEPARAM    " 696 " 


#define ERR_NOSUCHNICK_MSG          " :No such nick/channel "
#define ERR_NOSUCHSERVER_MSG        " :No such server "
#define ERR_NOSUCHCHANNEL_MSG       " :No such channel "
#define ERR_CANNOTSENDTOCHAN_MSG    " :Cannot send to channel "
#define ERR_TOOMANYCHANNELS_MSG     " :You have joined too many channels "
#define ERR_WASNOSUCHNICK_MSG       " :There was no such nickname "
#define ERR_TOOMANYTARGETS_MSG      " :Duplicate recipients. No message delivered "
#define ERR_UNKNOWNCOMMAND_MSG      " :Unknown command "
#define ERR_NOMOTD_MSG              ":MOTD File is missing "
#define ERR_NONICKNAMEGIVEN_MSG     " :No nickname given "
#define ERR_ERRONEUSNICKNAME_MSG    " :Erroneus nickname "
#define ERR_NICKNAMEINUSE_MSG       " :Nickname is already in use "
#define ERR_NICKCOLLISON_MSG        " :Nickname collision KILL "
#define ERR_USERNOTINCHANNEL_MSG    " :They aren't on that channel "
#define ERR_NOTONCHANNEL_MSG        " :You're not on that channel! "
#define ERR_USERONCHANNEL_MSG       " :is already on channel "
#define ERR_NOTREGISTERED_MSG       " :You have not registered "
#define ERR_NEEDMOREPARAMS_MSG      " :Not enough parameters "
#define ERR_ALREADYREGISTERED_MSG   " :You may not reregister "
#define ERR_PASSWDMISMATCH_MSG      " :Password incorrect "
#define ERR_CHANNELISFULL_MSG       " :Cannot join channel (+l) "
#define ERR_UNKNOWNMODE_MSG         " :is unknown mode char to me "
#define ERR_INVITEONLYCHAN_MSG      " :Cannot join channel (+i) "
#define ERR_BADCHANNELKEY_MSG       " :Cannot join channel (+k) "
#define ERR_BADCHANMASK_MSG         " 476 "
#define ERR_NOPRIVILEGES_MSG        " :Permission Denied- You're not an IRC operator "
#define ERR_CHANOPRIVSNEEDED_MSG    " :You're not channel operator "
#define ERR_UMODEUNKOWNFLAG_MSG     " :Unknown MODE flag "
#define ERR_USERSDONTMATCH_MSG      " :Cant change mode for other users "

#define RPL_NOTOPIC_MSG             " :No topic is set"
#define RPL_INVITING_MSG            " has been invited to "
#define ERR_KEYSET_MSG              " :Channel key already set"
#define ERR_INVALIDMODEPARAM        " 696 "

#define NOTENOUGH_OPPARAM            " :o * You must specify a parameter for the op mode. Syntax: <nick>."
#define NOTENOUGH_KEYPARAM           " :k * You must specify a parameter for the key mode. Syntax: <key>."
#define NOTENOUGH_LIMPARAM           " :l * You must specify a parameter for the limit mode. Syntax: <limit>."
#define INVALID_LIMIT           " Invalid limit mode parameter. Syntax: <limit>."

