# include "./includes/Channelcmds.hpp"

void   sendTopicAndMembers(Channel *channel, Client *client, int isname) {
    std::string topic = channel->getChName();
    topic = topic + " :" + channel->getTopic();
    std::string names = "= " + channel->getChName() + " :";
    std::vector<std::string> nicknames = channel->getNickNames(1);
    for(std::vector<std::string>::iterator it = nicknames.begin(); it != nicknames.end(); ++it) {
        names = names + *it + " ";
    }
    if (!isname)
        serverReply(" 332 ", topic, client);
    serverReply(" 353 ", names, client);
    serverReply(" 366 ",  channel->getChName() + " :END of /NAMES list", client); 
}

void popElementsOfVector(std::vector<std::string> &input) {
    while (input.size())
        input.pop_back();
}

// send message regarding channel to newly joined client
void namesUtils(Server &server, Client *client, std::vector<std::string> &input) {
    Channel *channel = NULL;
    int     status;
    std::string chname = "";
    std::string code = "";

    if (input.size() > 1)
        chname = input[1];
    status = checkChInput(input, 2);
    if (status) {
        // serverReply(std::to_string(status), getErrmsg(status, server), client);
        serverReplyofChannel(intToStr(status), chname, getErrmsg(status, server), client);
        throw std::invalid_argument(getErrmsg(status, server));
    }
    channel = server.getChannelIfExist(input[1]);
    if (channel == NULL) {
        serverReplyofChannel("403", chname, getErrmsg(403, server), client);
        throw std::invalid_argument(ERR_NOSUCHCHANNEL_MSG);
    }
    sendTopicAndMembers(channel, client, 1);
    // send all three messages to client;
}

void names(Server &server, Client *client, std::vector<std::string> &input) {
    size_t      i = 0;
    std::vector<std::string> chanames;
    std::vector<std::string>  finalinput;
    if (input.size() > 1)
        chanames = splitStringTwo(input[1], ',');
    if (chanames.size() > 1) {
        while (i < chanames.size()) {
            finalinput.push_back("NAMES");
            finalinput.push_back(chanames[i]);
            try {
                namesUtils(server, client, finalinput);
            } catch(const std::exception& e) {
                std::cerr << e.what() << '\n';
            }
            popElementsOfVector(finalinput);
            i++;
        }   
    }
    else
    {
        try {
            namesUtils(server, client, input);
        } catch(const std::exception& e) {
            std::cerr << e.what() << '\n';
        }
    }

}


void privMsgchannel(Server &server, Client *client, std::vector<std::string> &input) {
    Channel *channel = NULL;
    std::string msg = "";
    int         status;

    status = checkChInput(input, 3);
    if (status)
        throw std::invalid_argument(getErrmsg(status, server));
    channel = server.getChannelIfExist(input[1]);
    if (channel == NULL) {
        throw std::invalid_argument(ERR_NOSUCHNICK_MSG);
    }
    if (!channel->checkIfMember(client->nickname))
        throw std::invalid_argument(ERR_NOTONCHANNEL_MSG);
    for (std::vector<std::string>::const_iterator it = input.begin() + 2; it != input.end(); it++) {
		msg.append(*it + " ");
    }
    // printchannelmembers(channel);
    sendMessage(msg, channel);
}


void joinUtil(Server &server, Client *client, std::vector<std::string> &input) {
    Channel     *channel = NULL;
    std::string chkey = "";
    std::string chname = "";
    std::string pas;
    int         status;
    std::vector<std::string> tobe;
    if (input.size() >1)
        chname = input[1];
    status = checkChInput(input, 2);
    if (status) {
        // serverReply(std::to_string(status), getErrmsg(status, server), client);
        serverReplyofChannel(intToStr(status), chname, getErrmsg(status, server), client);
        throw std::invalid_argument(getErrmsg(status, server));
    }
    channel = server.getChannelIfExist(input[1]);
    if (channel == NULL) {
        channel = new Channel(input[1], chkey);
        server.addChannel(channel);
        channel->addAdmin(client);
    }
    status = checkModes(channel, client, input, 'i');
    if (status) {
        // serverReply(std::to_string(status), getErrmsg(status, server), client);
        serverReplyofChannel(intToStr(status), chname, getErrmsg(status, server), client);
        throw std::invalid_argument(getErrmsg(status, server));
    }
    if (channel->checkIfMember(client->nickname))
        return ;
    channel->addClient(client);
    channel->removeFromInvitation(client);
    // printchannelmembers(channel);
    sendMessage(getJoinMessage(client, input), channel);
    sendTopicAndMembers(channel, client, 0);
}

