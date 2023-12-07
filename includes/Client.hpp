#pragma once

#include "Server.hpp"

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
	bool 		is_registered;
	bool		is_authenticated;
	bool		is_operator;
	std::string away;
	std::string mode;
	std::string ip;

	Client(int fd, char *server_port);
	Client(Client const &src);
	Client &operator=(Client const &src);
	~Client();
};