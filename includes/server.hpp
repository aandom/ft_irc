#pragma once

#include <iostream>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <map>
#include <fcntl.h>
#include <sys/types.h>
#include <netdb.h>
#define _GNU_SOURCE
#include <poll.h>
#include <vector>
#include "Client.hpp"
#include "Parse.hpp"

#define TRUE             1
#define FALSE            0

class Client;

class Server {
	public:
		int 				socket_fd;
		int					port;
		int					new_sd;
		int					rc;
		char   				buffer[256];
		// struct sockaddr_in6 addr;
		struct addrinfo		*address;
		int				    timeout;
		std::string			password;
		struct pollfd		fds[200];
		int					nfds;	
		int					on;
		int					end_server;
		bool				close_conn;
		bool				compress_array;
		std::map<int, Client *>	clients;
		std::vector<std::string>	commands;

		Server(char *argv[]);
		~Server();
		Server(Server const &src);
		Server &operator=(Server const &src);

		void 	accept_client ();
		void 	read_client (int i);
		void 	init_socket ();
		void 	compress_fds ();
		void 	ft_poll();
		void 	ft_irc();
		void 	close_fds();
		void 	init_error(std::string error);
		void	close_connection(int i);
		void	create_commands(int fd, std::string str);
		void 	printClients();
};
