#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include <iostream>
#include <exception>
#include <ncurses.h>
#include <boost/thread.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>
#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>

typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

class Client final {
public:
    Client() = default;
    ~Client() = default;
    void run(int port = 1234);

private:
    void read_(const socket_ptr& socket);
    void send_(const socket_ptr& socket);
    void on_login(const socket_ptr& socket);
    void read_handle(const socket_ptr& socket);
    void write_handle(const socket_ptr& socket);
    void init_curses();


    boost::asio::io_service service;
    std::string username;
    WINDOW *output_window;
    WINDOW *input_window;
    int current_line = 1;
    bool curses_started = false;
};


#endif //CLIENT_CLIENT_H
