#include "includes/Command.hpp"
#include <sstream>

Command::Command(Server *server, Client *client, std::string str, int i) {
	this->server = server;
	this->client = client;
	this->str = str;
	this->i = i;
}

Command::~Command() { }

Command::Command(Command const &src) {
	*this = src;
}

Command &Command::operator=(Command const &src) {
	if (this != &src) {
		this->server = src.server;
		this->client = src.client;
		this->str = src.str;
	}
	return *this;
}

// Function to tokenize an IRC message
std::vector<std::string> Command::tokenizeMessage() {
    std::vector<std::string> tokens;
    std::istringstream iss(this->str);
    std::string token;
    
    if (std::strchr(this->str.c_str(), ':')) 
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
			tokens.push_back(this->str);
	}
	else 
	{
		while (iss >> token)
			tokens.push_back(token);
	}
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

void Command::NickCommand() {
	if (tokens.size() >= 1) {
		std::string nickname = tokens[0];
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

void Command::CapCommand() {
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

void Command::PassCommand() {
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

void Command::UserCommand() {
	if (this->client->is_registered == true)
		return (sendErrorResponse(ERR_ALREADYREGISTERED, "You may not reregister"));
	if (tokens.size() < 4)
		return (sendErrorResponse(ERR_NEEDMOREPARAMS, "USER : Need more parameters"));
	if (this->client->is_authenticated == false)
		return (sendErrorResponse(ERR_PASSWDMISMATCH, "You need to give a password first"));
	this->client->username = tokens[0];
	this->client->hostname = tokens[1];
	this->client->servername = tokens[2];
	this->client->realname = tokens[3];
	// if (tokens[3].size() > 1) {
	// 	if (tokens[3][0] == ':')
	// 		this->client->realname = tokens[3].erase(0, 1);
	// 	if (tokens.size() > 4) {
	// 		for (size_t i = 4; i < tokens.size(); i++)
	// 			this->client->realname += " " + tokens[i];
	// 	}
	// 	else
	// 		this->client->realname = tokens[3];
	// } else 
	// 	sendErrorResponse(ERR_NEEDMOREPARAMS, "USER : Need more parameters");
	this->client->is_registered = true;
	registrationReply();	
}

void Command::pingCommand() {
	if (tokens.size() >= 1)
		sendResponse("PONG " + client->hostname + " " + tokens[0]);
	else
		sendErrorResponse(ERR_NEEDMOREPARAMS, "PING : Need more parameters");
}

void Command::operCommand() {
	if (tokens.size() == 3) {
		// std::cout << server->operator_password << std::endl;
		// std::cout << tokens[1] << std::endl;
		if (tokens[2] == server->operator_password && tokens[1] == client->hostname) {
			client->is_operator = true;
			sendResponse(std::string(RPL_YOUREOPER) + " :You are now an IRC operator");
		} else {
			sendErrorResponse(ERR_PASSWDMISMATCH, "OPER :Password incorrect");
		}
	} else {
		sendErrorResponse(ERR_NEEDMOREPARAMS, "Error : Wrong Number of parameters");
	}
}

void Command::PrivmsgCommand() {
	if (tokens.size() >= 3) {
		std::string target = tokens[1];
		std::string message = tokens[2];
		if (target[0] == '#') {
			for (std::map<int, Client *>::iterator it = server->clients.begin(); it != server->clients.end(); it++)
			{
				if (it->second->nickname == target) {
					sendResponse("PRIVMSG " + target + " :" + message);
					return;
				}
			}
			sendErrorResponse(ERR_NOSUCHNICK, "PRIVMSG :No such nick/channel");
		} else {
			for (std::map<int, Client *>::iterator it = server->clients.begin(); it != server->clients.end(); it++)
			{
				if (it->second->nickname == target) {
					sendResponse("PRIVMSG " + target + " :" + message);
					return;
				}
			}
			sendErrorResponse(ERR_NOSUCHNICK, "PRIVMSG :No such nick/channel");
		}
	} else {
		sendErrorResponse(ERR_NEEDMOREPARAMS, "PRIVMSG :Need more parameters");
	}
}

void Command::NoticeCommand() {
	if (tokens.size() >= 3) {
		std::string target = tokens[1];
		std::string message = tokens[2];
		if (target[0] == '#') {
			for (std::map<int, Client *>::iterator it = server->clients.begin(); it != server->clients.end(); it++)
			{
				if (it->second->nickname == target) {
					sendResponse("NOTICE " + target + " :" + message);
					return;
				}
			}
			sendErrorResponse(ERR_NOSUCHNICK, "NOTICE :No such nick/channel");
		} else {
			for (std::map<int, Client *>::iterator it = server->clients.begin(); it != server->clients.end(); it++)
			{
				if (it->second->nickname == target) {
					sendResponse("NOTICE " + target + " :" + message);
					return;
				}
			}
			sendErrorResponse(ERR_NOSUCHNICK, "NOTICE :No such nick/channel");
		}
	} else {
		sendErrorResponse(ERR_NEEDMOREPARAMS, "NOTICE :Need more parameters");
	}
}

void Command::executeCommand() {
	if (client->is_registered == false) {
		if (this->command != "CAP" && this->command != "PASS" && this->command != "NICK" && this->command != "USER")
		{
			sendErrorResponse(ERR_UNKNOWNCOMMAND, "unknown command");
			return;
		}
		if (this->command == "NICK") {
			tokens.erase(tokens.begin());
			NickCommand();
		} else if (this->command == "PASS") {
			tokens.erase(tokens.begin());
			PassCommand();
		} else if (this->command == "CAP") {
			tokens.erase(tokens.begin());
			CapCommand();
		} else if (this->command == "USER") {
			tokens.erase(tokens.begin());
			UserCommand();
		}
	}
	else 
	{
		if (this->command == "PING") {
			tokens.erase(tokens.begin());
			pingCommand();
		}
		else if (this->command == "NICK") {
			tokens.erase(tokens.begin());
			NickCommand();
		}
		else if (this->command == "QUIT") {
			sendResponse("QUIT :Bye bye");
			server->clients.erase(client->fd);
			close(client->fd);
			this->server->fds[this->i].fd = -1;			
			server->compress_array = true;
		}
		else if (this->command == "OPER") {
			operCommand();
		}
		else if (this->command == "PRIVMSG") {
			PrivmsgCommand();
		}
		else if (this->command == "NOTICE") {
		}
		else if (this->command == "JOIN") {
		//
		}
		else if (this->command == "PART") {
		//
		}
		else
			sendErrorResponse(ERR_UNKNOWNCOMMAND, "unknown command");
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
