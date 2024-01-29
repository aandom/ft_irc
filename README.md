# IRC Project

### Installation

1. Clone the repository:

    ```bash
    git clone https://github.com/aandom/ft_irc.git
    

2. Navigate to the project directory:

    ```bash
    cd ft_irc
    

3. Build the project:

    ```bash
    make
## Usage

### Start Server
```bash
./ircserv <port> <password>
```
### Connect Client Locally from host machine
```bash
nc localhost <password>
PASS <pass>
NICK <nick>
USER <username> 0 <localhost> :<first_name last_name>
```

### Connect Client using Irssi
To connect an Irssi client, either you have to download Irssi in your machine or you can work in docker container. For docker container you can use 42-Valgrind container(the docker rule in the makefile will create to you multiple container so that you can use one for the server and others for multiple clients). When you run your server follow the below steps to connect clients. 
```bash
irssi
/set nick <name>
/set user_name <user name>
/set server_reconnect_time -1
/set real_name <real_name>
/connect localhost <port> <password>
```
Alternativly you can create an irssi container using the below steps 
```bash
docker run -it --rm --name irssi --network host irssi
/set nick <name>
/set user_name <user name>
/set real_name <real_name>
/connect host.docker.internal <port> <password>
```
### Commands in Irssi
```bash
/oper 127.0.0.1 pa$$word  # Shows @ operator name
/kill <nick> reason
/join <channel>{,<channel>} [<key>{,<key>}]
/msg <nick or channel> <message>
/notice <nick or channel> <message>
/kick channel, channel user, user reason
/part <channel>{,<channel>} [<reason>]
/topic <channel> : [<topic>]
/invite <nickname> <channel>
/mode<target>[<mode string> [<mode arguments>...]]
/disconnect
/quit
```

### Commands Locally
```bash
PRIVMSG <nick to be sent to> : message
NOTICE <nick to be sent to> : message
JOIN <channel>{,<channel>} [<key>{,<key>}]
KICK channel, channel user, user reason
PART <channel>{,<channel>} [<reason>]
TOPIC <channel> : [<topic>]
INVITE <nickname> <channel>
MODE <target>[<mode string> [<mode arguments>...]]
OPER 127.0.0.1 pa$$word
kill <nick> reason
QUIT
DISCONNECT

```
## External Functions

### `getaddrinfo`

Purpose: Resolves a hostname and service into address structures.

getaddrinfo(NULL, intToStr(this->port).c_str(), &hints, &this->address
Parameters:

- `node`: The hostname or IP address.
- `service`: The service name or port number.
- `hints`: A pointer to an `addrinfo` structure with hints.
- `res`: A pointer to the result.

Return: Returns 0 on success, non-zero on error.

### `socket`

Purpose: Creates a new socket.

this->socket_fd = socket(this->address->ai_family, this->address->ai_socktype, this->address->ai_protocol)

Parameters:
- `domain`: The protocol family (e.g., AF_INET for IPv4, AF_INET6 for IPv6).
- `type`: The socket type (e.g., SOCK_STREAM for TCP, SOCK_DGRAM for UDP).
- `protocol`: Specific protocol to be used (typically 0 for default).

Return: Returns a socket descriptor, or -1 on error.

### `setsockopt`

Purpose: Sets options for a socket.

Parameters:
- `socket`: The socket descriptor.
- `level`: The protocol level at which the option resides.
- `option_name`: The specific option to set.
- `option_value`: Pointer to the option value.
- `option_len`: Size of the option value.

Return: Returns 0 on success, -1 on error.

### `bind`

Purpose: Associates a socket with a specific local address and port.

Parameters:
- `socket`: The socket descriptor.
- `address`: The local address to bind to.
- `address_len`: The size of the local address structure.

Return: Returns 0 on success, -1 on error.

### `fcntl`

Purpose: Performs various operations on a file descriptor.

Parameters:
- `fd`: The file descriptor.
- `cmd`: The operation to perform.
- `arg`: Additional argument depending on the operation.

Return: Returns the result of the operation, or -1 on error.

### `poll`

Purpose: Waits for events on a set of file descriptors.

Parameters:
- `fds`: An array of structures of type `struct pollfd`.
- `nfds`: The number of file descriptors in the `fds` array.
- `timeout`: The maximum time (in milliseconds) that poll should wait for an event to occur.

Return: Returns the number of file descriptors that have events or errors, or one of the following values:
- -1: An error occurred, and `errno` is set to indicate the error.
- 0: The timeout specified by the `timeout` parameter expired, and no file descriptors had events.

### `send`, `recv`

Purpose: Send and receive data on a connected socket.

Parameters:
- `socket`: The socket descriptor.
- `buf`: Pointer to the data buffer.
- `len`: The length of the data buffer.
- `flags`: Additional flags.

Return: Returns the number of bytes sent/received, or -1 on error.

### `close`

Purpose: Closes a file descriptor (including sockets).

Parameters:
- `fd`: The file descriptor to close.

Return: Returns 0 on success, -1 on error.

## References

1. https://modern.ircdocs.horse/
2. https://www.rfc-editor.org
