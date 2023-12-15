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
        serverReply("332", topic, client);
    serverReply("353", names, client);
    serverReply("366",  channel->getChName() + " :END of /NAMES list", client); 
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

    if (input.size() > 1)
        chname = input[1];
    status = checkChInput(input, 2);
    if (status) {
        // serverReply(std::to_string(status), getErrmsg(status, server), client);
        serverReplyofChannel(std::to_string(status), chname, getErrmsg(status, server), client);
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
        serverReplyofChannel(std::to_string(status), chname, getErrmsg(status, server), client);
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
        serverReplyofChannel(std::to_string(status), chname, getErrmsg(status, server), client);
        throw std::invalid_argument(getErrmsg(status, server));
    }
    if (channel->checkIfMember(client->nickname))
        return ;
    channel->addClient(client);
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
        serverReplyofChannel(std::to_string(status), chname, getErrmsg(status, server), client);
        throw std::invalid_argument(getErrmsg(status, server));
    }
    channel = server.getChannelIfExist(input[1]);
    if (channel == NULL) {
        serverReplyofChannel("403", chname, getErrmsg(403, server), client);
        throw std::invalid_argument(ERR_NOSUCHCHANNEL_MSG);
    }
	if (!channel->checkIfMember(client->nickname)) 
    {
        serverReplyofChannel("442", chname, getErrmsg(442, server), client);
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
        serverReplyofChannel(std::to_string(status), chname, getErrmsg(status, server), client);
        throw std::invalid_argument(getErrmsg(status, server));
    }
    channel = server.getChannelIfExist(input[1]);
    if (channel == NULL) {
    
        serverReplyofChannel("403", chname, getErrmsg(403, server), client);
        throw std::invalid_argument(ERR_NOSUCHCHANNEL_MSG);
    }
	if (!channel->checkIfMember(client->nickname)) {
    
        serverReplyofChannel("442", chname, getErrmsg(442, server), client);
		throw std::invalid_argument(ERR_NOTONCHANNEL_MSG);
    }
    status = checkModes(channel, client, input, 't');
    if (status) {
        serverReplyofChannel("482", chname, getErrmsg(482, server), client);
		throw std::invalid_argument(ERR_CHANOPRIVSNEEDED_MSG);
    }
	if (input.size() == 2 && channel->getTopic().size() == 0)
    {
        serverReplyofChannel("331", chname, getErrmsg(331, server), client);
		throw std::invalid_argument(getErrmsg(331, server));
    }
	if (input.size() == 2 && channel->getTopic().size() > 0) {
        serverReply("332", channel->getChName() + " " + channel->getTopic(), client);
		return ;
	}
    new_topic = getNewTopic(input);
	channel->setTopic(new_topic);
    sendMessage(getTopicMessage(client, input) + new_topic , channel);
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
        serverReplyofChannel(std::to_string(status), chname, getErrmsg(status, server), client);
        throw std::invalid_argument(getErrmsg(status, server));
    }
    channel = server.getChannelIfExist(input[2]);
    if (channel == NULL) {
    
        serverReplyofChannel("403", chname, getErrmsg(403, server), client);
        throw std::invalid_argument(ERR_NOSUCHCHANNEL_MSG);
    }

	invitee = server.getClientIfExist(input[1]);
	if (!invitee){
        serverReply("401", getErrmsg(401, server), client);
        throw std::invalid_argument(ERR_NOSUCHCHANNEL_MSG);
    }
	// Check if this client is an operator
	if (!channel->isOperator(client)){
        serverReply("482", getErrmsg(482, server), client);
		throw std::invalid_argument((ERR_CHANOPRIVSNEEDED_MSG));
    }
	// Check if invitee is already in channel
	if (channel->checkIfMember(invitee->nickname)){
        serverReply("443", getErrmsg(443, server), client);
        throw std::invalid_argument(ERR_USERONCHANNEL_MSG);
    }
	channel->addToInvitation(invitee);
    serverReply("341",  ": " + invitee->nickname + getErrmsg(341, server) + channel->getChName(), client);
    serverReply("341",   ": " + client->nickname + " Is inviting you to join the channel " + channel->getChName(), invitee);
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
        serverReplyofChannel(std::to_string(status), chname, getErrmsg(status, server), client);
        throw std::invalid_argument(getErrmsg(status, server));
    }
    channel = server.getChannelIfExist(input[1]);
    if (channel == NULL) {
    
        serverReplyofChannel("403", chname, getErrmsg(403, server), client);
        throw std::invalid_argument(ERR_NOSUCHCHANNEL_MSG);
    }

	if (!channel->checkIfMember(client->nickname)) {
        serverReplyofChannel("442", chname, getErrmsg(442, server), client);
		throw std::invalid_argument(ERR_NOTONCHANNEL_MSG);
    }
	if (!channel->isOperator(client)){
        serverReply("482", getErrmsg(482, server), client);
		throw std::invalid_argument((ERR_CHANOPRIVSNEEDED_MSG));
    }
	// Check if user to kick exists on server and channel
	toBeRemoved = server.getClientIfExist(input[2]);
	if (!toBeRemoved || !channel->checkIfMember(toBeRemoved->nickname)){
        serverReply("441",  chname + " " + getErrmsg(441, server), client);
		throw std::invalid_argument((ERR_USERNOTINCHANNEL_MSG));
    }
    reason = getReason(input);
	// send remove message to 
    sendMessage(getKickMessage(client, input) + " :" + reason , channel);
	channel->removeClient(toBeRemoved);
	return ;

}

char extractSign(std::string input) {
    return (input[0]);

}

char extractMode(std::string input) {
    return (input[1]);
}

void mode(Server &server, Client *client, std::vector<std::string> &input) {

	Channel *channel;
    int      status;
    std::string chname = "";
    std::string trimed = "";
    char        mode;
    char        sign;

    if (input.size() > 2)
        chname = input[1];
    status = checkChInput(input, 2);
    if (status) {
        serverReplyofChannel(std::to_string(status), chname, getErrmsg(status, server), client);
        throw std::invalid_argument(getErrmsg(status, server));
    }
    channel = server.getChannelIfExist(input[1]);
    if (channel == NULL) {
    
        serverReplyofChannel("403", chname, getErrmsg(403, server), client);
        throw std::invalid_argument(ERR_NOSUCHCHANNEL_MSG);
    }

	if (!channel->checkIfMember(client->nickname)) {
        serverReplyofChannel("442", chname, getErrmsg(442, server), client);
		throw std::invalid_argument(ERR_NOTONCHANNEL_MSG);
    }
	if (!channel->isOperator(client)){
        serverReply("482", getErrmsg(482, server), client);
		throw std::invalid_argument((ERR_CHANOPRIVSNEEDED_MSG));
    }
    trimed = trimChars(input[2], " \n\r ");
    sign = extractSign(trimed);
    mode = extractMode(trimed);
    if (sign == '+' && !channel->getmodeAt(mode))
        channel->setMode(mode, true);
    else if (sign == '-' && channel->getmodeAt(mode))
        channel->setMode(mode, false);
    // delete &trimed;
	// send remove message to 
    sendMessage(getModeMessage(client, input), channel);
	return ;
}



