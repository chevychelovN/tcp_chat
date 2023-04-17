#include "client.h"

void Client::run(int port) {
    socket_ptr sock(new boost::asio::ip::tcp::socket(service));
    sock->connect( boost::asio::ip::tcp::endpoint( boost::asio::ip::address::from_string("127.0.0.1"), port));
    BOOST_LOG_TRIVIAL(info) << "Connected to server!";
    init_curses();
    on_login(sock);
    boost::thread t1( [this, sock] {read_handle(sock);});
    boost::thread t2( [this, sock] {write_handle(sock);});
    t1.join();
    t2.join();
}

void Client::send_(const socket_ptr& socket) {
    std::string msg;
    mvwprintw(input_window, 1, 1, "Enter input: ");
    echo();
    char input[50];
    wgetstr(input_window, input);
    msg = input;
    BOOST_LOG_TRIVIAL(debug) << "Send data: " << msg;
    if(msg == "/quit") {
        delwin(input_window);
        delwin(output_window);
        endwin();
        exit(0);
    }
    msg += "\n";
    socket->write_some(boost::asio::buffer(msg));
    werase(input_window);
    box(input_window, 0, 0);
}

void Client::read_(const socket_ptr& socket) {
    boost::asio::streambuf receive_buffer;
    boost::asio::read_until(*socket, receive_buffer,"\0");
    const char* data = boost::asio::buffer_cast<const char*>(receive_buffer.data());
    BOOST_LOG_TRIVIAL(debug) << "Read data: " << data;
    noecho();
    mvwprintw(output_window, current_line, 1, "%s", data);
    current_line++;
    if(current_line == LINES - 11) {
        werase(Client::output_window);
        box(Client::output_window, 0, 0);
        current_line = 1;
        mvwprintw(output_window, current_line, 1, "%s", data);
        current_line++;
        wrefresh(output_window);
    }
    wrefresh(output_window);
}

void Client::on_login(const socket_ptr &socket) {
    try {
        read_(socket);
    } catch (std::exception& e) {
        boost::system::error_code err;
        socket->close(err);
        BOOST_LOG_TRIVIAL(fatal) << "Server is not active";
        exit(1);
    }

    std::string msg;
    mvwprintw(input_window, 1, 1, "Enter input: ");
    echo();
    char input[512];
    wgetstr(input_window, input);
    msg = input;
    BOOST_LOG_TRIVIAL(debug) << "Send username: " << msg;
    if(msg == "/quit") {
        delwin(input_window);
        delwin(output_window);
        endwin();
        exit(0);
    }
    username = msg;
    msg += "\n";
    socket->write_some(boost::asio::buffer(msg));
    werase(input_window);
    box(input_window, 0, 0);
}

void Client::read_handle(const socket_ptr &socket) {
    while(true) {
        try {
            read_(socket);
        } catch (std::exception& e){
            boost::system::error_code err;
            socket->close(err);
            BOOST_LOG_TRIVIAL(fatal) << "Server is not active";
            exit(1);
        }

    }
}

void Client::write_handle(const socket_ptr &socket) {
    while(true) {
        send_(socket);
    }
}

void Client::init_curses() {
    initscr();
    cbreak();

    input_window = newwin(4, COLS, LINES-4, 0);
    box(input_window, 0, 0);
    wrefresh(input_window);

    output_window = newwin(LINES-10, COLS, 0, 0);
    box(output_window, 0, 0);
    wrefresh(output_window);
    curs_set(0);
    BOOST_LOG_TRIVIAL(debug) << "NCurses on";

}


