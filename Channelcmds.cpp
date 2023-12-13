# include "./includes/Channelcmds.hpp"


// std::string chcmds[] =  {"JOIN", "PART", "MODE", "TOPIC", "INVITE", "NAMES", "KICK", "LIST"};

void   printchannelmembers(Channel * channel) {
    std::vector<Client *> member = channel->getMembers();
    std::vector<Client *>::iterator it = member.begin();
    std::cout << "[######### members = [ ";
    for(; it != member.end(); ++it) {
        std::cout << (*it)->nickname  << " with fd " << (*it)->fd << " - ";
    }
    std::cout << "] #########]" << std::endl;
}

void   printVector(std::vector<std::string> & input) {
    std::vector<std::string>::iterator it = input.begin();
    std::cout << "[######### inputs = [ ";
    for(; it != input.end(); ++it) {
        std::cout << *it  <<" - ";
    }
    std::cout << "] #########]" << std::endl;
}


std::vector<std::string> splitStringTwo(std::string str, char delim) {
    std::vector<std::string> res;
    std::string token = "";
    for (size_t i = 0; i < str.size(); i++) {
        bool flag = true;
        if (str[i] != delim ) 
            flag = false;
        if (flag) {
            if (token.size() > 0) {
                res.push_back(token);
                token = "";
                // i += 1;
            }
        } else {
            token += str[i];
        }
    }
    res.push_back(token);
    return res;
}

std::string getErrmsg(int code, Server &server ) {
    std::string err = server._errors._errmsgs[code];
    return (err);
}


int checkChannelName(std::string & chname, int *status) {
    if (chname.find_first_of(" , :\n\r\a\0") != std::string::npos)
        return (*status = 403, 2);
    if (chname.size() > 50)
        return (*status = 403, 3);
    if (chname.at(0) != '#' || chname.size() < 2)
        return(*status = 403, 4);
    return (0);
}

int checkChInput(std::vector<std::string> & input, size_t minpars) {
    int status;

    if (input.size() < minpars)
        return (461); // Invalid number of parameters
    if (checkChannelName(input[1], &status))
        return (status); // bad channel name
    return (0); 
}

int checkModes(Channel * channel, Client * client, std::vector<std::string> & input, char checkfor) {
    if (checkfor == 'i')
    {
        if (channel->getmodeAt('i') && !channel->isInvited(client))
            return (473);
        if (channel->getmodeAt('l') && (channel->getNickNames().size() >= channel->getChLimit()))
            return (471);
        if ((channel->getmodeAt('k') && input.size() < 3) && channel->getChKey().compare(input[2]))
            return (475);
    }
    else if (checkfor == 't') {
        if (channel->getmodeAt('t') && !channel->isOperator(client))
		    return(8);
    }
    return (0);
}

std::string getJoinMessage(Client * client, std::vector<std::string> & input) {
    std::string res;

    res = ":" + client->nickname + "!" + client->username + "@" + client->client_ip \
              + " JOIN " + input[1]; 
    return (res);
}


void    sendMessage(std::string const &msg, Channel * channel) {
    std::vector<Client *> members = channel->getMembers();
    
    std::vector<Client *>::iterator it = members.begin();

    for (; it != members.end(); ++it) {
      // send the message to the client;
      Client * newcl = *it;
      sendResponse(msg, newcl);
    //   sendMsg((*it)->fd, msg);
    }
}

