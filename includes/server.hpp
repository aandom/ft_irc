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

typedef struct s_server
{
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
}	t_server;

void 	accept_client (t_server *server);
void 	read_client (t_server *server, int i);
void 	init_socket (t_server *server);
void 	init_server (t_server *server, char *argv[]);
void 	compress_fds (t_server *server);
void 	ft_poll(t_server *server);
void 	ft_irc(t_server *server);