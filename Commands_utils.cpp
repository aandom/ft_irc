#include "includes/Commands_utils.hpp"

void sendResponse(std::string message, Client *client) {
	std::string response = message + "\r\n";
	int ret = send(client->fd, response.c_str(), response.length(), 0);
	if (ret == -1)
		std::cout << "ERROR: " << strerror(errno) << std::endl;
}

void sendResponse1(std::string message, Client *client) {
	std::string response = message + "\r\n";
	int ret = send(client->fd, response.c_str(), response.length(), 0);
	if (ret == -1)
		std::cout << "ERROR: " << strerror(errno) << std::endl;
}

void serverReply(std::string code, std::string message, Client *client)
{
	sendResponse1(":" + client->servername + " " + code + " " + client->nickname + " " + message, client);
}

void serverReplyofChannel(std::string code, std::string chname ,std::string message, Client *client)
{
	sendResponse1(":" + client->servername + " " + code + " " + chname + " " + message, client);
}

void serverReplyofChannelsec(std::string code, std::string message, Client *client)
{
	sendResponse1(":" + client->servername + " " + code + "" + message + " ", client);
}

std::vector<std::string> tokenizeMessage(std::string str) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;
    
    if (std::strchr(str.c_str(), ':')) 
	{
		std::vector<std::string> tokensBeforeColon;
		while (getline(iss, token, ':')) {
			tokensBeforeColon.push_back(token);
		}
		if (tokensBeforeColon.size() >= 2) {
			std::istringstream issBeforeColon(tokensBeforeColon[0]);
			while (issBeforeColon >> token) {
				tokens.push_back(token);
			}
			tokens.push_back(tokensBeforeColon[1]);
		} else
			tokens.push_back(str);
	}
	else 
	{
		while (iss >> token)
			tokens.push_back(token);
	}
    return tokens;
}

bool isUniqueNickname(std::string nickname, std::map<int, Client *> clients, Client *client) {
	if (clients.empty())
		return true;
	for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (it->second->fd == client->fd)
			continue;
		if (it->second->nickname == nickname)
			return false;
	}
	return true;
}

void registrationReply(Client *client) {
	serverReply(RPL_WELCOME, ":Welcome to the Internet Relay Network " + client->nickname + "!" + client->username + "@" + client->hostname, client);
	serverReply(RPL_YOURHOST, ":Your host is " + client->hostname + ", running version 1.0", client);
	serverReply(RPL_CREATED, ":This server was created sometime", client);
	serverReply(RPL_MYINFO, ": " + client->hostname, client);
	serverReply(RPL_ISUPPORT, "CHANMODES=,k,l,it MODES=2 MAXNICKLEN=16 NICKLEN=16 CHANNELLEN=50 :CHANTYPES=#&", client);
}

void UserToUserMessage(std::string message, Client *src, Client *dst) {
	std::string msg = ":" + src->nickname + "!~" + src->username + "@" + src->hostname + " PRIVMSG ";
	msg += src->nickname + " :" + message + "\r\n";
	int rc = send(dst->fd, msg.c_str(), msg.length(), 0);
	if (rc == -1) {
		std::cout << "ERROR: " << strerror(errno) << std::endl;
		return;
	}
}

std::vector<std::string> splitString(const std::string& input, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream tokenStream(input);
    std::string token;

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}
