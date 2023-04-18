#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include <iostream>
#include <stdexcept>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

struct Client {
    socket_ptr socket;
    std::string username;
};


class Server {
public:
    Server() = default;
    ~Server() = default;
    void run(int port = 1234);

private:
    static std::string read_(const socket_ptr& socket);
    static void send_(const socket_ptr& socket, const std::string& message);
    void on_login(const socket_ptr& socket);
    void handle_client(const socket_ptr& socket);
    void broadcast(const socket_ptr& socket, const std::string& message);
    void disconnect(const socket_ptr& socket);
    Client find_current_client(const socket_ptr& socket);

    boost::asio::io_service io_service;
    std::vector<Client> clients;
};


#endif //SERVER_SERVER_H