void join(Server &server, Client *client, std::vector<std::string> &input) {
    size_t      i = 0;
    std::vector<std::string> chanames;
    std::vector<std::string> chkeys;
    std::vector<std::string>  finalinput;
    if (input.size() > 1)
        chanames = splitStringTwo(input[1], ',');
    if (input.size() > 2)
        chkeys = splitStringTwo(input[2], ',');
    if (chanames.size() > 1) {
        while (i < chanames.size()) {
            finalinput.push_back("JOIN");
            finalinput.push_back(chanames[i]);
            if (chkeys.size() > i)
                finalinput.push_back(chkeys[i]);
            try {
                joinUtil(server, client, finalinput);
            } catch(const std::exception& e) {
                std::cerr << e.what() << '\n';
            }
            popElementsOfVector(finalinput);
            i++;
        }   
    }
    else
    {
        try {
            joinUtil(server, client, input);
        } catch(const std::exception& e) {
            std::cerr << e.what() << '\n';
        }
    }
}


void partUtil(Server &server, Client *client, std::vector<std::string> &input) {
    Channel *channel = NULL;
    int     status;
    std::string chname = "";

    if (input.size() > 1)
        chname = input[1];
    status = checkChInput(input, 2);
    if (status) {
        // serverReply(std::to_string(status), getErrmsg(status, server), client);
        serverReplyofChannel(intToStr(status), chname, getErrmsg(status, server), client);
        throw std::invalid_argument(getErrmsg(status, server));
    }
    channel = server.getChannelIfExist(input[1]);
    if (channel == NULL) {
        serverReplyofChannel(" 403 ", chname, getErrmsg(403, server), client);
        throw std::invalid_argument(ERR_NOSUCHCHANNEL_MSG);
    }
	if (!channel->checkIfMember(client->nickname)) 
    {
        serverReplyofChannel(" 442 ", chname, getErrmsg(442, server), client);
		throw std::invalid_argument(ERR_NOTONCHANNEL_MSG);
    }

    // build part message here
	// send part message to all users of the channel
    sendMessage(getPartMessage(client, input), channel);
	channel->removeClient(client);
	return ;

}

void part(Server &server, Client *client, std::vector<std::string> &input) {
    size_t      i = 0;
    std::vector<std::string> chanames;
    std::vector<std::string>  finalinput;
    if (input.size() > 1)
        chanames = splitStringTwo(input[1], ',');
    if (chanames.size() > 1) {
        while (i < chanames.size()) {
            finalinput.push_back("PART");
            finalinput.push_back(chanames[i]);
            try {
                partUtil(server, client, finalinput);
            } catch(const std::exception& e) {
                std::cerr << e.what() << '\n';
            }
            popElementsOfVector(finalinput);
            i++;
        }   
    }
    else
    {
        try {
            partUtil(server, client, input);
        } catch(const std::exception& e) {
            std::cerr << e.what() << '\n';
        }
    }
}


void topic(Server &server, Client *client, std::vector<std::string> &input) {
	Channel *channel = NULL;
    int         status;
    std::string new_topic = "";
    std::string chname = "";

    if (input.size() > 1)
        chname = input[1];
    status = checkChInput(input, 2);
    if (status) {
        serverReplyofChannel(intToStr(status), chname, getErrmsg(status, server), client);
        throw std::invalid_argument(getErrmsg(status, server));
    }
    channel = server.getChannelIfExist(input[1]);
    if (channel == NULL) {
    
        serverReplyofChannel(" 403 ", chname, getErrmsg(403, server), client);
        throw std::invalid_argument(ERR_NOSUCHCHANNEL_MSG);
    }
	if (!channel->checkIfMember(client->nickname)) {
    
        serverReplyofChannel(" 442 ", chname, getErrmsg(442, server), client);
		throw std::invalid_argument(ERR_NOTONCHANNEL_MSG);
    }
	if (input.size() == 2 && channel->getTopic().size() > 0) {
        serverReply(" 332 ", channel->getChName() + " " + channel->getTopic(), client);
		return ;
	}
    status = checkModes(channel, client, input, 't');
    if (status) {
        serverReplyofChannel(" 482 ", chname, getErrmsg(482, server), client);
		throw std::invalid_argument(ERR_CHANOPRIVSNEEDED_MSG);
    }
	if (input.size() == 2 && channel->getTopic().size() == 0)
    {
        serverReplyofChannel(" 331 ", chname, getErrmsg(331, server), client);
		throw std::invalid_argument(getErrmsg(331, server));
    }
    new_topic = getNewTopic(input);
	channel->setTopic(new_topic);
    sendMessage(getTopicMessage(client, input) + " " + new_topic , channel);
	return ; 
}


