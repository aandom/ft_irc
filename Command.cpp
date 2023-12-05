#include "includes/Command.hpp"
#include <sstream>

Command::Command(Server *server, Client *client, std::string str) {
	this->server = server;
	this->client = client;
	this->str = str;
}

Command::~Command() { }

// Command::Command(Command const &src) {
// 	*this = src;
// }

// Function to tokenize an IRC message
std::vector<std::string> Command::tokenizeMessage() {
    std::vector<std::string> tokens;
    std::istringstream iss(this->str);
    std::string token;
    
    while (iss >> token)
        tokens.push_back(token);
    
    return tokens;
}

void Command::sendErrorResponse(std::string err_code, std::string message) {
	// std::cout << "ERROR: " << message << std::endl;
	std::string response = err_code + "ERROR: " + message + "\r\n";
	int ret = send(client->fd, response.c_str(), response.length(), 0);
	if (ret == -1)
		std::cout << "ERROR: " << strerror(errno) << std::endl;
}

void Command::sendResponse(std::string message) {
	std::string response = message + "\r\n";
	int ret = send(client->fd, response.c_str(), response.length(), 0);
	if (ret == -1)
		std::cout << "ERROR: " << strerror(errno) << std::endl;
}

bool Command::isUniqueNickname(std::string nickname) {
	for (std::map<int, Client *>::iterator it = server->clients.begin(); it != server->clients.end(); it++)
	{
		if (it->second->fd == client->fd)
			continue;
		if (it->second->nickname == nickname)
			return false;
	}
	return true;
}

void Command::NickCommand(std::vector<std::string> tokens) {
	if (tokens.size() >= 1) {
		std::string nickname = tokens[0];
		// Validate and store the nickname
		if (!isUniqueNickname(nickname)) {
			sendErrorResponse(ERR_NICKNAMEINUSE, "Nickname is already in use");
		} else {
			client->nickname = nickname;
			sendResponse("Nickname set successfully");
		}
	} else {
		sendErrorResponse(ERR_NONICKNAMEGIVEN, "No nickname given");
	}
}

void Command::CapCommand(std::vector<std::string> tokens) {
	std::string mes = "CAP * LS :multi-prefix userhost-in-names";
	if (tokens.size() >= 1) {
		std::string cap = tokens[0];
		if (cap == "LS") {
			sendResponse(mes);
		} else if (tokens.size() >= 2 && cap == "REQ") {
			sendResponse("CAP * ACK " + tokens[1] + "\n");
		} else if (cap == "END") {
			sendResponse("CAP * ACK :multi-prefix userhost-in-names");
		} else {
			sendErrorResponse(ERR_UNKNOWNCOMMAND, "CAP : Unknown command");
		}
	} else {
		sendErrorResponse(ERR_NEEDMOREPARAMS, "CAP : Need more parameters");
	}
}

void Command::PassCommand(std::vector<std::string> tokens) {
	if (tokens.size() >= 1) {
		if (client->is_registered == true) {
			sendErrorResponse(ERR_ALREADYREGISTERED, "You may not reregister");
			return;
		}
		if (tokens[0] == server->password) {
			client->is_authenticated = true;
		} else {
			sendErrorResponse(ERR_PASSWDMISMATCH, "Password incorrect");
			return;
		}
		sendResponse("Password set successfully");
	} else {
		sendErrorResponse(ERR_NEEDMOREPARAMS, "PASS : Need more parameters");
	}
}

void Command::registrationReply() {
	sendResponse(std::string (RPL_WELCOME) + " :Welcome to the Internet Relay Network " + client->nickname + "!" + client->username + "@" + client->hostname + "\r\n");
	sendResponse(std::string (RPL_YOURHOST) + " :Your host is " + this->client->hostname + ", running version 1.0\r\n");
	sendResponse(std::string (RPL_CREATED) + " :This server was created sometime\r\n");
	sendResponse(std::string (RPL_MYINFO) + " :This server was created sometime\r\n");
	sendResponse(std::string (RPL_ISUPPORT) + "CHANMODES=,k,l,it MODES=2 MAXNICKLEN=16 NICKLEN=16 CHANNELLEN=50 :CHANTYPES=#&\r\n");
}

void Command::UserCommand(std::vector<std::string> tokens) {
	if (this->client->is_registered == true)
		return (sendErrorResponse(ERR_ALREADYREGISTERED, "You may not reregister"));
	if (tokens.size() < 4)
		return (sendErrorResponse(ERR_NEEDMOREPARAMS, "USER : Need more parameters"));
	if (this->client->is_authenticated == false)
		return (sendErrorResponse(ERR_PASSWDMISMATCH, "You need to give a password first"));
	this->client->username = tokens[0];
	this->client->hostname = tokens[1];
	this->client->servername = tokens[2];
	if (tokens[3].size() > 1) {
		if (tokens[3][0] == ':')
			this->client->realname = tokens[3].erase(0, 1);
		if (tokens.size() > 4) {
			for (size_t i = 4; i < tokens.size(); i++)
				this->client->realname += " " + tokens[i];
		}
		else
			this->client->realname = tokens[3];
	} else 
		sendErrorResponse(ERR_NEEDMOREPARAMS, "USER : Need more parameters");
	this->client->is_registered = true;
	registrationReply();	
}

void Command::executeCommand() {
	if (client->is_registered == false) {
		if (this->command != "CAP" && this->command != "PASS" && this->command != "NICK" && this->command != "USER")
		{
			sendErrorResponse(ERR_NOTREGISTERED, "You have not registered");
			return;
		}
		if (this->command == "NICK") {
			tokens.erase(tokens.begin());
			NickCommand(tokens);
		} else if (this->command == "PASS") {
			tokens.erase(tokens.begin());
			PassCommand(tokens);
		} else if (this->command == "CAP") {
			tokens.erase(tokens.begin());
			CapCommand(tokens);
		} else if (this->command == "USER") {
			tokens.erase(tokens.begin());
			UserCommand(tokens);
		}
	}
}

void Command::parse_command() {
	this->tokens = tokenizeMessage();
    if (this->tokens.empty()) {
        sendErrorResponse(ERR_UNKNOWNCOMMAND, "Invalid message format");
        return;
    }
    this->command = tokens[0];
}
