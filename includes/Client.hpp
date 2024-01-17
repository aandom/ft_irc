#pragma once

#include "Server.hpp"

enum UserMode {
    INVISIBLE = 'i',
    SERVERNOTICES = 's',
    WALLOPS = 'w',
    // OPERATOR = 'o'
};

class Client {
public:
	int 		fd;
	// int 		server_port;
	std::string nickname;
	std::string username;
	std::string realname;
	std::string hostname;
	std::string servername;
	std::string client_ip;
	char   		buffer[1024];
	std::string str;
	bool 		is_registered;
	bool		is_authenticated;
	bool		is_operator;
	int			user_mode;
	std::string away;
	std::string mode;
	std::string ip;

	Client(int fd, char *server_port);
	Client(Client const &src);
	Client &operator=(Client const &src);
	~Client();
};