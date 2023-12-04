#include "includes/Client.hpp"

Client::Client(int fd, char *client_ip) {
	this->fd = fd;
	this->client_ip = client_ip;
	this->is_registered = false;
}

Client::Client(Client const &src) {
	*this = src;
}

Client &Client::operator=(Client const &src) {
	if (this != &src)
	{
		this->fd = src.fd;
		this->client_ip = src.client_ip;
		this->nickname = src.nickname;
		this->username = src.username;
		this->realname = src.realname;
		this->hostname = src.hostname;
		this->servername = src.servername;
		this->password = src.password;
		this->is_registered = src.is_registered;
		this->away = src.away;
		this->mode = src.mode;
		this->ip = src.ip;
	}
	return *this;
}

Client::~Client() { }
