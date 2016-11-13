#pragma once

#include <string>
#include <list>

#include "Useful.hpp"
#include "Differences.hpp"

using std::string;

struct ClientCommand {
    enum class Code : char {
        AUTH_REQ = 1,
        REG_REQ,
        SERVER_LIST_REQ,
        CREATE_GAME,
        JOIN_GAME,
        DISCONNECT
    };

    virtual const Code GetCode() const = 0;
};

struct AuthorizationClientCommand : public ClientCommand {
    string login;
    string password;

    virtual const Code GetCode() const override { return Code::AUTH_REQ; }

    AuthorizationClientCommand(string login, string password) : login(login),
                                                                password(password) { }
};

struct RegistrationClientCommand : public ClientCommand {
    string login;
    string password;

    virtual const Code GetCode() const override { return Code::REG_REQ; }

    RegistrationClientCommand(string login, string password) : login(login),
                                                               password(password) { }
};

struct GameListClientRequest : public ClientCommand {
    virtual const Code GetCode() const override { return Code::SERVER_LIST_REQ; }
};

struct CreateGameClientCommand : public ClientCommand {
    string title;

    virtual const Code GetCode() const override { return Code::CREATE_GAME; }

    CreateGameClientCommand(string title) : title(title) { }
};

struct JoinGameClientCommand : public ClientCommand {
    int id;

    virtual const Code GetCode() const override { return Code::JOIN_GAME; }

    JoinGameClientCommand(int id) : id(id) { }
};

struct DisconnectionClientCommand : public ClientCommand {
    virtual const Code GetCode() const override { return Code::DISCONNECT; }

    DisconnectionClientCommand() { }
};

struct ServerCommand {
    enum class Code : char {
        AUTH_SUCCESS = 1,
        REG_SUCCESS,
        AUTH_ERROR,
        REG_ERROR,

        GAME_CREATE_SUCCESS,
        GAME_CREATE_ERROR,
        GAME_LIST,
        GAME_JOIN_SUCCESS,
        GAME_JOIN_ERROR,

        GRAPHICS_FULL_UPDATE,
        GRAPHICS_DIFFS,

        COMMAND_CODE_ERROR
        //CONNECTION_ERROR,
    };

    virtual const Code GetCode() const = 0;
};

struct AuthSuccessServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return Code::AUTH_SUCCESS; }
};

struct RegSuccessServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return Code::REG_SUCCESS; }
};

struct AuthErrorServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return Code::AUTH_ERROR; }
};

struct RegErrorServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return Code::REG_ERROR; }
};

struct GameCreateSuccessServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return Code::GAME_CREATE_SUCCESS; }
};

struct GameCreateErrorServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return Code::GAME_CREATE_ERROR; }
};

struct GameJoinSuccessServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return Code::GAME_JOIN_SUCCESS; }
};

struct GameJoinErrorServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return Code::GAME_JOIN_ERROR; }
};

struct GameListServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return Code::GAME_LIST; }
};

class Camera;

struct GraphicsFullUpdateServerCommand : public ServerCommand {
    Camera *camera;
    GraphicsFullUpdateServerCommand(Camera *camera) : camera(camera) { }
    virtual const Code GetCode() const override { return Code::GRAPHICS_FULL_UPDATE; }
};

struct GraphicsDiffsServerCommand : public ServerCommand {
    const std::list<sptr<Diff>> differences;
    GraphicsDiffsServerCommand(const std::list<sptr<Diff>> &differences) : differences(differences) { }
    virtual const Code GetCode() const override { return Code::GRAPHICS_DIFFS; }
};

struct CommandCodeErrorServerCommand : public ServerCommand {
    virtual const Code GetCode() const override { return Code::COMMAND_CODE_ERROR; }
};

//struct ConnectionErrorServerCommand : public ServerCommand {
//    virtual const Code GetCode() const override { return Code::CONNECTION_ERROR; }
//};

