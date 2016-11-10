#include <iostream>

#include "Common/Useful.hpp"
#include "Common/NetworkConst.hpp"
#include "World.hpp"
#include "Server.hpp"
#include "Player.hpp"

Object::Object() {
    tile = nullptr;
}

Object::Object(Tile *tile) : tile(nullptr) {
    if (tile)
        tile->AddObject(this);
}

Tile::Tile(Map *map, int x, int y) :
    map(map), x(x), y(y)
{
    sprite = Global::Sprite::Space;
}

void Tile::AddObject(Object *obj) {
    Tile *lastTile;
    bool moved = false;
    if (lastTile = obj->GetTile()) {
        int n = 0;
        for (auto &i : lastTile->content) {
            if (i.get() == obj) {
                i.release();
                lastTile->content.remove(i);

                Block *block = lastTile->GetBlock();
                int x = lastTile->X() % Global::BLOCK_SIZE;
                int y = lastTile->Y() % Global::BLOCK_SIZE;
                if (GetBlock() == block) {
                    int toX = X() % Global::BLOCK_SIZE;
                    int toY = Y() % Global::BLOCK_SIZE;
                    block->AddDiff(MoveDiff(block, x, y, n, toX, toY));
                    moved = true;
                } else {
                    block->AddDiff(RemoveDiff(block, x, y, n));
                }
                
                break;
            }
            n++;
        }
    }
    obj->SetTile(this);
    content.push_back(uptr<Object>(obj));

    if (!moved) {
        Block *block = GetBlock();
        int x = X() % Global::BLOCK_SIZE;
        int y = Y() % Global::BLOCK_SIZE;
        block->AddDiff(AddDiff(block, x, y, int(content.size() - 1)));
    }
}

bool Tile::RemoveObject(Object *obj) {
    for (auto &i : content)
        if (i.get() == obj) {
            i.release();
            content.remove(i);
            obj->SetTile(nullptr);
            return true;
        }
    return false;
}

Block *Tile::GetBlock() const {
    return map->GetBlock(x / Global::BLOCK_SIZE, y / Global::BLOCK_SIZE);
}

Block::Block(Map *map, int blockX, int blockY) :
    map(map), blockX(blockX), blockY(blockY),
    size(Global::BLOCK_SIZE),
    tiles(size, vector<Tile *>(size))
{
    int y = 0;
    for (vector<Tile *> &vect : tiles) {
        int x = 0;
        for (Tile *&tile : vect) {
            tile = map->GetTile(x + blockX * size, y + blockY * size);
            x++;
        }
        y++;
    }
}

Map::Map(const int sizeX, const int sizeY) : 
    sizeX(sizeX), sizeY(sizeY),
    numOfBlocksX(sizeX / Global::BLOCK_SIZE + (sizeX % Global::BLOCK_SIZE ? 1 : 0)),
    numOfBlocksY(sizeY / Global::BLOCK_SIZE + (sizeY % Global::BLOCK_SIZE ? 1 : 0)),
    tiles(sizeY),
    blocks(numOfBlocksY)
{
    for (vector<uptr<Tile>> &vect : tiles) {
        vect = vector<uptr<Tile>>(sizeX);
    }

    int y = 0;
    for (vector<uptr<Tile>> &vect : tiles) {
        int x = 0;
        for (uptr<Tile> &tile : vect) {
            tile.reset(new Tile(this, x, y));
            x++;
        }
        y++;
    }

    for (vector<uptr<Block>> &vect : blocks) {
        vect = vector<uptr<Block>>(numOfBlocksY);
    }

    y = 0;
    for (vector<uptr<Block>> &vect : blocks) {
        int x = 0;
        for (uptr<Block> &tile : vect) {
            tile.reset(new Block(this, x, y));
            x++;
        }
        y++;
    }

    Server::log << "Map created: " << sizeX << "x" << sizeY << " (" << numOfBlocksX << "x" << numOfBlocksY << " blocks)" << std::endl
                << "Block size: " << Global::BLOCK_SIZE << std::endl;
}

Tile *Map::GetTile(int x, int y) const {
    if (x >= 0 && x < sizeX && y >= 0 && y < sizeY) 
        return tiles[y][x].get();
    return nullptr;
}

Block *Map::GetBlock(int x, int y) const {
    if (x >= 0 && x < numOfBlocksX && y >= 0 && y < numOfBlocksY)
        return blocks[y][x].get();
    return nullptr;
}

void World::Update(sf::Time timeElapsed) {
    time_since_testMob_update += timeElapsed;
    if (time_since_testMob_update >= sf::seconds(1)) {
        int x = testMob->GetTile()->X() + test_dx;
        int y = testMob->GetTile()->Y() + test_dy;
        if (test_dx == 1 && x == 51) test_dx = 0, test_dy = 1;
        if (test_dy == 1 && y == 51) test_dx = -1, test_dy = 0;
        if (test_dx == -1 && x == 49) test_dx = 0, test_dy = -1;
        if (test_dy == -1 && y == 49) test_dx = 1, test_dy = 0;
        //Server::log << x << y << endl;
        map->GetTile(x, y)->AddObject(testMob);
        time_since_testMob_update = sf::Time::Zero;
    }
}

void World::FillingWorld() {
    for (int i = 45; i <= 55; i++) {
        for (int j = 45; j <= 55; j++) {
            Tile *tile = map->GetTile(i, j);
            new Floor(tile);
            if ((i == 45 || i == 55) && (j == 45 || j == 55))
                new Wall(tile);
        }
    }
}

Mob *World::CreateNewPlayerMob() {
    Tile *startTile = map->GetTile(50, 50);
    return new Mob(startTile);
}