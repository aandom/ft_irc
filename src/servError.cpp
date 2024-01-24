#include "../includes/servError.hpp"

ErrorCode::ErrorCode() {

    // std::string _tostr = "";
    _errmsgs [331] =  RPL_NOTOPIC_MSG ;
    _errmsgs [341] =  RPL_INVITING_MSG ; 
    _errmsgs [401] =  ERR_NOSUCHNICK_MSG ; 
    _errmsgs [402] =  ERR_NOSUCHSERVER_MSG;
    _errmsgs [403] =  ERR_NOSUCHCHANNEL_MSG;
    _errmsgs [404] =  ERR_CANNOTSENDTOCHAN_MSG;
    _errmsgs [405] =  ERR_TOOMANYCHANNELS_MSG;
    _errmsgs [406] =  ERR_WASNOSUCHNICK_MSG;
    _errmsgs [407] =  ERR_TOOMANYTARGETS_MSG;
    _errmsgs [421] =  ERR_UNKNOWNCOMMAND_MSG;
    _errmsgs [422] =  ERR_NOMOTD_MSG;
    _errmsgs [431] =  ERR_NONICKNAMEGIVEN_MSG;
    _errmsgs [432] =  ERR_ERRONEUSNICKNAME_MSG;
    _errmsgs [433] =  ERR_NICKNAMEINUSE_MSG;
    _errmsgs [436] =  ERR_NICKCOLLISON_MSG;
    _errmsgs [441] =  ERR_USERNOTINCHANNEL_MSG;
    _errmsgs [442] =  ERR_NOTONCHANNEL_MSG;
    _errmsgs [443] =  ERR_USERONCHANNEL_MSG;
    _errmsgs [451] =  ERR_NOTREGISTERED_MSG;
    _errmsgs [461] =  ERR_NEEDMOREPARAMS_MSG;
    _errmsgs [462] =  ERR_ALREADYREGISTERED_MSG;
    _errmsgs [464] =  ERR_PASSWDMISMATCH_MSG;
    _errmsgs [467] =  ERR_KEYSET_MSG;
    _errmsgs [471] =  ERR_CHANNELISFULL_MSG;
    _errmsgs [472] =  ERR_UNKNOWNMODE_MSG;
    _errmsgs [473] =  ERR_INVITEONLYCHAN_MSG;
    _errmsgs [475] =  ERR_BADCHANNELKEY_MSG;
    _errmsgs [476] =  ERR_BADCHANMASK_MSG;
    _errmsgs [481] =  ERR_NOPRIVILEGES_MSG;
    _errmsgs [482] =  ERR_CHANOPRIVSNEEDED_MSG;
    _errmsgs [501] =  ERR_UMODEUNKOWNFLAG_MSG;
    _errmsgs [502] =  ERR_USERSDONTMATCH_MSG;
}

ErrorCode::ErrorCode(ErrorCode const &src) {
    *this = src;
}
ErrorCode & ErrorCode::operator=(ErrorCode const &src) {
    if (this != &src) {
        _errmsgs = src._errmsgs;
    }
    return (*this);
}

ErrorCode::~ErrorCode() {}