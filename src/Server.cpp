#include "../includes/Server.hpp"
/*##############################################################################################################
 												SERVER CONSTRUCTOR
###############################################################################################################*/
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
	this->compress_array = FALSE;
	this->operator_password = "pa$$word";
}

Server::~Server() {
	std::cout << "Destructor Called\n";
	for (int i = 1; i < this->nfds; i++)
	{
		close_connection(i);
	}

	for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++) {
		delete (*it);
	}
	std::cout << "closing socket\n";
	if (close(this->socket_fd) < 0)
		std::cout << "ERROR: " << strerror(errno) << std::endl;
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
		this->compress_array = src.compress_array;
		this->operator_password = src.operator_password;
	}
	return *this;
}

/*##############################################################################################################
 													POLL EVENTS
###############################################################################################################*/
void Server::ft_poll() {
	this->rc = poll(this->fds, this->nfds, -1);
	if (this->rc < 0)	{
		perror(" at poll()");
		this->end_server = TRUE;
	}
	if (this->rc == 0) {
		std::cout << "  poll() timed out.  End program." << std::endl;
		this->end_server = TRUE;
	}
}

void Server::close_connection(int i) {
	std::cout << "Closing connection with fd: " << this->clients[this->fds[i].fd]->fd << std::endl;
	quit(*this, this->clients[this->fds[i].fd]);
	delete this->clients[this->fds[i].fd];
	this->clients.erase(this->fds[i].fd);
	close(this->fds[i].fd);
	this->fds[i].fd = -1;
	this->compress_array = TRUE;
}

void Server::accept_client () {
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
		std::cout << "\033[32mClient IP: " << inet_ntoa(new_client_addr.sin_addr) << RESET << std::endl;
		std::cout << "\033[32mClient fd: " << this->new_sd << RESET << std::endl;
		this->clients[this->new_sd] = new Client(this->new_sd, inet_ntoa(new_client_addr.sin_addr));
		this->fds[this->nfds].fd = this->new_sd;
		this->fds[this->nfds].events = POLLIN;
		this->nfds++;
	}
}

void Server::read_client (int i) {
	bool is_quit = false;
	std::memset(this->clients[this->fds[i].fd]->buffer, 0, sizeof(this->clients[this->fds[i].fd]->buffer));
	this->rc = recv(this->fds[i].fd, this->clients[this->fds[i].fd]->buffer, sizeof(this->clients[this->fds[i].fd]->buffer) - 2, 0);
	if (this->rc < 0) {
		if (errno != EWOULDBLOCK) {
			perror("  recv() failed");
			this->close_connection(i);
		}
	}
	this->clients[this->fds[i].fd]->str += this->clients[this->fds[i].fd]->buffer;
	if (this->rc == 0) {
		this->close_connection(i);
		return;
	}
	if (this->clients[this->fds[i].fd]->str.find('\n') != std::string::npos)
	{
		std::vector<std::string> commands = splitString(this->clients[this->fds[i].fd]->str, '\n');
		for (std::vector<std::string>::iterator it = commands.begin(); it != commands.end(); it++)
		{
			Command cmd(this, this->clients[this->fds[i].fd], *it, i);
			cmd.parse_command();
			cmd.executeCommand();
			if (cmd.command == "QUIT")
				is_quit = true;
		}
		if (!is_quit && this->clients[this->fds[i].fd]->str.empty() == false)
			this->clients[this->fds[i].fd]->str.clear();
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

void Server::main_loop() {
	while (this->end_server == FALSE && g_endServer == FALSE)
	{
		ft_poll();
		for (int i = 0; i < this->nfds; i++)
		{
			if(this->fds[i].revents == 0)
				continue;
			else if (this->fds[i].fd == this->socket_fd)
				accept_client();
			else
				read_client(i);
		}
		if (this->compress_array)
			compress_fds();
	}
}

/*##############################################################################################################
 													CHANNEL RELATED
###############################################################################################################*/
void	Server::addChannel(Channel * channel) { _channels.push_back(channel); }

std::vector<Channel *> & Server::getChannels() { return _channels;}
std::map<int, Client *> & Server::getClients() { return clients;}

void	Server::removeChannel(Channel * channel) {
	ch_iterator it = _channels.begin();
    ch_iterator it_end = _channels.end();
    for (; it != it_end; ++it) {
        if (*it == channel)
            _channels.erase(it);
    }
}

std::vector<std::string> Server::getChannelNames() {
    std::vector<std::string> chNames;
    std::string              name;
	std::vector<Channel *> _members = getChannels();
    ch_iterator it = _members.begin();
    ch_iterator it_end = _members.end();
    for (; it != it_end; ++it) {
        Channel  * channel = *it;
        chNames.push_back(channel->getChName());
    }
    return (chNames);
}

Channel *	Server::getChannelIfExist(std::string const & cname) {
    std::vector<Channel *> channels = getChannels();
    ch_iterator it = channels.begin();
    ch_iterator it_end = channels.end();
    for (; it != it_end; ++it) {
        Channel  * channel = *it;
        if (channel->getChName() == cname)
			return (channel);
    }
	return (NULL);
}

Client *	Server::getClientIfExist(std::string const & cname) {
    std::map<int, Client *> clients = getClients();
    cl_iterator it = clients.begin();
    cl_iterator it_end = clients.end();
    for (; it != it_end; ++it) {
        Client  * client = it->second;
        if (client->nickname == cname)
			return (client);
    }
	return (NULL);
}


bool	Server::checkIfClientExists(std::string const & cname) {
    std::vector<std::string> chNames = getChannelNames();
    if (std::find(chNames.begin(), chNames.end(), cname) != chNames.end())
        return(true);
    return (false);
}