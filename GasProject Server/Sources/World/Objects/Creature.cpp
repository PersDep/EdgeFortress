#include "Creature.hpp"

#include <Server.hpp>
#include <Network/Differences.hpp>
#include <World/Map.hpp>
#include <World/Tile.hpp>
#include <World/Objects/Control.hpp>

Creature::Creature() {
	layer = 75;
	name = "Creature";
	const float speed = 4;
	seeInvisibleAbility = 0;
	AddComponent(new Control(speed));
}

void Creature::Update(sf::Time timeElapsed) {
	Object::Update(timeElapsed);
	if (stun > timeElapsed)
		stun -= timeElapsed;
	else
		stun = sf::Time::Zero;
}

bool Creature::InteractedBy(Object *) {
    Server::log << "Creature clicked" << std::endl;
	return true;
}

void Creature::Move(uf::vec2i order) {
	if (!order || IsStunned())
		return;

	// Form the intent based on the order
	SetDirection(uf::VectToDirection(order));

	Tile *tile = GetTile();
	if (tile) {
		uf::vec2i moveIntent;

		if (order.x) moveIntent.x = order.x;
		if (order.y) moveIntent.y = order.y;

		Tile *newTileDiag = tile->GetMap()->GetTile(tile->GetPos() + moveIntent);
		Tile *newTileX = tile->GetMap()->GetTile({ tile->GetPos().x + moveIntent.x, tile->GetPos().y });
		Tile *newTileY = tile->GetMap()->GetTile({ tile->GetPos().x, tile->GetPos().y + moveIntent.y });

		if (GetDensity()) {
			if (!newTileDiag || newTileDiag->IsDense()) moveIntent = GetMoveIntent();
			if (!newTileX || newTileX->IsDense()) moveIntent.x = 0;
			if (!newTileY || newTileY->IsDense()) moveIntent.y = 0;
		}

		SetMoveIntent(moveIntent);
	}
}

bool Creature::TryInteractWith(Object *obj) { 
	if (!obj || IsStunned())
		return false;
	return true;
}

void Creature::Stun() {
	GetTile()->GetBlock()->AddDiff(new StunnedDiff(this, sf::seconds(3)));
	stun = sf::seconds(3);
    Server::log << "Creature stunned" << std::endl;
}

bool Creature::PutOn(Item *) {
	return false;
}

uint Creature::GetInvisibleVisibility() const { return seeInvisibleAbility; }

bool Creature::IsStunned() const { return stun > sf::Time::Zero; }