// send message regarding channel to newly joined client
void names(Server &server, Client *client, Channel *channel) {
    (void)server;
	std::string messageOne = ":ircserv 332 " + client->nickname + " = " + channel->getChName() + " :";
    messageOne = messageOne + channel->getTopic();
	std::string messageTwo = ":ircserv 353 " + client->nickname + " = " + channel->getChName() + " :";
	std::vector<std::string> nicknames = channel->getNickNames();
	std::string messageThree = "";
	for (std::vector<std::string>::iterator it = nicknames.begin(); it != nicknames.end(); it++)
			messageTwo += (*it)+ " ";
	messageThree = ":ircserv 366 " + client->nickname + " " + channel->getChName() + " :End of NAMES list\r\n";

    // send all three messages to client;
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

void   sendTopicAndMembers(Channel *channel, Client *client) {
    std::string topic = channel->getChName();
    topic = topic + " :" + channel->getTopic();
    std::string names = "= " + channel->getChName() + " :";
    std::vector<std::string> nicknames = channel->getNickNames();
    serverReply("332", topic, client);
    for(std::vector<std::string>::iterator it = nicknames.begin(); it != nicknames.end(); ++it) {
        names = names + *it + " ";
    }
    serverReply("353", names, client);
    serverReply("366", "END of NAMES list", client);
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
    std::cout << "here0" << std::endl;
    if (status) {
        std::cout << "here1" << std::endl;
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
    sendTopicAndMembers(channel, client);
}

void popElementsOfVector(std::vector<std::string> &input) {
    while (input.size())
        input.pop_back();
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
        // std::cout << "HERE\n" << std::endl;
        // printVector(input);
        // joinUtil(server, client, input);
        try {
            joinUtil(server, client, input);
        } catch(const std::exception& e) {
            std::cerr << e.what() << '\n';
        }
    }
}


void part(Server &server, Client *client, std::vector<std::string> &input) {
    Channel *channel;
    int     status;

    status = checkChInput(input, 2);
    if (status)
        throw std::invalid_argument(getErrmsg(status, server));
    channel = server.getChannelIfExist(input[2]);
    if (channel == NULL)
        throw std::invalid_argument(ERR_NOSUCHCHANNEL_MSG);

	if (!channel->checkIfMember(client->nickname))
		throw std::invalid_argument(ERR_NOTONCHANNEL_MSG);

    // build part message here
	// send part message to all users of the channel
	channel->removeClient(client);
	return ;

}


void topic(Server &server, Client *client, std::vector<std::string> &input) {
	Channel *channel = NULL;
    int         status;
    std::string new_topic;

    status = checkChInput(input, 3);
    if (status)
        throw std::invalid_argument(getErrmsg(status, server));
    channel = server.getChannelIfExist(input[2]);
    if (channel == NULL)
        throw std::invalid_argument(ERR_NOSUCHCHANNEL_MSG);
	if (!channel->checkIfMember(client->nickname))
		throw std::invalid_argument(ERR_NOTONCHANNEL_MSG);

    status = checkModes(channel, client, input, 't');
    if (status)
        throw std::invalid_argument(getErrmsg(status, server));
	if (input.size() == 3 && channel->getTopic().size() == 0)
		throw std::invalid_argument("topic size invalid");
	if (input.size() == 3 && channel->getTopic().size() > 0) {
        //channel message to client and to members 
		return ;
	}
	// Extract the new topic from the input
	for (std::vector<std::string>::const_iterator it = input.begin() + 3; it != input.end(); it++)
		new_topic.append(*it + " ");
	if (new_topic.at(0) != ':')
		new_topic.insert(0, ":");
	// clear new_topic if only ":"
	if (new_topic.size() == 1)
		new_topic = "";
	// Set the new topic
	channel->setTopic(new_topic);
	// send a confirmation message
	return ; 
}


void invite(Server &server, Client *client, std::vector<std::string> &input) {

	Client *invitee;
	Channel *channel;
    int      status;

    status = checkChInput(input, 3);
    if (status)
        throw std::invalid_argument(getErrmsg(status, server));
    channel = server.getChannelIfExist(input[3]);
    if (channel == NULL)
        throw std::invalid_argument(ERR_NOSUCHCHANNEL_MSG);

	invitee = server.getClientIfExist(input[2]);
	if (!invitee)
		throw std::invalid_argument((ERR_NOSUCHNICK_MSG));
	// Check if this client is an operator
	if (!channel->isOperator(client))
		throw std::invalid_argument((ERR_CHANOPRIVSNEEDED_MSG));
	// Check if invitee is already in channel
	// if (channel->isMember(invitee->nickname))
	// 	throw std::invalid_argument(("Client is already member of the channnel");
	if (channel->checkIfMember(invitee->nickname))
		throw std::invalid_argument("Client is already member of the channnel");
	channel->addToInvitation(invitee);
    // all invitation messages here
	return ;
}


void kick(Server &server, Client *client, std::vector<std::string> &input) {

	Channel *channel;
	Client *toBeRemoved = NULL;
    int      status;

    status = checkChInput(input, 3);
    if (status)
        throw std::invalid_argument(getErrmsg(status, server));
    channel = server.getChannelIfExist(input[3]);
    if (channel == NULL)
        throw std::invalid_argument(ERR_NOSUCHCHANNEL_MSG);
	if (!channel->checkIfMember(client->nickname))
		throw std::invalid_argument((ERR_NOTONCHANNEL_MSG));
	if (!channel->isOperator(client))
		throw std::invalid_argument((ERR_CHANOPRIVSNEEDED_MSG));
	// Check if user to kick exists on server and channel
	toBeRemoved = server.getClientIfExist(input[3]);
	if (!toBeRemoved || !channel->checkIfMember(toBeRemoved->nickname))
		throw std::invalid_argument((ERR_USERNOTINCHANNEL_MSG));
	// send remove message to 
	channel->removeClient(toBeRemoved);
	return ;

}