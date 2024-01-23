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

bool allDigits(std::string str) {
    if (str.size() == 1 &&  (str.at(0) == '+' || str.at(0) == '-'))
        return (false);
    for (std::string::const_iterator it = str.begin(); it != str.end(); it++) {
		if (it == str.begin() &&  *it == '+')
			continue;
		else if (it == str.begin() && (*it == '-'))
			return (false);
		else if (!std::isdigit(*it) && *it != '.') {
			return (false);
		}
	}
    if (std::strtol(str.c_str(), NULL, 10) == 0) {
        return (false);
    } 
	return (true); 
}

std::vector<std::string> splitStringTwo(std::string str, char delim) {
    std::vector<std::string> res;
    std::string token = "";
    bool flag = true;
    for (size_t i = 0; i < str.size(); i++) {
        flag = (str[i] != delim) ? false : true;
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


int checkChannelName(std::string & chname) {
    if (chname.find_first_of(" , :\n\r\a\0") != std::string::npos)
        return (403);
    if (chname.size() > 50)
        return (403);
    if (chname.at(0) != '#' || chname.size() < 2)
        return(403);
    return (0);
}

int checkChInput(std::vector<std::string> & input, size_t minpars) {
    if (input.size() < minpars)
        return (461); // Invalid number of parameters
    if (minpars == 3 && input[0] == "INVITE")
        return (checkChannelName(input[2]));
    return(checkChannelName(input[1]));
}

int checkModes(Channel * channel, Client * client, std::vector<std::string> & input, char checkfor) {
    if (checkfor == 'i')
    {
        if (channel->getmodeAt('i') && channel->isInvited(client))
            return (0);
        if ((channel->getmodeAt('k') && input.size() < 3))
            return (475);
        if ((channel->getmodeAt('k') && input.size() >= 3) && channel->getChKey().compare(input[2]))
            return (475);
        if (channel->getmodeAt('i') && !channel->isInvited(client))
            return (473);
        if (channel->getmodeAt('l') && (channel->getNickNames(0).size() >= channel->getChLimit()))
            return (471);

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

std::string getQuitMessage(Client * client) {
    std::string res;
    res = ":" + client->nickname + "!" + client->username + "@" + client->client_ip \
              + " QUIT " + "Quit: leaving";
    return (res);
}

std::string getNewTopic(std::vector<std::string> & input) {
    std::string new_topic = "";
	for (std::vector<std::string>::iterator it = input.begin() + 2; it != input.end(); it++) {
		(it + 1) != input.end() ? new_topic.append(*it + " ") : new_topic.append(*it);
    }
    if (new_topic.size() == 0) {
        return new_topic;
	}
	// (new_topic.at(0) != ':') ? new_topic.insert(0, ":") : new_topic.insert(0, "");
	// clear new_topic if only ":"
	if (new_topic.size() == 1) {
		new_topic = "";
	}
    return (new_topic);
}

std::string getReason(std::vector<std::string> & input, size_t minpar) {
    std::string reason = "";

    if (input.size() < minpar + 1) {
        return (reason);
	}
	for (std::vector<std::string>::iterator it = input.begin() + minpar; it != input.end(); it++)
		(it + 1) != input.end() ? reason.append(*it + " ") : reason.append(*it);
	if (reason.at(0) != ':') {
		reason.insert(0, ":");
	}
	// clear reason if only ":"
	if (reason.size() == 1) {
		reason = "";
	}
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

    res = ":" + client->nickname + " " + "INVITE" + " " + input[1] + " " + input[2] + " :" + client->nickname + " is inviting you to join " + input[2];
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
    std::string values = "";

    if (channel->getmodeAt('i'))
        res += 'i';
    if (channel->getmodeAt('k')) {
        res += 'k';
        values = values + " " + channel->getChKey();
    }
    if (channel->getmodeAt('l')) {
        res += 'l';
        values = values + " " + intToStr(static_cast<int>(channel->getChLimit()));
    }
    if (channel->getmodeAt('t'))
        res += 't';
    return (res + values);
}

void    sendMessage(std::string const &msg, Channel * channel) {
    std::vector<Client *> members = channel->getMembers();

    std::vector<Client *>::iterator it = members.begin();

    for (; it != members.end(); ++it) {
      // send the message to the client;
      Client * newcl = *it;
      std::string response = msg + "\r\n";
      if (send(newcl->fd, response.c_str(), response.length(), 0) == -1)
        std::cout << "ERROR: " << strerror(errno) << std::endl;
    //   send(newcl->fd, response.c_str(), msg.length(), 0);

    //   sendResponse(msg, newcl);
    //   sendMsg((*it)->fd, msg);
    }
}

void UserToUserMessageChannel(std::string message, Client *src, Client *dst, Channel *channel) {
	std::string msg = ":" + src->nickname + "!" + src->username + "@" + src->hostname + " PRIVMSG ";
	msg += channel->getChName() + " :" + message + "\r\n";
	int rc = send(dst->fd, msg.c_str(), msg.length(), 0);
	if (rc == -1) {
		std::cout << "ERROR: " << strerror(errno) << std::endl;
		return;
	}
}

void UserToUserMessageChannel2(std::string message, Client *src, Client *dst, Channel *channel) {
	std::string msg = ":" + src->nickname + "!" + src->username + "@" + src->hostname + " NOTICE ";
	msg += channel->getChName() + " :" + message + "\r\n";
	int rc = send(dst->fd, msg.c_str(), msg.length(), 0);
	if (rc == -1) {
		std::cout << "ERROR: " << strerror(errno) << std::endl;
		return;
	}
}

void    sendMessageTwo(std::string const &msg, Channel * channel, Client *sender) {
    std::vector<Client *> members = channel->getMembers();

    std::vector<Client *>::iterator it = members.begin();

    for (; it != members.end(); ++it) {
        Client * newcl = *it;
        if (newcl != sender)
            UserToUserMessageChannel(msg, sender , newcl, channel);
    }
}

void    sendMessageThree(std::string const &msg, Channel * channel, Client *sender) {
    std::vector<Client *> members = channel->getMembers();

    std::vector<Client *>::iterator it = members.begin();

    for (; it != members.end(); ++it) {
        Client * newcl = *it;
        if (newcl != sender)
            UserToUserMessageChannel2(msg, sender , newcl, channel);
    }
}