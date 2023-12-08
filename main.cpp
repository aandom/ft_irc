#include "includes/Server.hpp"

int main (int argc, char *argv[])
{
    if (argc != 3) {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }
	Server server(argv);
	server.init_server();
	server.main_loop();
	return 0;
}
