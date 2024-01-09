#include "includes/Server.hpp"

bool end_server = FALSE;

static void signalHandler(int signal)
{
	std::cout << "\033[31m Interrupt signal (" << signal << ") received. \033[30m \n";
	end_server = TRUE;
	exit(signal);
}

int main (int argc, char *argv[])
{
    if (argc != 3) {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
	Server server(argv);
	server.init_server();
	server.main_loop();
	return 0;
}
// Press Ctrl + Z to suspend the process.
// You can list the suspended jobs with the jobs command, and then use the job number with the fg command to reverse suspension.