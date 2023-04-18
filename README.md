# tcp_chat
## Overview
This project is a simple implementation of a chat application using TCP protocol. 
The application allows multiple clients to connect to a server and communicate with each other in real-time.
Clients connect to the server on its port and first authenticate (introduce themselves),
after which they get access to the current list of participants 
and message flow, and can send their message to the general chat room.


Operating System:

• POSIX :: Linux

Programming Language:

• C++20

## Getting started
### Prerequisite
- gcc v11.3+
- cmake v3.25+
- boost v1.74
  - boost::asio
  - boost::log
- gtest
- gmock
- ncurces

### Installation guide

1. Clone repository 
```
git clone https://github.com/chevychelovN/tcp_chat.git
cd tcp_chat
```

2. Building project
```
cmake CMakeLists.txt
make
```


### Running program

After building you can simply start server and client and start chatting!
```
./server_run
./client_run
```

For ending session client can write ```/quit```

You can check logs in client_*.log or server_*.log

### Notes
Features:
- Real-time communication between multiple clients connected to the same server
- Clients can send messages to the server and the server broadcasts the message to all connected clients
- Clients can disconnect from the server using the command ```/quit```

Limitations:
- The application does not support encryption or authentication, so all messages are sent in plaintext and anyone with access to the network traffic can read them
- The application does not have a user interface, so all communication is done through the command-line interface with ncurses style

## Screenshots
![image](https://user-images.githubusercontent.com/86499004/232757266-ecd80252-690d-4fb3-bbb3-15ef53d7ffa5.png)
![image](https://user-images.githubusercontent.com/86499004/232757443-097d76f4-c76e-4199-9282-b4906a4d5628.png)
![image](https://user-images.githubusercontent.com/86499004/232757533-1a33caa9-f489-4c36-8dbd-8e7c3106ce8d.png)
