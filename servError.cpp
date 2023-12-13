# include "includes/servError.hpp"

ErrorCode::ErrorCode() {

    std::string _tostr = "";
    _errmsgs [401] =  _tostr + ERR_NOSUCHNICK_MSG ; 
    _errmsgs [402] =  _tostr + ERR_NOSUCHSERVER_MSG;
    _errmsgs [403] =  _tostr + ERR_NOSUCHCHANNEL_MSG;
    _errmsgs [404] =  _tostr + ERR_CANNOTSENDTOCHAN_MSG;
    _errmsgs [405] =  _tostr + ERR_TOOMANYCHANNELS_MSG;
    _errmsgs [406] =  _tostr + ERR_WASNOSUCHNICK_MSG;
    _errmsgs [407] =  _tostr + ERR_TOOMANYTARGETS_MSG;
    _errmsgs [421] =  _tostr + ERR_UNKNOWNCOMMAND_MSG;
    _errmsgs [422] =  _tostr + ERR_NOMOTD_MSG;
    _errmsgs [431] =  _tostr + ERR_NONICKNAMEGIVEN_MSG;
    _errmsgs [432] =  _tostr + ERR_ERRONEUSNICKNAME_MSG;
    _errmsgs [433] =  _tostr + ERR_NICKNAMEINUSE_MSG;
    _errmsgs [436] =  _tostr + ERR_NICKCOLLISON_MSG;
    _errmsgs [441] =  _tostr + ERR_USERNOTINCHANNEL_MSG;
    _errmsgs [442] =  _tostr + ERR_NOTONCHANNEL_MSG;
    _errmsgs [443] =  _tostr + ERR_USERONCHANNEL_MSG;
    _errmsgs [451] =  _tostr + ERR_NOTREGISTERED_MSG;
    _errmsgs [461] =  _tostr + ERR_NEEDMOREPARAMS_MSG;
    _errmsgs [462] =  _tostr + ERR_ALREADYREGISTERED_MSG;
    _errmsgs [464] =  _tostr + ERR_PASSWDMISMATCH_MSG;
    _errmsgs [471] =  _tostr + ERR_CHANNELISFULL_MSG;
    _errmsgs [472] =  _tostr + ERR_UNKNOWNMODE_MSG;
    _errmsgs [473] =  _tostr + ERR_INVITEONLYCHAN_MSG;
    _errmsgs [475] =  _tostr + ERR_BADCHANNELKEY_MSG;
    _errmsgs [476] =  _tostr + ERR_BADCHANMASK_MSG;
    _errmsgs [481] =  _tostr + ERR_NOPRIVILEGES_MSG;
    _errmsgs [482] =  _tostr + ERR_CHANOPRIVSNEEDED_MSG;
    _errmsgs [501] =  _tostr + ERR_UMODEUNKOWNFLAG_MSG;
    _errmsgs [502] =  _tostr + ERR_USERSDONTMATCH_MSG;
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