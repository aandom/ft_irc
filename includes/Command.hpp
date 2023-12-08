#pragma once
#include "Server.hpp"
#include "Client.hpp"

class Server;
class Client;

class Command {
private:
	Server	*server;
	Client	*client;
	
	struct t_command {
		std::string name;
		void (Command::*function)();
	};

public:
	std::vector<std::string> 	tokens;
	std::string					command;
	std::string					str;
	int							i;
	static t_command			r_commands[];
	Command(Server *server, Client *client, std::string str, int i);
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

	/****************************** Connection Commands********************************/
	void						CapCommand();
	void						NickCommand();
	void						PassCommand();
	void						UserCommand();
	void						pingCommand();
	void						operCommand();
	/*********************************Sending Message*******************************/
	void						PrivmsgCommand();
};
