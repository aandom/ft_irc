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
	// Command(Command const &src);
	// Command &operator=(Command const &src);

	void 						parse_command();
	void 						executeCommand();
	std::vector<std::string> 	tokenizeMessage();
	void						CapCommand(std::vector<std::string> tokens);
	void						sendErrorResponse(std::string err_code, std::string message);
	void 						sendResponse(std::string message);
	void						NickCommand(std::vector<std::string> tokens);
	void						PassCommand(std::vector<std::string> tokens);
	void						UserCommand(std::vector<std::string> tokens);
	bool 						isUniqueNickname(std::string nickname);
	void 						registrationReply();
};
