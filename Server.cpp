#include "includes/Server.hpp"

Server::Server(char *argv[]) {
	this->socket_fd = -1;
	this->port = std::atoi(argv[1]);
	this->new_sd = -1;
	this->rc = 1;
	this->timeout = 0;
	this->password = argv[2];
	this->nfds = 1;
	this->on = 1;
	this->end_server = FALSE;
	this->close_conn = FALSE;
	this->compress_array = FALSE;
}

Server::~Server() {
	for (int i = 0; i < this->nfds; i++)
	{
		if(this->fds[i].fd >= 0)
			close(this->fds[i].fd);
	}
}

Server::Server(Server const &src) {
	*this = src;
}

Server &Server::operator=(Server const &src) {
	if (this != &src)
	{
		this->socket_fd = src.socket_fd;
		this->port = src.port;
		this->new_sd = src.new_sd;
		this->rc = src.rc;
		this->timeout = src.timeout;
		this->password = src.password;
		this->nfds = src.nfds;
		this->on = src.on;
		this->end_server = src.end_server;
		this->close_conn = src.close_conn;
		this->compress_array = src.compress_array;
	}
	return *this;
}

void Server::init_error(std::string error) {
	std::cerr << error << std::endl;
	close(this->socket_fd);
	exit(1);
}

void Server::init_socket () {
	struct addrinfo	hints;

	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = IPPROTO_TCP;
	if (getaddrinfo(NULL, std::to_string(this->port).c_str(), &hints, &this->address) != 0)
		init_error("getaddrinfo() failed");
	this->socket_fd = socket(this->address->ai_family, this->address->ai_socktype, this->address->ai_protocol);
	if (this->socket_fd < 0)
		init_error("socket() failed");
	int on = 1;
	if (setsockopt(this->socket_fd, SOL_SOCKET,  SO_REUSEADDR, &on, sizeof(on)) < 0)
		init_error("setsockopt() failed");
	if (fcntl(this->socket_fd, F_SETFL, O_NONBLOCK) < 0)
		init_error("fcntl() failed");
	if (bind(this->socket_fd, this->address->ai_addr, this->address->ai_addrlen) < 0)
		init_error("bind() failed");
	if (listen(this->socket_fd, 10) < 0)
		init_error("listen() failed");
	// freeaddrinfo(this->address);
	memset(this->fds, 0 , sizeof(this->fds));
	this->fds[0].fd = this->socket_fd;
	this->fds[0].events = POLLIN;
	this->timeout = (5 * 60 * 1000);
}

void Server::ft_poll() {
	// std::cout << "Waiting on poll()..." << std::endl;
	this->rc = poll(this->fds, this->nfds, -1);
	if (this->rc < 0)	{
		perror("  poll() failed");
		this->end_server = TRUE;
	}
	if (this->rc == 0) {
		std::cout << "  poll() timed out.  End program." << std::endl;
		this->end_server = TRUE;
	}
}

void Server::close_connection(int i) {
	std::cout << "Closing connection with fd: " << this->fds[i].fd << std::endl;
	close(this->fds[i].fd);
	this->fds[i].fd = -1;
	this->compress_array = TRUE;
	delete this->clients[this->fds[i].fd];
}

void Server::accept_client () {
	// std::cout << "  Listening socket is readable" << std::endl;
	while (true)
	{
		struct sockaddr_in	new_client_addr;
		socklen_t			new_client_addr_size;

		new_client_addr_size = sizeof(new_client_addr);	
		this->new_sd = accept(this->socket_fd, (struct sockaddr *)&new_client_addr, &new_client_addr_size);
		if (this->new_sd < 0) {
			if (errno != EWOULDBLOCK) {
				perror("  accept() failed");
				this->end_server = TRUE;
			}
			return;
		}
		std::cout << "Client IP: " << inet_ntoa(new_client_addr.sin_addr) << std::endl;
		std::cout << "Client fd: " << this->new_sd << std::endl;
		this->clients[this->new_sd] = new Client(this->new_sd, inet_ntoa(new_client_addr.sin_addr));
		this->fds[this->nfds].fd = this->new_sd;
		this->fds[this->nfds].events = POLLIN;
		this->nfds++;
	}
}

void Server::read_client (int i) {
	std::cout << "  Descriptor " << this->fds[i].fd << " is readable" << std::endl;
	this->close_conn = FALSE;
	std::string str;
	while (TRUE) 
	{
		this->rc = recv(this->fds[i].fd, this->buffer, sizeof(this->buffer), 0);
		if (this->rc < 0) {
			if (errno != EWOULDBLOCK) {
				perror("  recv() failed");
				this->close_conn = TRUE;
			}
			break;
		}
		str += this->buffer;
		if (this->rc == 0) {
			std::cout << "  Connection closed" << std::endl;
			this->close_conn = TRUE;
			break;
		}
	}
	std::cout << str.size() << " bytes received from " << this->fds[i].fd << "\n" << str << std::endl;
	Command cmd(this, this->clients[this->fds[i].fd], str);
	cmd.parse_command();
	cmd.executeCommand();
	if (this->close_conn) {
		close(this->fds[i].fd);
		this->fds[i].fd = -1;
		this->compress_array = TRUE;
	}
}

void Server::compress_fds () {
	this->compress_array = FALSE;
	for (int i = 0; i < this->nfds; i++)
	{
		if (this->fds[i].fd == -1)
		{
			for(int j = i; j < this->nfds; j++)
			{
				this->fds[j] = this->fds[j+1];
			}
			i--;
			this->nfds--;
		}
	}
}

void Server::printClients () {
	for (std::map<int, Client *>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
	{
		std::cout << "fd: " << it->second->fd << std::endl;
		std::cout << "nickname: " << it->second->nickname << std::endl;
		std::cout << "username: " << it->second->username << std::endl;
		std::cout << "realname: " << it->second->realname << std::endl;
		std::cout << "hostname: " << it->second->hostname << std::endl;
		std::cout << "servername: " << it->second->servername << std::endl;
		std::cout << "client_ip: " << it->second->client_ip << std::endl;
		std::cout << "is_registered: " << it->second->is_registered << std::endl;
		std::cout << "away: " << it->second->away << std::endl;
		std::cout << "mode: " << it->second->mode << std::endl;
		std::cout << "ip: " << it->second->ip << std::endl;
	}
}

void Server::ft_irc() {
	while (this->end_server == FALSE) 
	{
		ft_poll();
		for (int i = 0; i < this->nfds; i++)
		{
			// std::cout << "i = " << i << " " << "event: " << this->fds[i].revents << std::endl;
			if(this->fds[i].revents == 0)
				continue;
			else if (this->fds[i].revents == (POLLIN | POLLHUP))
				close_connection(i);
			else if (this->fds[i].fd == this->socket_fd)
				accept_client();
			else
				read_client(i);
		}
		if (this->compress_array)
			compress_fds();
		// printClients();
	}
}
