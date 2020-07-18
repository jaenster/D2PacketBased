//
// Created by jaenster on 17/07/2020.
//

#include "../Hooks/Hook.h"

Models::Game::Game() {
    this->players = new UnitCollection<Game, Player>;
    this->monsters = new UnitCollection<Game, Monster>;
    this->objects = new UnitCollection<Game, Object>;
    this->missiles = new UnitCollection<Game, Missile>;
    this->items = new UnitCollection<Game, Item>;
    this->tiles = new UnitCollection<Game, Tile>;

    this->players->local = this;
    this->monsters->local = this;
    this->objects->local = this;
    this->missiles->local = this;
    this->items->local = this;
    this->tiles->local = this;

    this->ItemEventEmitter<Game>::base = this;
    this->PlayerEventEmitter<Game>::base = this;

    this->ego = new Ego(this);

    this->world = new World(this);
    this->shop = new Shop(this);

    Hooks::addGame(this);
}

Models::Game::~Game() {
    delete this->players;
    delete this->monsters;
    delete this->objects;
    delete this->missiles;
    delete this->items;
    delete this->tiles;

    delete this->ego;
}

