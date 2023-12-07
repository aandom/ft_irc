#pragma once
#include "Server.hpp"
#include "Client.hpp"

class Server;
class Client;

class Command {
private:
	Server	*server;
	Client	*client;
public:
	std::vector<std::string> 	tokens;
	std::string					command;
	std::string					str;
	Command(Server *server, Client *client, std::string str);
	~Command();
	Command(Command const &src);
	Command &operator=(Command const &src);

	void 						parse_command();
	void 						executeCommand();
	std::vector<std::string> 	tokenizeMessage();
	void						sendErrorResponse(std::string err_code, std::string message);
	void 						sendResponse(std::string message);
	bool 						isUniqueNickname(std::string nickname);
	void 						registrationReply();

	/****************************** Registration Commands***************************************/
	void						CapCommand();
	void						NickCommand();
	void						PassCommand();
	void						UserCommand();
};