void invite(Server &server, Client *client, std::vector<std::string> &input) {

	Client *invitee;
	Channel *channel;
    int      status;
    std::string chname = "";

    if (input.size() > 2)
        chname = input[2];
    status = checkChInput(input, 3);
    if (status) {
        serverReplyofChannel(intToStr(status), chname, getErrmsg(status, server), client);
        throw std::invalid_argument(getErrmsg(status, server));
    }
    channel = server.getChannelIfExist(input[2]);
    if (channel == NULL) {
    
        serverReplyofChannel(" 403 ", chname, getErrmsg(403, server), client);
        throw std::invalid_argument(ERR_NOSUCHCHANNEL_MSG);
    }

	invitee = server.getClientIfExist(input[1]);
	if (!invitee){
        serverReply(" 401 ", getErrmsg(401, server), client);
        throw std::invalid_argument(ERR_NOSUCHCHANNEL_MSG);
    }
	// Check if this client is an operator
	if (!channel->isOperator(client)){
        serverReply(" 482 ", getErrmsg(482, server), client);
		throw std::invalid_argument((ERR_CHANOPRIVSNEEDED_MSG));
    }
	// Check if invitee is already in channel
	if (channel->checkIfMember(invitee->nickname)){
        serverReply(" 443 ", getErrmsg(443, server), client);
        throw std::invalid_argument(ERR_USERONCHANNEL_MSG);
    }
	channel->addToInvitation(invitee);
    // serverReply(" 341 ",  ": " + invitee->nickname + getErrmsg(341, server) + channel->getChName(), client);
    serverReply(" 341 ", invitee->nickname + " " + channel->getChName(), client);
    // serverReply(" 341 ",   ": " + client->nickname + " Is inviting you to join the channel " + channel->getChName(), invitee);
    // serverReply(" 341 ", getInviteMessage(client, input), invitee);
    sendResponse1(getInviteMessage(client, input), invitee);
    // sendMessage(getInviteMessage(client, input), channel);
    // all invitation messages here
	return ;
}


void kick(Server &server, Client *client, std::vector<std::string> &input) {

	Channel *channel;
	Client *toBeRemoved = NULL;
    int      status;
    std::string chname = "";
    std::string reason = "";

    if (input.size() > 2)
        chname = input[2];
    status = checkChInput(input, 3);
    if (status) {
        serverReplyofChannel(intToStr(status), chname, getErrmsg(status, server), client);
        throw std::invalid_argument(getErrmsg(status, server));
    }
    channel = server.getChannelIfExist(input[1]);
    if (channel == NULL) {
    
        serverReplyofChannel(" 403 ", chname, getErrmsg(403, server), client);
        throw std::invalid_argument(ERR_NOSUCHCHANNEL_MSG);
    }

	if (!channel->checkIfMember(client->nickname)) {
        serverReplyofChannel(" 442 ", chname, getErrmsg(442, server), client);
		throw std::invalid_argument(ERR_NOTONCHANNEL_MSG);
    }
	if (!channel->isOperator(client)){
        serverReply(" 482 ", getErrmsg(482, server), client);
		throw std::invalid_argument((ERR_CHANOPRIVSNEEDED_MSG));
    }
	// Check if user to kick exists on server and channel
	toBeRemoved = server.getClientIfExist(input[2]);
	if (!toBeRemoved || !channel->checkIfMember(toBeRemoved->nickname)){
        serverReply(" 441 ",  chname + " " + getErrmsg(441, server), client);
		throw std::invalid_argument((ERR_USERNOTINCHANNEL_MSG));
    }
    reason = getReason(input);
	// send remove message to 
    sendMessage(getKickMessage(client, input) + " :" + reason , channel);
	channel->removeClient(toBeRemoved);
	return ;

}


int    tryExtractKeyMode(char sign, char m,  size_t & c, std::map<std::string, std::string> & nmodes, std::vector<std::string> &input) {
    if ((sign == '+' || sign == '-') && (m == 'k' && input.size() > c)) {
        if (sign == '+') {
            nmodes["+k"] = input[c];
            c++;
        }
        else
            nmodes["-k"] = "";
        return (1);
    }
    return (0);
}

