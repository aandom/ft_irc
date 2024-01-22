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
	this->operator_password = "pa$$word";
}

Server::~Server() {
	std::cout << "Destructor Called\n";
	for (int i = 1; i < this->nfds; i++)
	{
		close_connection(i);
	}

	for (std::vector<Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
	{
		delete *it;
		this->_channels.erase(it);
	}
	std::cout << "closing socket\n";
	close(this->socket_fd);
	// clear fd, client, channel,
	// what about command objects?
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
		this->operator_password = src.operator_password;
	}
	return *this;
}

void Server::init_error(std::string error) {
	std::cerr << error << std::endl;
	close(this->socket_fd);
	exit(1);
}

// Initializes the socket address structure using getaddrinfo
void Server::init_server_address() {
    struct addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family= AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(NULL, intToStr(this->port).c_str(), &hints, &this->address) != 0)
        init_error("getaddrinfo() failed");
}

// Creates the socket and sets the socket_fd attribute
void Server::create_socket() {
    this->socket_fd = socket(this->address->ai_family, this->address->ai_socktype, this->address->ai_protocol);
    if (this->socket_fd < 0)
        init_error("socket() failed");
}

// Sets socket options such as SO_REUSEADDR and makes the socket non-blocking
void Server::set_socket_options() {
    int on = 1;
    if (setsockopt(this->socket_fd, SOL_SOCKET,  SO_REUSEADDR, &on, sizeof(on)) < 0)
        init_error("setsockopt() failed");

    if (fcntl(this->socket_fd, F_SETFL, O_NONBLOCK) < 0)
        init_error("fcntl() failed");
}

// Binds the socket to the specified address
void Server::bind_socket() {
    if (bind(this->socket_fd, this->address->ai_addr, this->address->ai_addrlen) < 0)
        init_error("bind() failed");
}

// Starts listening for incoming connections on the socket
void Server::start_listening() {
    if (listen(this->socket_fd, 10) < 0)
	{
        init_error("listen() failed");
	}
	std::cout << GREEN << "Server listening on all networks on port " << this->port << RESET << std::endl;
}

// Initializes the poll array with the socket for later use in poll()
void Server::initialize_poll() {
    memset(this->fds, 0 , sizeof(this->fds));
    this->fds[0].fd = this->socket_fd;
    this->fds[0].events = POLLIN | POLLOUT;
}

// Initiates the entire socket setup process
void Server::init_server() {
    init_server_address();
    create_socket();
    set_socket_options();
    bind_socket();
    start_listening();
    initialize_poll();
    freeaddrinfo(this->address);
    this->timeout = (5 * 60 * 1000);
}

void Server::ft_poll() {
	// std::cout << "Waiting on poll()..." << std::endl;
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
	delete this->clients[this->fds[i].fd];
	this->clients.erase(this->fds[i].fd);
	close(this->fds[i].fd);
	this->fds[i].fd = -1;
	this->compress_array = TRUE;
}

void Server::accept_client () {
	std::cout << "Listening socket is readable" << std::endl;
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
	this->close_conn = FALSE;
	std::memset(this->clients[this->fds[i].fd]->buffer, 0, sizeof(this->clients[this->fds[i].fd]->buffer));
	this->rc = recv(this->fds[i].fd, this->clients[this->fds[i].fd]->buffer, sizeof(this->clients[this->fds[i].fd]->buffer) - 2, 0);
	if (this->rc < 0) {
		if (errno != EWOULDBLOCK) {
			perror("  recv() failed");
			this->close_conn = TRUE;
		}
	}
	this->clients[this->fds[i].fd]->str += this->clients[this->fds[i].fd]->buffer;
	if (this->rc == 0) {
		std::cout << "  Connection closed" << std::endl;
		this->close_conn = TRUE;
	}
	std::cout << "Received " << this->clients[this->fds[i].fd]->str.length() << " bytes in the below string" << std::endl << this->clients[this->fds[i].fd]->str;
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
	if (this->close_conn) {
		close(this->fds[i].fd);
		this->fds[i].fd = -1;
		this->compress_array = TRUE;
	}
}

void Server::compress_fds () {
	std::cout << "Compressing fds called " << std::endl;
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
		std::cout << "is_authenticated: " << it->second->is_authenticated << std::endl;
		std::cout << "is_operator: " << it->second->is_operator << std::endl;
		std::cout << std::endl;
	}
}

void Server::main_loop() {
	while (this->end_server == FALSE && g_endServer == FALSE)
	{
		ft_poll();
		for (int i = 0; i < this->nfds; i++)
		{
			// std::cout << "fd = "<< this->fds[i].fd << "  revent value  :  " << this->fds[i].revents  << " and i = " << i << std::endl;
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
	}
}

// channel related
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