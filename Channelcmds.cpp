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
    if (channel->checkIfMember(client->nickname))
        serverReply(" 353 ", names, client);
    serverReply(" 366 ",  channel->getChName() + " :END of /NAMES list.", client); 
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
        serverReplyofChannelsec(" " + intToStr(status) + " ", " " + client->nickname + " " + chname + getErrmsg(status, server), client);
        throw std::invalid_argument(getErrmsg(status, server));
    }
    channel = server.getChannelIfExist(input[1]);
    if (channel == NULL) {
        serverReplyofChannelsec(" 403 ", " " + client->nickname + " " + chname + getErrmsg(403, server), client);
        // serverReply(" 366 ",  chname + " :END of /NAMES list.", client); 
        // throw std::invalid_argument(ERR_NOSUCHCHANNEL_MSG);
        return ;
    }
    if (!channel->checkIfMember(client->nickname)) {
        serverReply(" 366 ",  chname + " :END of /NAMES list.", client); 
        // throw std::invalid_argument(ERR_NOSUCHCHANNEL_MSG);
        return ;
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
            namesUtils(server, client, finalinput);
            popElementsOfVector(finalinput);
            i++;
        }
    }
    else {
        std::cout << "entering else part here \n" << std::endl;
        namesUtils(server, client, input);
    }
}


