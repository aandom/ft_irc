#include "includes/Parse.hpp"
#include <sstream>

Parse::Parse(Server *server, Client *client, std::string str) {
	this->server = server;
	this->client = client;
	this->str = str;
}

Parse::~Parse() { }

// Parse::Parse(Parse const &src) {
// 	*this = src;
// }

// Function to tokenize an IRC message
std::vector<std::string> Parse::tokenizeMessage() {
    std::vector<std::string> tokens;
    std::istringstream iss(this->str);
    std::string token;
    
    while (iss >> token) {
        tokens.push_back(token);
    }
    
    return tokens;
}

void Parse::sendErrorResponse(const std::string& message) {
	// std::cout << "ERROR: " << message << std::endl;
	std::string response = "ERROR: " + message + "\r\n";
	int ret = send(client->fd, response.c_str(), response.length(), 0);
	if (ret == -1)
		std::cout << "ERROR: " << strerror(errno) << std::endl;
}

void Parse::sendResponse(std::string message) {
	std::string response = message + "\r\n";
	int ret = send(client->fd, response.c_str(), response.length(), 0);
	if (ret == -1)
		std::cout << "ERROR: " << strerror(errno) << std::endl;
}

bool Parse::isUniqueNickname(std::string nickname) {
	for (std::map<int, Client *>::iterator it = server->clients.begin(); it != server->clients.end(); it++)
	{
		if (it->second->fd == client->fd)
			continue;
		if (it->second->nickname == nickname)
			return false;
	}
	return true;
}

void Parse::handleNickCommand(std::vector<std::string> tokens) {
	if (tokens.size() >= 1) {
		std::string nickname = tokens[0];
		// Validate and store the nickname
		if (!isUniqueNickname(nickname)) {
			sendErrorResponse("Nickname is already in use");
		} else {
			client->nickname = nickname;
			sendResponse("Nickname set successfully");
		}
	} else {
		// Invalid NICK command format
		sendErrorResponse("Invalid NICK command format");
	}
}

void Parse::parse_command() {
	std::vector<std::string> tokens = tokenizeMessage();
    // Check if the message has enough tokens
    if (tokens.empty()) {
        // Invalid message
        sendErrorResponse("Invalid message format");
        return;
    }
    // Identify the IRC command
    this->command = tokens[0];
	if (client->is_registered == false) {
		if (this->command != "CAP" && this->command != "PASS" && this->command != "NICK" && this->command != "USER")
		{
			sendErrorResponse("You are not registered");
			return;
		}
    // Dispatch based on the command
		if (command == "NICK") {
			tokens.erase(tokens.begin());
			handleNickCommand(tokens);
		}
		// else if (command == "USER") {
		// 	tokens.erase(tokens.begin());
		// 	handleUserCommand(tokens);
		// } else if (command == "CAP") {
		// 	tokens.erase(tokens.begin());
		// 	handleCapCommand(tokens);
		// } else {
		// 	// Unhandled command
		// 	sendErrorResponse("Unhandled command");
		// }
	}
}

/*
// Function to handle the NICK command
void handleNickCommand(const std::vector<std::string>& params) {
    if (params.size() >= 1) {
        std::string nickname = params[0];

        // Validate and store the nickname
        if (isUniqueNickname(nickname)) {
            storeNickname(nickname);
            sendResponse("Nickname set successfully");
        } else {
            sendErrorResponse("Nickname is already in use");
        }
    } else {
        // Invalid NICK command format
        sendErrorResponse("Invalid NICK command format");
    }
}

// Function to handle the USER command
void handleUserCommand(const std::vector<std::string>& params) {
    if (params.size() >= 4) {
        std::string username = params[0];
        std::string hostname = params[1];
        std::string servername = params[2];
        std::string realname = params[3];

        // Validate and store user information
        // (username, hostname, servername, realname)
        // Send appropriate responses
        storeUserInfo(username, hostname, servername, realname);
        sendResponse("User information set successfully");
    } else {
        // Invalid USER command format
        sendErrorResponse("Invalid USER command format");
    }
}

// Function to handle the CAP command
void handleCapCommand(const std::vector<std::string>& params) {
    // Implement CAP command handling
    // You can negotiate capabilities with the client here
    // and respond accordingly
}

// Example function to process an IRC message
void processMessage(const std::string& message) {
    // Tokenize the message
    std::vector<std::string> tokens = tokenizeMessage(message);

    // Check if the message has enough tokens
    if (tokens.empty()) {
        // Invalid message
        sendErrorResponse("Invalid message format");
        return;
    }

    // Identify the IRC command
    std::string command = tokens[0];

    // Dispatch based on the command
    if (command == "NICK") {
        tokens.erase(tokens.begin()); // Remove the command
        handleNickCommand(tokens);
    } else if (command == "USER") {
        tokens.erase(tokens.begin()); // Remove the command
        handleUserCommand(tokens);
    } else if (command == "CAP") {
        tokens.erase(tokens.begin()); // Remove the command
        handleCapCommand(tokens);
    } else {
        // Unhandled command
        sendErrorResponse("Unhandled command");
    }
}

int main() {
    // Example usage
    std::string message = "NICK mynickname";
    processMessage(message);

    message = "USER myusername myhostname myservername :My Real Name";
    processMessage(message);

    message = "CAP LS";
    processMessage(message);

    return 0;
}

*/