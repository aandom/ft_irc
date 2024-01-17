#include "includes/Server.hpp"
#include "csignal"

bool end_server = FALSE;

static void signalHandler(int signal)
{
	std::cout << "\033[31m Interrupt signal (" << signal << ") received. \033[30m \n";
	end_server = TRUE;
}

int main (int argc, char *argv[])
{
    if (argc != 3) {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }
    signal(SIGINT, signalHandler);
    // signal(SIGTERM, signalHandler);
	Server server(argv);
	server.init_server();
	server.main_loop();
	return 0;
}
// Press Ctrl + Z to suspend the process.
// You can list the suspended jobs with the jobs command, and then use the job number with the fg command to reverse suspension.

// to start server
// ./ircserv <port> <password>
// /set nick <name>
// /set user_name <user name>
//  /set real_name <real_name
// /connect localhost <port> <password>
// /oper 127.0.0.1 pa$$word
// /join #<channel_name>
// /msg <nick> <message>

// for client run irssi in docker (change irssi to irssi1 etc for each client)
// docker run -it --rm --name irssi --network host irssi
// /set nick <name>
// /set user_name <user name>
//  /set real_name <real_name
// /connect host.docker.internal <port> <password>

// /join #<channel_name>
// /msg <nick> <message>


// for client locally
// nc localhost <password>
// PASS <pass>
// NICK <nick>
// USER <nick> 0 <localhost> :<first_name last_name>
// OPER 127.0.0.1 pa$$word
// PRIVMSG <nick to be sent to> : message


// JOIN <channel>{,<channel>} [<key>{,<key>}]
// above means list channels seperated by commas and the list their keys
//KICK channel, channel user, user reason
//PART <channel>{,<channel>} [<reason>]
// TOPIC <channel> : [<topic>]
// INVITE <nickname> <channel>
// MODE <target>[<mode string> [<mode arguments>...]]
// ∗ KICK - Eject a client from the channel
// ∗ INVITE - Invite a client to a channel
// ∗ TOPIC - Change or view the channel topic
// ∗ MODE - Change the channel’s mode:
// · i: Set/remove Invite-only channel
// · t: Set/remove the restrictions of the TOPIC command to channel
// operators
// · k: Set/remove the channel key (password)
// · o: Give/take channel operator privilege
// · l: Set/remove the user limit to channel