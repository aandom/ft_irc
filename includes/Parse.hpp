#pragma once
#include "Server.hpp"
#include "Client.hpp"

class Server;
class Client;

class Parse {
private:
	Server	*server;
	Client	*client;
public:
	std::vector<std::string> tokens;
	std::string					command;
	std::string					str;
	std::string					params;
	std::string					prefix;
	std::string					trailing;
	std::string					middle;
	std::string::size_type		pos;
	std::string::size_type		pos2;
	std::string::size_type		pos3;
	Parse(Server *server, Client *client, std::string str);
	~Parse();
	// Parse(Parse const &src);
	// Parse &operator=(Parse const &src);

	void 						parse_command();
	std::vector<std::string> 	tokenizeMessage();
	// void						handleError(const std::string& message);
	void						sendErrorResponse(const std::string& message);
	void						handleNickCommand(std::vector<std::string> tokens);
	void						handleUserCommand(std::vector<std::string> tokens);
	void						handleCapCommand(std::vector<std::string> tokens);
	bool 						isUniqueNickname(std::string nickname);
	void 						sendResponse(std::string message);
};
