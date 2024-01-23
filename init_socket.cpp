#include "includes/Server.hpp"

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