#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/server.h"

class MockServer : public Server {
public:
    MOCK_METHOD(void, on_login, (socket_ptr));
    MOCK_METHOD(void, handle_client, (socket_ptr));
};

TEST(ServerTest, Run) {
MockServer server;
EXPECT_CALL(server, on_login(::testing::_)).Times(1);
EXPECT_CALL(server, handle_client(::testing::_)).Times(1);

int port = 8080;
boost::asio::io_service io_service;
boost::asio::ip::tcp::socket socket(io_service);
socket.connect( boost::asio::ip::tcp::endpoint( boost::asio::ip::address::from_string("127.0.0.1"), port));

boost::thread t1( [&server, port] {server.run(port);});
boost::this_thread::sleep_for(boost::chrono::seconds(1));
t1.interrupt();
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);

    return RUN_ALL_TESTS();
}