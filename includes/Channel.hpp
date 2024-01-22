/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aandom <aandom@student.abudhabi42.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 16:41:32 by aandom            #+#    #+#             */
/*   Updated: 2023/12/02 16:41:32 by aandom           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Client.hpp"
# include "Server.hpp"

class Client;

class Channel
{
    typedef std::vector<Client *>::iterator ch_iterator;
    private:
        std::string                 _chName;
        std::vector <Client *>      _admins;
        std::vector <Client *>      _members;
        std::map<char, bool>        _mode;
        std::string                 _chKey;
        size_t                      _chLimit;
        std::string                 _topic;
        std::vector <int>           _invitations;

        Channel();
        
        
    public:
        Channel(std::string &name, std::string &key);
        Channel(const Channel & src);
        Channel & operator=(const Channel & src);
        ~Channel();

        std::string const &     getChName() const;
        std::vector <Client *>  getAdmins() const;
        std::vector <Client *>  getMembers() const;
        std::map<char, bool>    getMode() const;
        std::vector <int>       getInvitation() const;
        std::string const &     getChKey() const;
        std::string const &     getTopic() const;
        size_t                  getChLimit() const;
        size_t                  getChSize() const;
        std::vector<std::string> getNickNames(int adminIcon);

        void    setChKey(std::string key);
        void    setMode(char c, bool status);
        void    setChLimit(size_t limit);
        void    setTopic(std::string const & topic);

        void    broadcast(std::string msg, Client * exclude);

        void    addClient(Client * client);
        void    addAdmin(Client * client);
        void    removeClient(Client * client);
        void    chick(Client * client, Client * target, std::string reason);
        bool    checkIfMember(const std::string & src);
        bool    isOperator(Client * src);
        bool    getmodeAt(char c);
        bool    isInvited(Client * clinet);
        bool    isNickIsAdmin(const std::string & clientname);
        void    removeAddmin(Client * client);
        
        void    addToInvitation(Client * client);
        void    removeFromInvitation(Client * client);
};

#endif