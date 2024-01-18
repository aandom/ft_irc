#pragma once

#include <map>
#include <iostream>
#include <algorithm>
#include <string>
# define RESET "\033[0m"
# define RED "\033[31m"

#define ERR_NOSUCHNICK "\033[31m 401 "
#define ERR_NOSUCHSERVER "\033[31m 402 "
#define ERR_NOSUCHCHANNEL "\033[31m 403 "
#define ERR_CANNOTSENDTOCHAN "\033[31m 404 "
#define ERR_TOOMANYCHANNELS "\033[31m 405 "
#define ERR_WASNOSUCHNICK "\033[31m 406 "
#define ERR_TOOMANYTARGETS "\033[31m 407 "
#define ERR_UNKNOWNCOMMAND "\033[31m 421 "
#define ERR_NOMOTD "\033[31m 422 "
#define ERR_NONICKNAMEGIVEN "\033[31m 431 "
#define ERR_ERRONEUSNICKNAME "\033[31m 432 "
#define ERR_NICKNAMEINUSE "\033[31m 433 "
#define ERR_NICKCOLLISON "\033[31m 436 "
#define ERR_USERNOTINCHANNEL "\033[31m 441 "
#define ERR_NOTONCHANNEL "\033[31m 442 "
#define ERR_USERONCHANNEL "\033[31m 443 "
#define ERR_NOTREGISTERED "\033[31m 451 "
#define ERR_NEEDMOREPARAMS "\033[31m 461 "
#define ERR_ALREADYREGISTERED "\033[31m 462 "
#define ERR_PASSWDMISMATCH "\033[31m 464 "
#define ERR_CHANNELISFULL "\033[31m 471 "
#define ERR_UNKNOWNMODE "\033[31m 472 "
#define ERR_INVITEONLYCHAN "\033[31m 473 "
#define ERR_BADCHANNELKEY "\033[31m 475 "
#define ERR_BADCHANMASK "\033[31m 476 "
#define ERR_NOPRIVILEGES "\033[31m 481 "
#define ERR_CHANOPRIVSNEEDED "\033[31m 482 "
#define ERR_UMODEUNKOWNFLAG "\033[31m 501 "
#define ERR_USERSDONTMATCH "\033[31m 502 "
#define ERR_KEYSET " 467 "


#define ERR_NOSUCHNICK_MSG "\033[31m :No such nick/channel "
#define ERR_NOSUCHSERVER_MSG "\033[31m :No such server "
#define ERR_NOSUCHCHANNEL_MSG "\033[31m :No such channel "
#define ERR_CANNOTSENDTOCHAN_MSG "\033[31m :Cannot send to channel "
#define ERR_TOOMANYCHANNELS_MSG "\033[31m :You have joined too many channels "
#define ERR_WASNOSUCHNICK_MSG "\033[31m :There was no such nickname "
#define ERR_TOOMANYTARGETS_MSG "\033[31m :Duplicate recipients. No message delivered "
#define ERR_UNKNOWNCOMMAND_MSG "\033[31m :Unknown command "
#define ERR_NOMOTD_MSG "\033[31m:MOTD File is missing "
#define ERR_NONICKNAMEGIVEN_MSG "\033[31m :No nickname given "
#define ERR_ERRONEUSNICKNAME_MSG "\033[31m :Erroneus nickname "
#define ERR_NICKNAMEINUSE_MSG "\033[31m :Nickname is already in use "
#define ERR_NICKCOLLISON_MSG "\033[31m :Nickname collision KILL "
#define ERR_USERNOTINCHANNEL_MSG "\033[31m :They aren't on that channel "
#define ERR_NOTONCHANNEL_MSG "\033[31m :You're not on that channel! "
#define ERR_USERONCHANNEL_MSG "\033[31m :is already on channel "
#define ERR_NOTREGISTERED_MSG "\033[31m :You have not registered "
#define ERR_NEEDMOREPARAMS_MSG "\033[31m :Not enough parameters "
#define ERR_ALREADYREGISTERED_MSG "\033[31m :You may not reregister "
#define ERR_PASSWDMISMATCH_MSG "\033[31m :Password incorrect "
#define ERR_CHANNELISFULL_MSG "\033[31m :Cannot join channel (+l) "
#define ERR_UNKNOWNMODE_MSG "\033[31m :is unknown mode char to me "
#define ERR_INVITEONLYCHAN_MSG "\033[31m :Cannot join channel (+i) "
#define ERR_BADCHANNELKEY_MSG "\033[31m :Cannot join channel (+k) "
#define ERR_BADCHANMASK_MSG "\033[31m 476 "
#define ERR_NOPRIVILEGES_MSG "\033[31m :Permission Denied- You're not an IRC operator "
#define ERR_CHANOPRIVSNEEDED_MSG "\033[31m :You're not channel operator "
#define ERR_UMODEUNKOWNFLAG_MSG "\033[31m :Unknown MODE flag "
#define ERR_USERSDONTMATCH_MSG "\033[31m :Cant change mode for other users "

#define RPL_NOTOPIC_MSG " :No topic is set"
#define RPL_INVITING_MSG " has been invited to "
#define ERR_KEYSET_MSG " :Channel key already set"

