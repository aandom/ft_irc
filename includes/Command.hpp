#pragma once
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Channelcmds.hpp"

class Server;
class Client;

typedef void (*m_func)(Server &, Client *, std::vector<std::string> &);

class Command {
private:
	Server	*server;
	Client	*client;

	struct t_command {
		std::string name;
		void (Command::*function)();
	};

public:
	typedef void (Command::*cl_func)();
	std::vector<std::string> 	tokens;
	std::string					command;
	std::string					str;
	int							i;
	static t_command			r_commands[];
	Command(Server *server, Client *client, std::string str, int i);
	~Command();
	Command(Command const &src);
	Command &operator=(Command const &src);
	std::map<std::string, m_func> chancmds;
	std::map<std::string, cl_func> clcmds;


	void 						parse_command();
	void 						executeCommand();

	/****************************** Connection Commands********************************/
	void						CapCommand();
	void						NickCommand();
	void						PassCommand();
	void						UserCommand();
	void						pingCommand();
	void						operCommand();
	void						motdCommand();
	void						quitCommand();
	void 						whoisCommand();
	void						modeCommand();
	void 						userMode();
	// **
	void						killCommand();
	/*********************************Sending Message*******************************/
	void						PrivmsgCommand();
};
