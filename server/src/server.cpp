#include "server.h"

void Server::run(int port) {
    boost::asio::ip::tcp::acceptor acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
    BOOST_LOG_TRIVIAL(info) << "Server started";
    while(true) {
        socket_ptr sock(new boost::asio::ip::tcp::socket(io_service));
        acceptor_.accept(*sock);
        BOOST_LOG_TRIVIAL(debug) << "User connected with socket " << sock;
        on_login(sock);
        boost::thread t1( [this, sock] {handle_client(sock);});
        t1.detach();
    }
}

std::string Server::read_(const socket_ptr& socket) {
    boost::asio::streambuf buf;
    boost::asio::read_until( *socket, buf, "\n" );
    std::string data = boost::asio::buffer_cast<const char*>(buf.data());
    BOOST_LOG_TRIVIAL(info) << "Read data: " << data;
    return data;
}

void Server::send_(const socket_ptr& socket, const std::string &message) {
    const std::string msg = message + "\n";
    BOOST_LOG_TRIVIAL(info) << "Send data: " << msg;
    boost::asio::write( *socket, boost::asio::buffer(message) );
}

void Server::on_login(const socket_ptr &socket) {
    try {
        send_(socket, "Hello from server, enter your name: ");
        std::string username = read_(socket);
        if(!clients.empty()) {
            std::string message = "Now online: ";
            for(const auto& client: clients) {
                message += client.username.substr(0, client.username.size() - 1) + ",";
            }
            message.replace(message.size() - 1, 1, "\n");
            send_(socket, message);
        } else {
            std::string message = "Nobody online right now :(\n";
            send_(socket, message);
        }
        std::string notify = username.substr(0, username.size() - 1) +" entered the chat ^_^\n";
        for(const auto& client: clients) {
            send_(client.socket, notify);
        }
        clients.push_back({socket, username});
        BOOST_LOG_TRIVIAL(debug) << "Last client: " << socket << " " << username;
    } catch(std::exception& e) {
        disconnect(socket);
        return;
    }

}

void Server::handle_client(const socket_ptr& socket) {
    while(true) {
        try {
            std::string message = read_(socket);
            broadcast(socket, message);
        } catch(std::exception& e) {
            disconnect(socket);
            return;
        }
    }
}

void Server::broadcast(const socket_ptr &socket, const std::string &message) {
    std::string name = find_current_client(socket).username;
    name = name.substr(0, name.size() - 1);
    BOOST_LOG_TRIVIAL(debug) << "Broadcast sender name: " << name;
    std::string result =  name + ": " + message;
    BOOST_LOG_TRIVIAL(debug) << "Broadcast result message: " << result;
    for(const auto& client: clients) {
        send_(client.socket, result);
    }
}

void Server::disconnect(const socket_ptr &socket) {
    BOOST_LOG_TRIVIAL(debug) << "Client " << socket << " disconnected";
    std::string message;
    for(int i = 0; i < clients.size(); i++)
    {
        if(clients[i].socket == socket) {
            message += clients[i].username.substr(0, clients[i].username.size() - 1);
            clients.erase(clients.begin() + i);
            BOOST_LOG_TRIVIAL(debug) << "Client " << socket << " removed from vector";
        }
    }
    message += " left the chat\n";
    for(const auto& client: clients) {
        send_(client.socket, message);
    }
    boost::system::error_code err;
    socket->close(err);
}

Client Server::find_current_client(const socket_ptr &socket) {
    try {
        for (auto client: clients) {
            if(client.socket == socket) {
                return client;
            }
        }
    } catch (std::exception& e) {
        BOOST_LOG_TRIVIAL(error) << "Not found socket: " << socket;
        std::cerr << e.what();
        exit(1);
    }
    BOOST_LOG_TRIVIAL(warning) << "Client was not found";
    return {};
}