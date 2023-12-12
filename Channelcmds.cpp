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

std::string getErrmsg(int code) {
    std::string err = "error happened";
    (void) code;
    return (err);
}

void    sendMsg(int fd, std::string const & msg) {
    int ret = send(fd, msg.c_str(), msg.length(), 0);
	if (ret == -1)
		std::cerr << "ERROR: " << strerror(errno) << std::endl;
}

int checkChannelName(std::string & chname, int *status) {
    if (chname.find_first_of(" , :\n\r\a\0") != std::string::npos)
        return (*status = 2, 2);
    if (chname.size() > 50)
        return (*status = 3, 3);
    if (chname.at(0) != '#' || chname.size() < 2)
        return(*status = 4, 4);
    return (0);
}

int checkChInput(std::vector<std::string> & input) {
    int status;

    if (input.size() < 2)
        return (1); // Invalid number of parameters
    if (checkChannelName(input[1], &status))
        return (status); // bad channel name
    return (0); 
}

int checkModes(Channel * channel, Client * client, std::vector<std::string> & input, char checkfor) {
    if (checkfor == 'i')
    {
        if (channel->getmodeAt('i') && !channel->isInvited(client))
            return (5);
        if (channel->getmodeAt('l') && (channel->getNickNames().size() >= channel->getChLimit()))
            return (6);
        if ((channel->getmodeAt('k') && input.size() < 3) && channel->getChKey().compare(input[2]))
            return (7);
        
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

    status = checkChInput(input);
    if (status)
        throw std::invalid_argument(getErrmsg(status));
    channel = server.getChannelIfExist(input[1]);
    if (channel == NULL) {
        throw std::invalid_argument(ERR_NOSUCHNICK_MSG);
    }
    if (!channel->checkIfMember(client->nickname))
        throw std::invalid_argument(ERR_NOTONCHANNEL_MSG);
    for (std::vector<std::string>::const_iterator it = input.begin() + 2; it != input.end(); it++) {
		msg.append(*it + " ");
    }
    std::cout << "[########### successfully added user to channel ###########]" << std::endl;
    std::cout << "[########### " << getJoinMessage(client, input)  << " ###########]" << std::endl;
    printchannelmembers(channel);
    sendMessage(msg, channel);
}

void join(Server &server, Client *client, std::vector<std::string> &input) {
    Channel *channel = NULL;
    std::string     chkey = "";
    std::string pas;
    int         status;

    status = checkChInput(input);
    if (status)
    {
        std::cout << "[###########status =  " << status << "]" << std::endl;
        throw std::invalid_argument(getErrmsg(status));
    }
    channel = server.getChannelIfExist(input[1]);
    if (channel == NULL) {
        channel = new Channel(input[1], chkey);
        server.addChannel(channel);
        channel->addAdmin(client);
    }
    status = checkModes(channel, client, input, 'i');
    if (status)
    {
        std::cout << "[###########status =  " << status << "]" << std::endl;
        throw std::invalid_argument(getErrmsg(status));
    }
    // if (channel->getNickNames().find(client->nickname()) != channel->getNickNames().end())
    //     return ;
    if (channel->checkIfMember(client->nickname))
        return ;
    channel->addClient(client);
    std::cout << "[########### successfully added user to channel ###########]" << std::endl;
    std::cout << "[########### " << getJoinMessage(client, input)  << " ###########]" << std::endl;
    printchannelmembers(channel);
    sendMessage(getJoinMessage(client, input), channel);
}


void part(Server &server, Client *client, std::vector<std::string> &input) {
    Channel *channel;
    int     status;

    status = checkChInput(input);
    if (status)
        throw std::invalid_argument(getErrmsg(status));
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

    status = checkChInput(input);
    if (status)
        throw std::invalid_argument(getErrmsg(status));
    channel = server.getChannelIfExist(input[2]);
    if (channel == NULL)
        throw std::invalid_argument(ERR_NOSUCHCHANNEL_MSG);
	if (!channel->checkIfMember(client->nickname))
		throw std::invalid_argument(ERR_NOTONCHANNEL_MSG);

    status = checkModes(channel, client, input, 't');
    if (status)
        throw std::invalid_argument(getErrmsg(status));
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

    status = checkChInput(input);
    if (status)
        throw std::invalid_argument(getErrmsg(status));
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

    status = checkChInput(input);
    if (status)
        throw std::invalid_argument(getErrmsg(status));
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