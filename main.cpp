#include "includes/Server.hpp"
#include <csignal>

int g_endServer = FALSE;

void sigint_handler(int sig)
{
    (void)sig;
    g_endServer = TRUE;
}

int main (int argc, char *argv[])
{
    if (argc != 3) {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }
    signal(SIGINT, sigint_handler);
	Server server(argv);
	server.init_server();
	server.main_loop();
	return 0;
}


// docker run -it --cap-add=SYS_PTRACE --security-opt seccomp=unconfined --security-opt apparmor=unconfined --name 42-valgrind --network host --rm -v "$PWD:/home/vscode/src" valgrind "/bin/zsh"