void privMsgchannel(Server &server, Client *client, std::vector<std::string> &input) {
    Channel *channel = NULL;
    std::string msg = "";
    int         status;

    status = checkChInput(input, 3);
    if (status) {
        serverReplyofChannelsec(" " + intToStr(status) + " ", " " + client->nickname + " " + input[1] + getErrmsg(status, server), client);
        throw std::invalid_argument(getErrmsg(status, server));
    }
    channel = server.getChannelIfExist(input[1]);
    if (channel == NULL) {
        serverReplyofChannelsec(" 403 ", " " + client->nickname + " " + input[1] + getErrmsg(403, server), client);
        throw std::invalid_argument(ERR_NOSUCHCHANNEL_MSG);
    }
	if (!channel->checkIfMember(client->nickname)) {
        serverReplyofChannelsec(" 442 ", " " + client->nickname + " " + input[1] + getErrmsg(442, server), client);
		throw std::invalid_argument(ERR_NOTONCHANNEL_MSG);
    }

    for (std::vector<std::string>::const_iterator it = input.begin() + 2; it != input.end(); it++) {
        (it + 1) != input.end() ? msg.append(*it + " ") : msg.append(*it); 
		// msg.append(*it + " ");
    }
    // msg.append(*(input.begin() + 2));
    sendMessageTwo(msg, channel, client);
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
        serverReplyofChannelsec(" " + intToStr(status) + " ", " " + client->nickname + " " + chname + getErrmsg(status, server), client);
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
        serverReplyofChannelsec(intToStr(status), " " + client->nickname + " " + chname + getErrmsg(status, server), client);
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

void removeChannelIfnoMember(Server &server, Channel *channel) {
    if (channel->getMembers().size() == 0)
        server.removeChannel(channel);
}

void partUtil(Server &server, Client *client, std::vector<std::string> &input) {
    Channel *channel = NULL;
    int     status;
    std::string chname = "";

    if (input.size() > 1)
        chname = input[1];
    status = checkChInput(input, 2);
    if (status) {
        serverReplyofChannelsec(" " + intToStr(status) + " ", " " + client->nickname + " " + chname + getErrmsg(status, server), client);
        throw std::invalid_argument(getErrmsg(status, server));
    }
    channel = server.getChannelIfExist(input[1]);
    if (channel == NULL) {
        serverReplyofChannelsec(" 403 ", " " + client->nickname + " " + chname + getErrmsg(403, server), client);
        throw std::invalid_argument(ERR_NOSUCHCHANNEL_MSG);
    }
	if (!channel->checkIfMember(client->nickname)) {
        serverReplyofChannelsec(" 442 ", " " + client->nickname + " " + chname + getErrmsg(442, server), client);
		throw std::invalid_argument(ERR_NOTONCHANNEL_MSG);
    }
    std::string reason = getReason(input, 2);
    // build part message here
	// send part message to all users of the channel
    sendMessage(getPartMessage(client, input) + " " + reason , channel);
	channel->removeClient(client);
    removeChannelIfnoMember(server, channel);
	return ;

}

void part(Server &server, Client *client, std::vector<std::string> &input) {
    size_t      i = 0;
    std::vector<std::string> chanames;
    std::vector<std::string>  finalinput;
    std::string reason = "";
    if (input.size() > 1)
        chanames = splitStringTwo(input[1], ',');
    if (input.size() > 2)
        reason = input[2];
    if (chanames.size() > 1) {
        while (i < chanames.size()) {
            finalinput.push_back("PART");
            finalinput.push_back(chanames[i]);
            finalinput.push_back(reason);
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
        serverReplyofChannelsec(" " + intToStr(status) + " ", " " + client->nickname + " " + chname + getErrmsg(status, server), client);
        throw std::invalid_argument(getErrmsg(status, server));
    }
    channel = server.getChannelIfExist(input[1]);
    if (channel == NULL) {
        serverReplyofChannelsec(" 403 ", " " + client->nickname + " " + chname + getErrmsg(403, server), client);
        throw std::invalid_argument(ERR_NOSUCHCHANNEL_MSG);
    }
	if (!channel->checkIfMember(client->nickname)) {
        serverReplyofChannelsec(" 442 ", " " + client->nickname + " " + chname + getErrmsg(442, server), client);
		throw std::invalid_argument(ERR_NOTONCHANNEL_MSG);
    }
	if (input.size() == 2 && channel->getTopic().size() > 0) {
        serverReply(" 332 ", channel->getChName() + " " + channel->getTopic(), client);
		return ;
	}
    status = checkModes(channel, client, input, 't');
    if (status) {
        serverReplyofChannelsec(" " + intToStr(status) + " ", " " + client->nickname + " " + chname + getErrmsg(status, server), client);
		throw std::invalid_argument(ERR_CHANOPRIVSNEEDED_MSG);
    }
	if (input.size() == 2 && channel->getTopic().size() == 0)
    {
        serverReplyofChannel(" 331 ", chname, getErrmsg(331, server), client);
		throw std::invalid_argument(getErrmsg(331, server));
    }
    new_topic = getNewTopic(input);
	channel->setTopic(new_topic);
    sendMessage(getTopicMessage(client, input) + " :" + new_topic , channel);
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
        serverReplyofChannelsec(" " + intToStr(status) + " ", " " + client->nickname + " " + chname + getErrmsg(status, server), client);
        throw std::invalid_argument(getErrmsg(status, server));
    }
    channel = server.getChannelIfExist(input[2]);
    if (channel == NULL) {
        serverReplyofChannelsec(" 403 ", " " + client->nickname + " " + chname + getErrmsg(403, server), client);
        throw std::invalid_argument(ERR_NOSUCHCHANNEL_MSG);
    }

	invitee = server.getClientIfExist(input[1]);
	if (!invitee){
        serverReplyofChannelsec(" 401 ", " " + client->nickname + " " + input[1] + getErrmsg(401, server), client);
        throw std::invalid_argument(ERR_NOSUCHCHANNEL_MSG);
    }
    if (!channel->checkIfMember(client->nickname)){
        serverReplyofChannelsec(" 441 ", " " + client->nickname + " " + chname + getErrmsg(441, server), client);
        throw std::invalid_argument(ERR_USERNOTINCHANNEL_MSG);
    }
	// Check if this client is an operator
	if (!channel->isOperator(client)){
        serverReplyofChannelsec(" 482 ", " " + client->nickname + " " + chname + getErrmsg(482, server), client);
		throw std::invalid_argument((ERR_CHANOPRIVSNEEDED_MSG));
    }
	// Check if invitee is already in channel
	if (channel->checkIfMember(invitee->nickname)){
        serverReplyofChannelsec(" 443 ", " " + client->nickname + " " + chname + getErrmsg(443, server), client);
        throw std::invalid_argument(ERR_USERONCHANNEL_MSG);
    }

	channel->addToInvitation(invitee);
    // serverReply(" 341 ",  ": " + invitee->nickname + getErrmsg(341, server) + channel->getChName(), client);
    serverReply(" 341 ", invitee->nickname + " " + channel->getChName() , client);
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

    if (input.size() > 1)
        chname = input[1];
    status = checkChInput(input, 3);
    if (status) {
        serverReplyofChannelsec(" " + intToStr(status) + " ", " " + client->nickname + " " + chname + getErrmsg(status, server), client);
        throw std::invalid_argument(getErrmsg(status, server));
    }
    channel = server.getChannelIfExist(input[1]);
    if (channel == NULL) {
        serverReplyofChannelsec(" 403 ", " " + client->nickname + " " + chname + getErrmsg(403, server), client);
        throw std::invalid_argument(ERR_NOSUCHCHANNEL_MSG);
    }

	if (!channel->checkIfMember(client->nickname)) {
        // serverReplyofChannel(" 442 ", chname, getErrmsg(442, server), client);
        serverReplyofChannelsec(" 442 ", " " + client->nickname + " " + input[1] + getErrmsg(442, server), client);
		throw std::invalid_argument(ERR_NOTONCHANNEL_MSG);
    }
	if (!channel->isOperator(client)){
        // serverReply(" 482 ", getErrmsg(482, server), client);
        serverReplyofChannelsec(" 482 ", " " + client->nickname + " " + chname + getErrmsg(482, server), client);
		throw std::invalid_argument((ERR_CHANOPRIVSNEEDED_MSG));
    }
	// Check if user to kick exists on server and channel
	toBeRemoved = server.getClientIfExist(input[2]);
	if (!toBeRemoved){
        serverReplyofChannelsec(" 401 ", " " + client->nickname + " " + input[2] + getErrmsg(401, server), client);
		throw std::invalid_argument((ERR_USERNOTINCHANNEL_MSG));
    }

	if (!channel->checkIfMember(toBeRemoved->nickname)){
        // serverReply(" 441 ",  chname + " " + getErrmsg(441, server), client);
        serverReplyofChannelsec(" 441 ", " " + toBeRemoved->nickname + " " + chname + getErrmsg(441, server), client);
		throw std::invalid_argument((ERR_USERNOTINCHANNEL_MSG));
    }
    reason = getReason(input, 3);
	// send remove message to 
    sendMessage(getKickMessage(client, input) + " " + reason , channel);
	channel->removeClient(toBeRemoved);
    removeChannelIfnoMember(server, channel);
	return ;

}


int    tryExtractKeyMode(char sign, char m,  t_ctr & c, std::map<std::string, t_val> & nmodes, std::vector<std::string> &input) {
    t_val modepair;
    if ((sign == '+' || sign == '-') && (m == 'k' && input.size() > c.c)) {
        if (sign == '+') {
             modepair.key = "+k";
            // nmodes["+k"] = input[c];
        }
        else
             modepair.key = "-k";
            // nmodes["-k"] = input[c];
        modepair.value = input[c.c];
        nmodes[intToStr(static_cast<int>(c.idx))] = modepair;
        (c.idx)++;
        (c.c)++;
        // c++;
        return (1);
    }
    else if (sign == '-' && (m == 'k')) {
        // nmodes["-k"] = "";
        modepair.key = "-k";
        modepair.value = "";
        nmodes[intToStr(static_cast<int>(c.idx))] = modepair;
        (c.idx)++;
        return (1);
    }
    return (0);
}

int    tryExtractOprMode(char sign, char m,  t_ctr & c, std::map<std::string, t_val> & nmodes, std::vector<std::string> &input) {
    t_val modepair;
    if ((sign == '+' || sign == '-') && (m == 'o' && input.size() > c.c)) {
        if (sign == '+') {
            modepair.key = "+o";
            // nmodes["+o"] = input[c];
        }
        else {
            modepair.key = "-o";
            // nmodes["-o"] = input[c];
        }
        modepair.value = input[c.c];
        nmodes[intToStr(static_cast<int>(c.idx))] = modepair;
        (c.idx)++;
        (c.c)++;
        // c++;
        return (1);
    }
    return (0);
}

int    tryExtractLimitMode(char sign, char m,  t_ctr & c, std::map<std::string, t_val> & nmodes, std::vector<std::string> &input) {
    t_val modepair;
    if ((sign == '+' || sign == '-') && (m == 'l' && input.size() > c.c)) {
        if (sign == '+') {
            modepair.key = "+l";
            modepair.value = input[c.c];
            // nmodes["+l"] = input[c];
            // c++;
            (c.c)++;
        }
        else {
            // nmodes["-l"] = "";
            modepair.key = "-l";
            modepair.value = "";
        }
        nmodes[intToStr(static_cast<int>(c.idx))] = modepair;
        (c.idx)++;
        return (1);
    }
    return (0);
}

int    tryExtractOtherMode(char sign, char m, t_ctr & c, std::map<char, bool> chmode , std::map<std::string, t_val> & nmodes) {
    std::string newm;
    t_val modepair;
    if ((sign == '+' || sign == '-') && (chmode.find(m) != chmode.end())) {
        newm = "";
        newm = newm + sign + m;
        // nmodes[newm] = "";
        modepair.key = newm;
        modepair.value = "";
        nmodes[intToStr(static_cast<int>(c.idx))] = modepair;
        (c.idx)++;
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


int applyKeyMode(Channel *channel, std::string key, std::string value,  std::string & modes, std::string & values, char & sign) {
    if (key == "+k" && !channel->getmodeAt('k') && value != "") {
        channel->setMode('k', true);
        channel->setChKey(value);
        values = values + " " + value;
        recordMode(modes, sign, '+', "k");
        return (1);
    }
    else if (key == "-k" && channel->getmodeAt('k') && value == "") {
        channel->setMode('k', false);
        values = values + " " + channel->getChKey();
        channel->setChKey("");
        recordMode(modes, sign, '-', "k");
        return (1);
    }
    else if (key == "-k" && channel->getmodeAt('k') && value != "") {
        if (value != channel->getChKey())
            return (3);
        channel->setMode('k', false);
        channel->setChKey("");
        values = values + " " + value;
        recordMode(modes, sign, '-', "k");
        return (1);
    }
    return (0);
}

int applyOprMode(Server &server, Channel *channel, std::string key, std::string nick, std::string & modes, std::string & values, char & sign) {
    Client * newclient = server.getClientIfExist(nick);

    if ((key == "+o" || key == "-o") &&  !newclient)
        return(401);
    if (key == "+o" && (nick != "" && channel->checkIfMember(nick) && !channel->isNickIsAdmin(nick))) {
        channel->addAdmin(newclient);
        values = values + " " + nick;
        recordMode(modes, sign, '+', "o");
        return (1);
    }
    else if (key == "-o" && channel->checkIfMember(nick) && channel->isNickIsAdmin(nick)) {
        channel->removeAddmin(newclient);
        values = values + " " + nick;
        recordMode(modes, sign,'-', "o");
        return (1);
    }
    return (0);
}

int applyLimitMode(Server &server, Channel *channel, std::string key, std::string limit, std::string &modes, std::string & values, char & sign) {
    (void) server;
    if (key == "+l" && (limit != "")) {
        size_t value = static_cast<size_t>(std::strtoll((limit.c_str()), NULL, 10));
        channel->setMode('l', true);
        channel->setChLimit(value);
        values = values + " " + intToStr(static_cast<int>(value));
        recordMode(modes, sign, '+', "l");
        return (1);
    }
    else if (key == "-l") {
        channel->setMode('l', false);
        recordMode(modes, sign,'-', "l");
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


void tryApplyMode(Server &server, Client *client, Channel *channel, std::vector<std::string> &input, std::map<std::string, t_val>  nmodes) {
    std::map<std::string, t_val>::iterator cit = nmodes.begin();
    std::string appliedmode = "";
    std::string values = "";
    char sign = 'n';
    int res = 0;
    for (; cit != nmodes.end(); cit++) {
        std::cout << cit->second.key << " - " << cit->second.value << std::endl;
        if ((res = applyOprMode(server, channel, cit->second.key, cit->second.value, appliedmode, values, sign)) & 1) {
            if (res > 1)
                serverReplyofChannelsec(" " + intToStr(res) + " ", " " + client->nickname + " " + cit->second.value + getErrmsg(res, server), client);
            continue;
        }
        else if ((res = applyKeyMode(channel, cit->second.key, cit->second.value, appliedmode, values, sign)) & 1) {
            if (res == 3)
                serverReplyofChannelsec(ERR_KEYSET, " " + client->nickname + " " + channel->getChName() + getErrmsg(467, server), client);
            continue;
        }
        else if (applyLimitMode(server, channel, cit->second.key, cit->second.value, appliedmode, values, sign))
            continue;
        else if (applyInviteMode(server, channel, cit->second.key, cit->second.value, appliedmode, sign))
            continue;
        else if (applyTopicMode(server, channel, cit->second.key, cit->second.value, appliedmode, sign))
            continue;
    }
    if (appliedmode.size() != 0) {
        appliedmode += " " + values;
        sendMessage(getModeMessageTwo(client, input[1], appliedmode), channel);
    }
}

void modeUtils(Server &server, Client *client, Channel *channel, std::vector<std::string> &input) {
    std::string trimed = trimChars(input[2], " \n\r ");
    std::map<std::string, t_val>  newmodes;
    std::map<char, bool> chmode = channel->getMode();
    // size_t  counter = 3;
    t_ctr counter;
    counter.c = 3;
    counter.idx = 0;
    int     c = 0;
    char    sign = trimed[0];

    for (std::string::iterator it = trimed.begin() + 1; it != trimed.end(); it++) {
        if (std::isspace(*it))
            continue;
        c++;
        if ((sign == '+' || sign == '-') && (*it == 'o' && input.size() <= counter.c))
            serverReplyofChannel(ERR_NEEDMOREPARAMS, channel->getChName() , getErrmsg(461, server), client);
        else if (sign == '+' && ((*it == 'k' || *it == 'l') && input.size() <= counter.c))
            serverReplyofChannel(ERR_NEEDMOREPARAMS, channel->getChName() , getErrmsg(461, server), client);
        else if (tryExtractOprMode(sign, *it, counter, newmodes, input))
            continue;
        else if (tryExtractKeyMode(sign, *it, counter, newmodes, input))
            continue;
        else if (tryExtractLimitMode(sign, *it, counter, newmodes, input))
            continue;
        else if (tryExtractOtherMode(sign, *it, counter, chmode, newmodes))
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
        serverReplyofChannelsec(" " + intToStr(status) + " ", " " + client->nickname + " " + chname + getErrmsg(status, server), client);
        throw std::invalid_argument(getErrmsg(status, server));
    }
    channel = server.getChannelIfExist(input[1]);
    if (channel == NULL) {
        serverReplyofChannelsec(" 403 ", " " + client->nickname + " " + chname + getErrmsg(403, server), client);
        throw std::invalid_argument(ERR_NOSUCHCHANNEL_MSG);
    }

	if (!channel->checkIfMember(client->nickname)) {
        // serverReplyofChannel(" 442 ", chname, getErrmsg(442, server), client);
        serverReplyofChannelsec(" 442 ", " " + client->nickname + " " + input[1] + getErrmsg(442, server), client);
		throw std::invalid_argument(ERR_NOTONCHANNEL_MSG);
    }
    if (input.size() == 2) {
        return (serverReply(" 324 ", chname + " " + getMofchannel(channel), client));
    }
	if (!channel->isOperator(client)){
        // serverReply(" 482 ", getErrmsg(482, server), client);
        serverReplyofChannelsec(" 482 ", " " + client->nickname + " " + chname + getErrmsg(482, server), client);
		throw std::invalid_argument((ERR_CHANOPRIVSNEEDED_MSG));
    }
    modeUtils(server, client, channel, input);
	return ;
}
