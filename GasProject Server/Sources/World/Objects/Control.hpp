#pragma once

#include "Component.hpp"

#include <SFML/System.hpp>

class Player;

class Control : public Component {
private:
	float speed;
	sf::Vector2i moveIntent;
	sf::Vector2i moveOrder; // receive from player

	Player *player;

public:
	explicit Control(float speed);

	void Update(sf::Time timeElapsed) override;

	void MoveCommand(sf::Vector2i order);

	float GetSpeed() const;
	Player *GetPlayer() const;

	friend Player;
};