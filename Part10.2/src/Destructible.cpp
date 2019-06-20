//
// Created by Master on 16/06/2019.
//

#include <stdio.h>
#include "main.h"

Destructible::Destructible(float maxHp, float defense, const char *corpseName) :
        maxHp(maxHp), hp(maxHp), defense(defense) {
    this->corpseName = _strdup(corpseName);
}

Destructible::~Destructible() {
    free((char *) corpseName);
}

float Destructible::takeDamage(Actor *owner, float damage) {
    damage -= defense;
    if (damage > 0) {
        hp -= damage;
        if (hp <= 0) {
            die(owner);
        }
    } else {
        damage = 0;
    }

    return damage;
}

void Destructible::load(TCODZip &zip) {
    maxHp = zip.getFloat();
    hp = zip.getFloat();
    defense = zip.getFloat();
    corpseName = _strdup(zip.getString());
}

void Destructible::save(TCODZip &zip) {
    zip.putFloat(maxHp);
    zip.putFloat(hp);
    zip.putFloat(defense);
    zip.putString(corpseName);
}

Destructible *Destructible::create(TCODZip &zip) {
    DestructibleType type=(DestructibleType)zip.getInt();
    Destructible *destructible=NULL;
    switch(type) {
        case MONSTER : destructible = new MonsterDestructible(0,0,NULL); break;
        case PLAYER : destructible = new PlayerDestructible(0,0,NULL); break;
    }

    destructible->load(zip);
    return destructible;
}

float Destructible::heal(float amount) {
    hp += amount;
    if (hp > maxHp) {
        amount -= hp - maxHp;
        hp = maxHp;
    }
    return amount;
}

void Destructible::die(Actor *owner) {
    //transform actor into a corpse!
    owner->ch = '%';
    owner->col = TCODColor::darkRed;
    owner->name = corpseName;
    owner->blocks = false;
    //make sure coprses are drawn before living actors
    engine.sendToBack(owner);
}

//Monster and Player actor's deaths are handled a little different so they get their own constructors and die methods.
MonsterDestructible::MonsterDestructible(float maxHp, float defense, const char *corpseName) :
        Destructible(maxHp, defense, corpseName) {}

void MonsterDestructible::save(TCODZip &zip) {
    zip.putInt(MONSTER);
    Destructible::save(zip);
}

void MonsterDestructible::die(Actor *owner) {
    //transform monster into a corpse, stop blocking and moving.
    engine.gui->message(TCODColor::lightGrey, "%s is dead", owner->name);
    Destructible::die(owner);
}

PlayerDestructible::PlayerDestructible(float maxHp, float defense, const char *corpseName) :
        Destructible(maxHp, defense, corpseName) {}

void PlayerDestructible::save(TCODZip &zip) {
    zip.putInt(PLAYER);
    Destructible::save(zip);
}

void PlayerDestructible::die(Actor *owner) {
    engine.gui->message(TCODColor::red, "You died!");
    Destructible::die(owner);
    engine.gameStatus = Engine::DEFEAT;
}
