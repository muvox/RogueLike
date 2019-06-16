//
// Created by Master on 16/06/2019.
//

#include <stdio.h>
#include "main.h"

Destructible::Destructible(float maxHp, float defense, const char *corpseName) :
        maxHp(maxHp), hp(maxHp), defense(defense), corpseName(corpseName) {}

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

void Destructible::die(Actor *owner) {
    //transform actor into a corpse!
    owner->ch = '%';
    owner->col = TCODColor::darkRed;
    owner->name = corpseName;
    owner->blocks = false;
    //make sure coprses are drawn before living actors
    engine.sendtoBack(owner);
}

//Monster and Player actor's deaths are handled a little different so they get their own constructors and die methods.
MonsterDestructible::MonsterDestructible(float maxHp, float defense, const char *corpseName) :
        Destructible(maxHp, defense, corpseName) {}


PlayerDestructible::PlayerDestructible(float maxHp, float defense, const char *corpseName) :
        Destructible(maxHp, defense, corpseName) {}

void MonsterDestructible::die(Actor *owner){
    //transform monster into a corpse, stop blocking and moving.
    printf("%s is dead\n", owner-> name);
    Destructible::die(owner);
}

void PlayerDestructible::die(Actor *owner) {
    printf ("You died!\n");
    Destructible::die(owner);
    engine.gameStatus=Engine::DEFEAT;
}
