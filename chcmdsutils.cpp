# include "./includes/Channelcmds.hpp"


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

std::string intToStr(int nbr) {
    std::stringstream tmp;  
    tmp << nbr;      
    std::string str;          
    tmp >> str;
    return ( "" + str + ""); 
}

static std::string leftTrim(const std::string &s, const std::string tobetrimed) {
    size_t start = s.find_first_not_of(tobetrimed);
    return (start == std::string::npos) ? "" : s.substr(start);
}
 
static std::string rightTrim(const std::string &s, const std::string tobetrimed) {
    size_t end = s.find_last_not_of(tobetrimed);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trimChars(std::string str, const std::string tobetrimed) {
	return (rightTrim(leftTrim(str, tobetrimed), tobetrimed));	
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
    if (minpars == 3 && input[0] == "INVITE")
        checkChannelName(input[2], &status);
    else if (checkChannelName(input[1], &status))
    {
        std::cout << "[ " << input[1] << " - " << status << " ]" << std::endl;
        return (status); // bad channel name
    }
    return (0); 
}

int checkModes(Channel * channel, Client * client, std::vector<std::string> & input, char checkfor) {
    if (checkfor == 'i')
    {
        if (channel->getmodeAt('i') && !channel->isInvited(client))
            return (473);
        if (channel->getmodeAt('i') && channel->isInvited(client))
            return (0);
        if (channel->getmodeAt('l') && (channel->getNickNames(0).size() >= channel->getChLimit()))
            return (471);
        if ((channel->getmodeAt('k') && input.size() < 3))
            return (475);
        if ((channel->getmodeAt('k') && input.size() >= 3) && channel->getChKey().compare(input[2]))
            return (475);
        
    }
    else if (checkfor == 't') {
        if (channel->getmodeAt('t') && !channel->isOperator(client))
		    return(482);
    }
    return (0);
}

std::string getJoinMessage(Client * client, std::vector<std::string> & input) {
    std::string res;

    res = ":" + client->nickname + "!" + client->username + "@" + client->client_ip \
              + " JOIN " + input[1]; 
    return (res);
}

std::string getNewTopic(std::vector<std::string> & input) {
    std::string new_topic = "";

	for (std::vector<std::string>::iterator it = input.begin() + 2; it != input.end(); it++)
		new_topic.append(*it + " ");
	if (new_topic.at(0) != ':')
		new_topic.insert(0, ":");
	// clear new_topic if only ":"
	if (new_topic.size() == 1)
		new_topic = "";
    return (new_topic);
}

std::string getReason(std::vector<std::string> & input) {
    std::string reason = "";
    if (input.size() < 4)
        return (reason);
	for (std::vector<std::string>::iterator it = input.begin() + 3; it != input.end(); it++)
		reason.append(*it + " ");
	if (reason.at(0) != ':')
		reason.insert(0, ":");
	// clear reason if only ":"
	if (reason.size() == 1)
		reason = "";
    return (reason);
}

std::string getPartMessage(Client * client, std::vector<std::string> & input) {
    std::string res;

    res = ":" + client->nickname + "!" + client->username + "@" + client->client_ip \
              + " PART " + input[1]; 
    return (res);
}

std::string getTopicMessage(Client * client, std::vector<std::string> & input) {
    std::string res;

    res = ":" + client->nickname + "!" + client->username + "@" + client->client_ip \
              + " TOPIC " + input[1] ; 
    return (res);
}

std::string getInviteMessage(Client * client, std::vector<std::string> & input) {
    std::string res;

    // res = ":" + client->nickname + "!" + client->username + "@" + client->client_ip \
    //           + " INVITE " + input[1] + " " + input[2]; 
    // return (res);
    
    res = ":" + client->nickname + " " + "INVITE" + " " + input[1] + " " + input[2]; 
    return (res);
}

std::string getKickMessage(Client * client, std::vector<std::string> & input) {
    std::string res;

    res = ":" + client->nickname + "!" + client->username + "@" + client->client_ip \
              + " KICK " + input[1] + " " + input[2]; 
    return (res);
}

std::string getModeMessage(Client * client, std::vector<std::string> & input) {
    std::string res;

    res = ":" + client->nickname + "!" + client->username + "@" + client->client_ip \
              + " MODE " + input[1] + " " + input[2]; 
    return (res);
}

std::string getModeMessageTwo(Client * client, std::string chname , std::string & input) {
    std::string res;

    res = ":" + client->nickname + "!" + client->username + "@" + client->client_ip \
              + " MODE "  + chname  + " :" + input; 
    return (res);
}

std::string getMofchannel(Channel * channel) {
    std::string res  = "+";

    if (channel->getmodeAt('k'))
        res += 'k';
    if (channel->getmodeAt('i'))
        res += 'i';
    if (channel->getmodeAt('t'))
        res += 't';
    if (channel->getmodeAt('l'))
        res += 'l';
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