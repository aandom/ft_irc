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
#include <fcntl.h>
#include <sys/types.h>
#include <netdb.h>
#include <poll.h>
#include <vector>
#include <map>
#include <sstream>
#include "Client.hpp"
#include "Command.hpp"
#include "error_codes.hpp"
#include "reply.hpp"
#include "Commands_utils.hpp"
#include "Channel.hpp"
#include "Channelcmds.hpp"
#include "servError.hpp"
#include <cstdio>

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
		struct addrinfo		*address;
		int				    timeout;
		std::string			password;
		std::string			operator_password;
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

		void 	init_server_address();
		void 	create_socket();
		void 	set_socket_options();
		void 	bind_socket();
		void 	start_listening();
		void 	initialize_poll();
		void 	init_server ();

		void 	accept_client ();
		void 	read_client (int i);
		void 	compress_fds ();
		void 	ft_poll();
		void 	main_loop();
		void 	close_fds();
		void 	init_error(std::string error);
		void	close_connection(int i);
		void	create_commands(int fd, std::string str);
		void 	printClients();

		std::vector<Channel *> &getChannels();
		std::map<int, Client *> &getClients();
		Client * getClientIfExist(std::string const & cname);
		Channel * getChannelIfExist(std::string const & cname);
		void	addChannel(Channel * channel);
		void	removeChannel(Channel * channel);
		bool	checkIfClientExists(std::string const & cname);
		std::vector<std::string>  getChannelNames();
		ErrorCode _errors;
		typedef std::vector<Channel *>::iterator ch_iterator;
		typedef std::map<int, Client *>::iterator cl_iterator;

	private:
		std::vector<Channel *> _channels;


};