int    tryExtractOprMode(char sign, char m,  size_t & c, std::map<std::string, std::string> & nmodes, std::vector<std::string> &input) {
    if ((sign == '+' || sign == '-') && (m == 'o' && input.size() > c)) {
        if (sign == '+')
            nmodes["+o"] = input[c];
        else
            nmodes["-o"] = input[c];
        c++;
        return (1);
    }
    return (0);
}

int    tryExtractLimitMode(char sign, char m,  size_t & c, std::map<std::string, std::string> & nmodes, std::vector<std::string> &input) {
    if ((sign == '+' || sign == '-') && (m == 'l' && input.size() > c)) {
        if (sign == '+') {
            nmodes["+l"] = input[c];
            c++;
        }
        else
            nmodes["-l"] = "";
        return (1);
    }
    return (0);
}

int    tryExtractOtherMode(char sign, char m,  std::map<char, bool> chmode , std::map<std::string, std::string> & nmodes) {
    std::string newm;
    if ((sign == '+' || sign == '-') && (chmode.find(m) != chmode.end())) {
        newm = "";
        newm = newm + sign + m;
        nmodes[newm] = "";
        return (1);
    }
    return (0);
}


void    recordMode(std::string &modes, char & sign, char newsign, std::string newmode) {
    if (sign == '+' && newsign == '+') {
        modes += newmode;
    }
    else if ((sign == 'n' || sign == '+') && newsign == '-') {
        modes = modes + '-'  + newmode;
        sign = '-';
    }
    else if (sign == '-' && newsign == '-') {
        modes += newmode;
    }
    else if ((sign == 'n' || sign == '-') && newsign == '+') {
        modes = modes + '+'  + newmode;
        sign = '+';
    }
    return ;
}


int applyKeyMode(Channel *channel, std::string key, std::string value,  std::string & modes, char & sign) {
    if (key == "+k" && !channel->getmodeAt('k') && value != "") {
        channel->setMode('k', true);
        channel->setChKey(value);
        recordMode(modes, sign, '+', "k");
        return (1);
    }
    else if (key == "-k" && channel->getmodeAt('k') && value != "") {
        channel->setMode('k', false);
        channel->setChKey("");
        recordMode(modes, sign, '-', "k");
        return (1);
    }
    return (0);
}

int applyOprMode(Server &server, Channel *channel, std::string key, std::string nick, std::string & modes, char & sign) {
    if (key == "+o" && (nick != "" && channel->checkIfMember(nick) && !channel->isNickIsAdmin(nick))) {
        Client * newclient = server.getClientIfExist(nick);
        channel->addAdmin(newclient);
        recordMode(modes, sign, '+', "o");
        return (1);
    }
    else if (key == "-o" && channel->checkIfMember(nick) && channel->isNickIsAdmin(nick)) {
        Client * newclient = server.getClientIfExist(nick);
        channel->removeAddmin(newclient);
        recordMode(modes, sign, '-', "o");
        return (1);
    }
    return (0);
}

int applyLimitMode(Server &server, Channel *channel, std::string key, std::string limit, std::string &modes, char & sign) {
    (void) server;
    if (key == "+l" && (limit != "")) {
        size_t value = static_cast<size_t>(std::strtoll((limit.c_str()), NULL, 10));
        channel->setMode('l', true);
        channel->setChLimit(value);
        recordMode(modes, sign, '+', "l");
        return (1);
    }
    else if (key == "-l") {
        channel->setMode('l', false);
        recordMode(modes, sign, '-', "l");
        return (1);
    }
    return (0);
}

int applyInviteMode(Server &server, Channel *channel, std::string key, std::string value, std::string &modes, char &sign) {
    (void) value;
    (void) server;
    if (key == "-i" && channel->getmodeAt('i')) {
        channel->setMode('i', false);
        recordMode(modes, sign, '-', "i");
        return (1);
    }
    else if (key == "+i" && !channel->getmodeAt('i')) {
        channel->setMode('i', true);
        recordMode(modes, sign, '+', "i");
        return (1);
    }
    return (0);
}

