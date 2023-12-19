/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aandom <aandom@student.abudhabi42.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 16:50:39 by aandom            #+#    #+#             */
/*   Updated: 2023/12/02 16:50:39 by aandom           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/Channel.hpp"

Channel::Channel(std::string &name, std::string &key) : _chName(name),
                                                        _chKey(key),
                                                        _chLimit(0),
                                                        _topic("")
{
    _mode['k'] = false;
    _mode['i'] = false;
    _mode['t'] = false;
    _mode['l'] = false;
}

Channel::Channel(const Channel & src) {
	*this = src;
}

Channel &Channel::operator=(const Channel & src) {
	if (this != &src) {
        _chName      =  src.getChName();
        _admins      =  src.getAdmins();
        _members     =  src.getMembers();
        _mode        =  src.getMode();
        _topic       =  src.getTopic();
        _chKey       =  src.getChKey();
        _chLimit     =  src.getChLimit();
        _invitations =  src.getInvitation();
	}
	return *this;
}


Channel::~Channel() {}

std::string const &     Channel::getChName() const { return _chName;}
std::vector <Client *>  Channel::getAdmins() const {return _admins;}
std::vector <Client *>  Channel::getMembers() const {return _members;}
std::map<char, bool>    Channel::getMode() const {return _mode;}
std::vector <int>       Channel::getInvitation() const {return _invitations;}
std::string const &     Channel::getChKey() const {return _chKey;}
std::string const &     Channel::getTopic() const {return _topic;}
size_t                  Channel::getChLimit() const {return _chLimit;}
size_t                  Channel::getChSize() const {return _members.size();}

std::vector<std::string> Channel::getNickNames(int adminIcon) {
    std::vector<std::string> nicknames;
    std::vector <Client *>   admins = getAdmins();
    std::string              name;
    ch_iterator it = _members.begin();
    ch_iterator it_end = _members.end();
    for (; it != it_end; ++it) {
        Client  * user = *it;
        // name = (user == _admin ? "" : "") + user->nickname;
        if (adminIcon)
            name = ((std::find(admins.begin(), admins.end(), user) != admins.end()) ? "@" : "") + user->nickname;
        else
            name = ((std::find(admins.begin(), admins.end(), user) != admins.end()) ? "" : "") + user->nickname;
        nicknames.push_back(name);
    }
    return (nicknames);
}

void    Channel::setChKey(std::string key) {_chKey = key;}
void    Channel::setMode(char c, bool status) {_mode[c] = status;}
void    Channel::setChLimit(size_t limit) {_chLimit = limit;}
void    Channel::setTopic(std::string const & topic) { _topic = topic;}

// void    Channel::broadcast(std::string msg, Client * exclude) {

// }

void    Channel::addClient(Client * client) { _members.push_back(client); }
void    Channel::addAdmin(Client * client) { _admins.push_back(client); }

void    Channel::removeClient(Client * client) {
    Client *newadmin = NULL;
    ch_iterator it = _members.begin();
    ch_iterator it_end = _members.end();
    for (; it != it_end; ++it) {
        if (*it == client) {
            if (isOperator (client) && _admins.size() == 1 && _members.size() > 1) {
                this->removeAddmin(client);
                // if (client != *(_members.begin()))
                //     newadmin = *(_members.begin());
                // else
                //     newadmin = *(_members.begin() + 1);
                newadmin = (client != *(_members.begin())) ? *(_members.begin()) : *(_members.begin() + 1);
                if (newadmin != NULL)
                    this->addAdmin(newadmin);
            }
            _members.erase(it);
            if (_members.size() == 0)
            {
                std::cout << "YOU MIGHT NEED TO DELETE THE CHANNEL AT THIS POINT" << std::endl;
            }
        }
    }

    // client->removeChannel(_chName);
    this->removeAddmin(client);
}

void    Channel::removeAddmin(Client * client) {
    ch_iterator it = _admins.begin();
    ch_iterator it_end = _admins.end();
    for (; it != it_end; ++it) {
        if (*it == client)
            _admins.erase(it);
    }
    // client->removeChannel(_chName);
}


bool    Channel::checkIfMember(const std::string & clientname) {
    std::vector<std::string> nicknames = getNickNames(0);
    if (std::find(nicknames.begin(), nicknames.end(), clientname) != nicknames.end())
        return(true);
    return (false);
}

bool    Channel::isNickIsAdmin(const std::string & clientname) {
    std::string clname = "@" + clientname;
    std::vector<std::string> nicknames = getNickNames(1);
    if (std::find(nicknames.begin() , nicknames.end(), clname) != nicknames.end())
        return(true);
    return (false);
}

bool    Channel::isOperator(Client * src) {
    std::vector<Client *> operators = getAdmins();
    if (std::find(operators.begin(), operators.end(), src) != operators.end())
        return(true);
    return (false);
}

bool    Channel::getmodeAt(char c) {
    return (_mode[c]);
}

 bool    Channel::isInvited(Client * client) {
    std::vector<int> invitees = getInvitation();
    if (std::find(invitees.begin(), invitees.end(), client->fd) != invitees.end())
        return(true);
    return (false);
 }

void    Channel::addToInvitation(Client * client) {
    _invitations.push_back(client->fd);
}

void    Channel::removeFromInvitation(Client * client) {
    // std::vector<int> invitees = getInvitation();
    std::vector<int>::iterator it = std::find(this->_invitations.begin(), this->_invitations.end(), client->fd);
    if (it != this->_invitations.end())
    {
        std::cout << "found the fd to delete " << *it << " " << client->fd << std::endl;
        this->_invitations.erase(it);
        // invitees.erase(it);
    }
}

