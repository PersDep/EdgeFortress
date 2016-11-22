#include "Server.hpp"
#include "Player.hpp"
#include "Network/Network.hpp"
#include "World/World.hpp"
#include "PlayerCommand.hpp"

class Server;

Player::Player(Server *server, sf::TcpSocket *socket) : ckey(""),
                                                        server(server),
                                                        game(nullptr),
                                                        connection(new Connection(socket, server, this)),
                                                        sync(false)
{

}

void Player::Update() {
    while (!commandsFromClient.Empty()) {
        PlayerCommand *temp = commandsFromClient.Pop();
        switch (temp->GetCode()) {
            case PlayerCommand::Code::JOIN: {
                SetMob(game->GetWorld()->CreateNewPlayerMob());
                break;
            }
        }
        if (temp) delete temp;
    }
}

void Player::SendUpdates() {
    if (camera && !camera->IsSuspense()) {
        if (sync) {
            const auto &&differences = camera->GetVisibleDifferences();
            if (!differences.empty()) {
                commandsToClient.Push(new GraphicsDiffsServerCommand(differences));
            }
        } else {
            AddCommandToClient(new GraphicsFullUpdateServerCommand(GetCamera()));
            sync = true;
        }
    }
}

void Player::SetMob(Mob *mob) {
    this->mob = mob;
    SetCamera(new Camera(mob->GetTile()));
};

void Player::AddCommandToClient(ServerCommand *command) {
    commandsToClient.Push(command);
}

void Player::AddCommandFromClient(PlayerCommand *command) {
    commandsFromClient.Push(command);
}

Player::~Player() { connection->Stop(); }