int applyTopicMode(Server &server, Channel *channel, std::string key, std::string value, std::string &modes, char &sign) {
    (void) value;
    (void) server;
    if (key == "+t" && !channel->getmodeAt('t')) {
        channel->setMode('t', true);
        recordMode(modes, sign, '+', "t");
        return (1);
        // modes += "+t";
    }
    else if (key == "-t" && channel->getmodeAt('t')) {
        channel->setMode('t', false);
        recordMode(modes, sign, '-', "t");
        return (1);
        // modes += "-t";
    }
    return (0);
}

void tryApplyMode(Server &server, Client *client, Channel *channel, std::vector<std::string> &input, std::map<std::string, std::string>  nmodes) {
    std::map<std::string, std::string>::iterator cit = nmodes.begin();
    std::string appliedmode = "";
    char sign = 'n';
    for (; cit != nmodes.end(); cit++) {
        std::cout << cit->first << " - ";
        if (applyKeyMode(channel, cit->first, cit->second, appliedmode, sign))
            continue;
        else if (applyOprMode(server, channel, cit->first, cit->second, appliedmode, sign))
            continue;
        else if (applyLimitMode(server, channel, cit->first, cit->second, appliedmode, sign))
            continue;
        else if (applyInviteMode(server, channel, cit->first, cit->second, appliedmode, sign))
            continue;
        else if (applyTopicMode(server, channel, cit->first, cit->second, appliedmode, sign))
            continue;
    }
    if (appliedmode.size() != 0)
        sendMessage(getModeMessageTwo(client, input[1], appliedmode), channel);
}

void modeUtils(Server &server, Client *client, Channel *channel, std::vector<std::string> &input) {
    std::string trimed = trimChars(input[2], " \n\r ");
    std::map<std::string, std::string>  newmodes;
    std::map<char, bool> chmode = channel->getMode();
    size_t  counter = 3;
    int     c = 0;
    char    sign = trimed[0];

    for (std::string::iterator it = trimed.begin() + 1; it != trimed.end(); it++) {
        if (std::isspace(*it))
            continue;
        c++;
        if ((sign == '+' || sign == '-') && (*it == 'o' && input.size() <= counter))
            serverReplyofChannel(ERR_NEEDMOREPARAMS, channel->getChName() , getErrmsg(461, server), client);
        else if (sign == '+' && ((*it == 'k' || *it == 'l') && input.size() <= counter))
            serverReplyofChannel(ERR_NEEDMOREPARAMS, channel->getChName() , getErrmsg(461, server), client);
        else if (tryExtractKeyMode(sign, *it, counter, newmodes, input))
            continue;
        else if (tryExtractOprMode(sign, *it, counter, newmodes, input))
            continue;
        else if (tryExtractLimitMode(sign, *it, counter, newmodes, input))
            continue;
        else if (tryExtractOtherMode(sign, *it, chmode, newmodes))
            continue;
        else if ((*it == '+' || *it == '-') && c == 1)
            serverReplyofChannel(ERR_UNKNOWNMODE, channel->getChName() , getErrmsg(472, server), client);
        else if ((*it == '+' || *it == '-') && (c > 1 && *(it - 1) != '-' && *(it - 1) != '+'))
            sign = *it;
        else
            serverReplyofChannel(ERR_UNKNOWNMODE, channel->getChName() , getErrmsg(472, server), client);
    }

    tryApplyMode(server, client, channel, input, newmodes);
}

void mode(Server &server, Client *client, std::vector<std::string> &input) {

	Channel *channel;
    int      status;
    std::string chname = "";

    if (input.size() >= 2)
        chname = input[1];
    status = checkChInput(input, 2);
    if (status) {
        serverReplyofChannel(intToStr(status), chname, getErrmsg(status, server), client);
        throw std::invalid_argument(getErrmsg(status, server));
    }
    channel = server.getChannelIfExist(input[1]);
    if (channel == NULL) {
    
        serverReplyofChannel(" 403 ", chname, getErrmsg(403, server), client);
        throw std::invalid_argument(ERR_NOSUCHCHANNEL_MSG);
    }

	if (!channel->checkIfMember(client->nickname)) {
        serverReplyofChannel(" 442 ", chname, getErrmsg(442, server), client);
		throw std::invalid_argument(ERR_NOTONCHANNEL_MSG);
    }
    if (input.size() == 2)
        return (serverReply(" 324 ", chname + " " + getMofchannel(channel), client));
	if (!channel->isOperator(client)){
        serverReply(" 482 ", getErrmsg(482, server), client);
		throw std::invalid_argument((ERR_CHANOPRIVSNEEDED_MSG));
    }
    modeUtils(server, client, channel, input);
	return ;
}
