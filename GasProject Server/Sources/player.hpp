#pragma once

#include <string>

#include "Common/Useful.hpp"

using std::string;

namespace sf {
    class TcpSocket;
}

class Server;
class Game;
class Connection;
struct ServerCommand;

class Player {
private:
    string ckey;
    Server *server;
    Game *game;
    uptr<Connection> connection;

    ThreadSafeQueue<ServerCommand *> commandQueue;

public:
    Player(Server *server, sf::TcpSocket *socket);

    string GetCKey() { return ckey; }

    friend Connection;
};