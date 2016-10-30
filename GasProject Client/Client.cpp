#include "Client.hpp"
#include "Window.hpp"
#include "State.hpp"
#include <net_const.hpp>

ClientController::ClientController() : player(new Player),
                                       window(new Window(this)),
                                       state(new MenuLoginState(this)),
                                       newState(nullptr) {

}

void ClientController::Run() {
    if (!Connection::Start("localhost", PORT, this)) {
		Log::log << "Connection error!" << endl;
    } else {
		Log::log << "Connected" << endl;
    };
    sf::Clock clock;

	
    while (window->isOpen()) {
        sf::Time timeElapsed = clock.restart();
        window->Update(timeElapsed);
        if (newState) {
            state.reset(newState);
            newState = nullptr;
        }
    }
	Connection::Stop();
}

Log Log::log;

int main() {
	ClientController clientController;
    clientController.Run();
	
	return 0;
}