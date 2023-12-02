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

#define TRUE             1
#define FALSE            0

class server {
	public:
		int 				socket_fd;
		int					port;
		int					new_sd;
		int					rc;
		char   				buffer[80];
		struct sockaddr_in6 addr;
		int				    timeout;
		std::string			password;
		struct pollfd		fds[200];
		int					nfds;	
		int					on;
		int					end_server;
		bool				close_conn;
		bool				compress_array;

		server(char *argv[]);
		~server();
		server(server const &src);
		server &operator=(server const &src);

		void 	accept_client ();
		void 	read_client (int i);
		void 	init_socket ();
		void 	compress_fds ();
		void 	ft_poll();
		void 	ft_irc();
		void 	close_fds();
		void 	init_error(std::string error);
		void	close_connection(int i);
};
