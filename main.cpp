#include "includes/server.hpp"

void ft_irc(t_server *server) {

	while (server->end_server == FALSE) 
	{
		ft_poll(server);
		for (int i = 0; i < server->nfds; i++)
		{
			if(server->fds[i].revents == 0)
				continue;

			if(server->fds[i].revents != POLLIN)
			{
				std::cout << "  Error! revents = " << server->fds[i].revents << std::endl;
				server->end_server = TRUE;
				break;
			}
			if (server->fds[i].fd == server->socket_fd)
				accept_client(server);
			else
				read_client(server, i);
    	}
		if (server->compress_array)
			compress_fds(server);
	}
}

void close_fds(t_server *server) {
	for (int i = 0; i < server->nfds; i++)
	{
		if(server->fds[i].fd >= 0)
			close(server->fds[i].fd);
	}
}

int main (int argc, char *argv[])
{
	t_server server;
    if (argc != 3) {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }
	init_server(&server, argv);
	init_socket(&server);
	ft_irc(&server);
	close_fds(&server);
}
