#pragma once

#include "server.hpp"

typedef struct s_client {
	int 		socket_fd;
	int 		server_port;
	std::string nickname;
	std::string username;
	std::string realname;
	std::string hostname;
	std::string servername;
	std::string password;
	bool 		is_registered;
	std::string away;
	std::string mode;
	std::string ip;
} t_client;