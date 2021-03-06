#include "Block.hpp"

#include <SFML/System.hpp>

#include "TileGrid.hpp"
#include "Tile.hpp"

Block::Block(TileGrid *tileGrid) :
	tileGrid(tileGrid), relPos(),
	tiles(tileGrid->GetBlockSize())
{
	int y = 0;
	for (auto &vect : tiles) {
		vect.resize(tileGrid->GetBlockSize());
		int x = 0;
		for (auto &tile : vect) {
			tile.reset(new Tile(this, x, y));
			x++;
		}
		y++;
	}
}

void Block::Update(sf::Time timeElapsed) {
    for (auto &vect : tiles)
        for (auto &tile : vect)
            tile->Update(timeElapsed);
}

void Block::Resize(uint tileSize) {
    for (auto &vect : tiles)
        for (auto &tile : vect)
            tile->Resize(tileSize);
}

uf::vec2i Block::GetRelPos() const { return relPos; }

Tile* Block::GetTile(int x, int y) const {
	if (x >= 0 && x < tileGrid->GetBlockSize() && y >= 0 && y < tileGrid->GetBlockSize())
		return tiles[y][x].get();
	return nullptr;
}

TileGrid *Block::GetTileGrid() const { return tileGrid; }