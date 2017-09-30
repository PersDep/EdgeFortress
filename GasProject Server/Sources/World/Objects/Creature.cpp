#include "Creature.hpp"

#include "Control.hpp"

Creature::Creature() {
	layer = 75;
	name = "Creature";
	const float speed = 4;
	AddComponent(new Control(speed));
}

uint Creature::GetInvisibleVisibility() const { return seeInvisibleAbility; }