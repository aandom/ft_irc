#include "includes/Command.hpp"

Command::Command(Server *server, Client *client, std::string str, int i) {
	this->server = server;
	this->client = client;
	this->str = str;
	this->i = i;

	chancmds["JOIN"]   = join;
	chancmds["PART"]   = part;
	chancmds["TOPIC"]  = topic;
	chancmds["INVITE"] = invite;
	chancmds["KICK"]   = kick;
	chancmds["NAMES"]  = names;

	clcmds["PING"]    = &Command::pingCommand;
	clcmds["NICK"]    = &Command::NickCommand;
	clcmds["QUIT"]    = &Command::quitCommand;
	clcmds["OPER"]    = &Command::operCommand;
	clcmds["PRIVMSG"] = &Command::PrivmsgCommand;
	clcmds["WHOIS"]   = &Command::whoisCommand;
	clcmds["MODE"]   = &Command::modeCommand;
}

Command::t_command Command::r_commands[] = {
	{"CAP", &Command::CapCommand},
	{"NICK", &Command::NickCommand},
	{"PASS", &Command::PassCommand},
	{"USER", &Command::UserCommand},
	{"", NULL},
};

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

void Command::NickCommand() {
	if (client->is_registered == true)
		return (serverReply(ERR_ALREADYREGISTERED, "You may not reregister", client));
	if (client->is_authenticated == false)
		return (serverReply(ERR_PASSWDMISMATCH, "You need to give a password first", client));
	if (tokens.size() >= 2) {
		std::string nickname = tokens[1];
		std::cout << "nickname: " << nickname << std::endl;
		if (!isUniqueNickname(nickname, this->server->clients, this->client)) {
			serverReply(ERR_NICKNAMEINUSE, "Nickname is already in use", client);
		} else {
			client->nickname = nickname;
		}
	} else
		serverReply(ERR_NONICKNAMEGIVEN, "No nickname given", client);
	
}

void Command::CapCommand() {
	std::string mes = "CAP * LS :multi-prefix userhost-in-names";
	if (tokens.size() >= 2) {
		std::string cap = tokens[1];
		if (cap == "LS")
			sendResponse(mes, this->client);
		else if (tokens.size() >= 3 && cap == "REQ")
			sendResponse("CAP * ACK " + tokens[2], this->client);
		else if (cap == "END")
		{
			sendResponse("CAP * ACK :multi-prefix userhost-in-names", this->client);
		}
		else
			serverReply(ERR_UNKNOWNCOMMAND, "CAP : Unknown command", client);
	} else
			serverReply(ERR_NEEDMOREPARAMS, "CAP : Need more parameters", client);
}

void Command::PassCommand() {
	if (tokens.size() >= 2) {
		if (client->is_registered == true)
			return (serverReply(ERR_ALREADYREGISTERED, "You are already registered", client));
		tokens[1] = (tokens[1].at(0) == ':') ? tokens[1].substr(1) : tokens[1];
		if (tokens[1] == server->password)
			client->is_authenticated = true;
		else
			serverReply(ERR_PASSWDMISMATCH, "Password incorrect", client);
	} else
		serverReply(ERR_NEEDMOREPARAMS, "PASS : Need more parameters", client);
}

void Command::UserCommand() {
	// static int i;
	if (this->client->is_registered == true)
		return (serverReply(ERR_ALREADYREGISTERED, "You may not reregister", this->client));
	if (tokens.size() < 5)
		return (serverReply(ERR_NEEDMOREPARAMS, "USER : Need more parameters", this->client));
	if (this->client->is_authenticated == false)
		return (serverReply(ERR_PASSWDMISMATCH, "You need to give a password first", this->client));
	// if (this->client->nickname.empty())
	// 	return (serverReply(ERR_PASSWDMISMATCH, "You need to give a nick first", this->client));
	if (this->client->nickname.empty() && ++i)
		client->nickname = "user_" + intToStr(i);
	this->client->username = tokens[1];
	this->client->servername = tokens[3];
	this->client->realname = tokens[4];
	this->client->is_registered = true;
	registrationReply(this->client);
	motdCommand();
}

void Command::pingCommand() {
	if (tokens.size() >= 2)
		serverReply("PONG", ":" + client->hostname + " " + tokens[1], client);
	else
		serverReply(ERR_NEEDMOREPARAMS, "PING : Need more parameters", client);
}

void Command::operCommand() {
	if (tokens.size() == 3) {
		if (tokens[2] == server->operator_password && tokens[1] == client->hostname) {
			client->is_operator = true;
			serverReply(RPL_YOUREOPER, ":You are now an IRC operator", client);
		} else
			serverReply(ERR_PASSWDMISMATCH, "OPER : incorrect password or host", client);

	} else
		serverReply(ERR_NEEDMOREPARAMS, "OPER : Wrong Number of parameters", client);
}

void Command::PrivmsgCommand() {
	if (tokens.size() >= 3)
	{
		std::string target = tokens[1];
		std::string message = tokens[2];
		if (target[0] == '#' && target[1] == '*' && client->is_operator == true)
		{
			for (std::map<int, Client *>::iterator it = server->clients.begin(); it != server->clients.end(); it++)
				UserToUserMessage(message, client, it->second);
		}
		else if (target[0] == '#') {
			try {
				privMsgchannel(*server, client, tokens);
			} catch (std::exception &e) {
				handleException(e.what());
			}
		}
		else if (std::strchr(target.c_str(), ','))
		{
			std::vector<std::string> targets = splitString(target, ',');
			for (std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); it++)
			{
				for (std::map<int, Client *>::iterator it2 = server->clients.begin(); it2 != server->clients.end(); it2++)
				{
					if (it2->second->nickname == *it)
						UserToUserMessage(message, client, it2->second);
				}
			}
		}
		else 
		{
			for (std::map<int, Client *>::iterator it = server->clients.begin(); it != server->clients.end(); it++)
			{
				if (it->second->nickname == target) {
					UserToUserMessage(message, client, it->second);
					return;
				}
			}
			serverReply(ERR_NOSUCHNICK, "PRIVMSG :No such nick/channel", client);
		}
	}
	else
		serverReply(ERR_NEEDMOREPARAMS, "PRIVMSG :Need more parameters", client);
}

