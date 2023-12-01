#include "includes/server.hpp"

void accept_client (t_server *server) {
	std::cout << "  Listening socket is readable" << std::endl;
	do 
	{
		server->new_sd = accept(server->socket_fd, NULL, NULL);
		if (server->new_sd < 0) {
			if (errno != EWOULDBLOCK) {
				perror("  accept() failed");
				server->end_server = TRUE;
			}
			return;
		}
		std::cout << "  New incoming connection - " << server->new_sd << std::endl;
		server->fds[server->nfds].fd = server->new_sd;
		server->fds[server->nfds].events = POLLIN;
		server->nfds++;

	} while (server->new_sd != -1);
}

void read_client (t_server *server, int i) {
	std::cout << "  Descriptor " << server->fds[i].fd << " is readable" << std::endl;
	server->close_conn = FALSE;
	do {
		server->rc = recv(server->fds[i].fd, server->buffer, sizeof(server->buffer), 0);
		if (server->rc < 0) {
			if (errno != EWOULDBLOCK) {
				perror("  recv() failed");
				server->close_conn = TRUE;
			}
			break;
		}
		if (server->rc == 0) {
			std::cout << "  Connection closed" << std::endl;
			server->close_conn = TRUE;
			break;
		}
		int len = server->rc;
		std::cout << len << " bytes received\n" << server->buffer;
		// server->rc = send(server->fds[i].fd, server->buffer, len, 0);
		// if (server->rc < 0) {
		// 	perror("  send() failed");
		// 	server->close_conn = TRUE;
		// 	break;
		// }
	} while (TRUE);
	if (server->close_conn) {
		close(server->fds[i].fd);
		server->fds[i].fd = -1;
		server->compress_array = TRUE;
	}
}

void init_error(std::string error, t_server *server) {
	std::cerr << error << std::endl;
	close(server->socket_fd);
	exit(1);
}

void init_socket (t_server *server) {
	server->socket_fd = socket(AF_INET6, SOCK_STREAM, 0);
	if (server->socket_fd < 0)
		init_error("socket() failed", server);
	
	server->rc = setsockopt(server->socket_fd, SOL_SOCKET,  SO_REUSEADDR, (char *)&server->on, sizeof(server->on));
	if (server->rc < 0)
		init_error("setsockopt() failed", server);
	
	server->rc = ioctl(server->socket_fd, FIONBIO, (char *)&server->on);
	if (server->rc < 0)
		init_error("ioctl() failed", server);

	memset(&server->addr, 0, sizeof(server->addr));
	server->addr.sin6_family      = AF_INET6;
	memcpy(&server->addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
	server->addr.sin6_port = htons(server->port);
	server->rc = bind(server->socket_fd, (struct sockaddr *)&server->addr, sizeof(server->addr));
	if (server->rc < 0)
		init_error("bind() failed", server);

	server->rc = listen(server->socket_fd, 32);
	if (server->rc < 0)
		init_error("listen() failed", server);

	memset(server->fds, 0 , sizeof(server->fds));
	server->fds[0].fd = server->socket_fd;
	server->fds[0].events = POLLIN;
	server->timeout = (3 * 60 * 1000);
}

void init_server (t_server *server, char *argv[]) {
	server->socket_fd = -1;
	server->port = std::atoi(argv[1]);
	server->new_sd = -1;
	server->rc = 1;
	server->timeout = 0;
	server->password = argv[2];
	server->nfds = 1;
	server->on = 1;
	server->end_server = FALSE;
	server->close_conn = FALSE;
	server->compress_array = FALSE;
}

void compress_fds (t_server *server) {
	server->compress_array = FALSE;
	for (int i = 0; i < server->nfds; i++)
	{
		if (server->fds[i].fd == -1)
		{
			for(int j = i; j < server->nfds; j++)
			{
				server->fds[j].fd = server->fds[j+1].fd;

			}
			i--;
			server->nfds--;
		}
	}
}

void ft_poll(t_server *server) {
	std::cout << "Waiting on poll()..." << std::endl;
	server->rc = poll(server->fds, server->nfds, server->timeout);
	if (server->rc < 0)	{
		perror("  poll() failed");
		server->end_server = TRUE;
	}
	if (server->rc == 0) {
		std::cout << "  poll() timed out.  End program." << std::endl;
		server->end_server = TRUE;
	}
}