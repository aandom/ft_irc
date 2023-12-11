#pragma once

#include "Server.hpp"
#include "Client.hpp"
#include "Command.hpp"

void 						sendResponse1(std::string message, Client *client);
void 						serverReply(std::string code, std::string message, Client *client);
void 						sendResponse(std::string message, Client *client);
std::vector<std::string> 	tokenizeMessage(std::string str);
bool						isUniqueNickname(std::string nickname, std::map<int, Client *> clients, Client *client);
void 						registrationReply(Client *client);
void 						UserToUserMessage(std::string message, Client *src, Client *dst);