void Command::userMode() {
	std::string nickname = tokens[1];
	for (std::map<int, Client *>::iterator it = server->clients.begin(); it != server->clients.end(); it++)
	{
		if (it->second->nickname == nickname) {
			if (tokens[2][0] == '+')
			{
				if (tokens[2][1] == 'o')
					it->second->is_operator = true;
				else if (tokens[2][1] == 'i')
					it->second->user_mode = INVISIBLE;
				else if (tokens[2][1] == 's')
					it->second->user_mode = SERVERNOTICES;
				else if (tokens[2][1] == 'w')
					it->second->user_mode = WALLOPS;
			}
			else if (tokens[2][0] == '-')
			{
				if (tokens[2][1] == 'o')
					it->second->is_operator = false;
				else if (tokens[2][1] == 'i')
					it->second->user_mode = 0;
				else if (tokens[2][1] == 's')
					it->second->user_mode = 0;
				else if (tokens[2][1] == 'w')
					it->second->user_mode = 0;
			}
			return;
		}
	}	
	serverReply(ERR_NOSUCHNICK, "MODE :No such nick/channel", client);
}

void Command::modeCommand() {
	if (tokens.size() >= 2 && tokens[1][0] == '#')
	{
		try {
			mode(*this->server, this->client, this->tokens);
		} catch(const std::exception& e) {
			handleException(e.what());
		}
			
	}
	else if (tokens.size() >= 3)
		userMode();
	else
		serverReply(ERR_NEEDMOREPARAMS, "MODE :Need more parameters", client);
}

void Command::motdCommand() {
	serverReply(RPL_MOTDSTART, ":- " + client->servername + " Message of the day - ", client);
	serverReply(RPL_MOTD, ":- Welcome to the Internet Relay Network " + client->nickname + "!" +  \
		client->username + "@" + client->hostname + "", client);
	serverReply(RPL_MOTD, ":- This is a friendly community", client);
	serverReply(RPL_MOTD, ":- This Ft_IRC server is made by: dsium, aandom, and zsyyida", client);
	serverReply(RPL_ENDOFMOTD, ":End of MOTD command", client);
}

void Command::quitCommand() {
	sendResponse("QUIT :Bye bye", this->client);
	server->clients.erase(client->fd);
	close(client->fd);
	this->server->fds[this->i].fd = -1;			
	server->compress_array = true;
}

void Command::whoisCommand() {
	if (tokens.size() >= 2) {
		std::string nickname = tokens[1];
		for (std::map<int, Client *>::iterator it = server->clients.begin(); it != server->clients.end(); it++)
		{
			if (it->second->nickname == nickname) {
				serverReply(RPL_WHOISUSER, nickname + " " + it->second->username + " " + it->second->hostname + " * :" + it->second->realname, client);
				serverReply(RPL_WHOISSERVER, nickname + " " + it->second->servername + " :Ft_IRC", client);
				if (it->second->is_operator == true)
					serverReply(RPL_WHOISOPERATOR, nickname + " :is an IRC operator", client);
				serverReply(RPL_ENDOFWHOIS, nickname + " :End of WHOIS list", client);
				return;
			}
		}
		serverReply(ERR_NOSUCHNICK, "WHOIS :No such nick/channel", client);
	} else {
		serverReply(ERR_NEEDMOREPARAMS, "WHOIS :Need more parameters", client);
	}
}

void Command::handleException(std::string err) {
	std::string chname = "";
	if (this->command == "INIVTE" && tokens.size() >= 3)
		chname = tokens[2];
	else if (err == "461") {
		chname = this->command;
	}
	else
		chname = tokens[1];
	srvRplErr(err, chname, client, *server);
}

void Command::executeCommand() {
	if (client->is_registered == false) {
		for (int i = 0; !r_commands[i].name.empty(); i++)
		{
			if (r_commands[i].name == this->command)
				return (this->*r_commands[i].function)();
		}
		return (serverReply(ERR_UNKNOWNCOMMAND, "unknown command before registration", client));
	}
	else 
	{
		try {
			if (clcmds.find(this->command) != clcmds.end()){
				cl_func tobecalled =  clcmds[this->command];
				(this->*tobecalled)();
			}
			else if (chancmds.find(this->command) != chancmds.end()){
				(chancmds[this->command])(*this->server, this->client, this->tokens);
			}
			else if (this->command == "MOTD")
				motdCommand();
			else
				serverReply(ERR_UNKNOWNCOMMAND, "unknown command after registration", client);
		} catch (std::exception &e) {
			// std::cout << "err = [" ;
			// std::cout <<  e.what() << "]" << std::endl;
			handleException(e.what());
		}
	}
}

void Command::parse_command() {
	this->tokens = tokenizeMessage(this->str);
    if (this->tokens.empty()) {
		serverReply(ERR_UNKNOWNCOMMAND, "Invalid message format", client);
        return;
    }
    this->command = tokens[0];
}
