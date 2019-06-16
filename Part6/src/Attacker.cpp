//
// Created by Master on 16/06/2019.
//
#include <stdio.h>
#include "main.h"

Attacker::Attacker(float power) : power(power) {}

void Attacker::attack(Actor *owner, Actor *target) {
    if (target->destructible && !target->destructible->isDead()) {
        if (power - target->destructible->defense > 0) {
            printf("%s attacks %s for %g hit points.\n", owner->name, target->name, power-target->destructible->defense);
        } else {
            printf("%s attacks %s but it has no effect!\n", owner->name, target->name);
        }
        target->destructible->takeDamage(target, power);
    } else {
        printf("%s attacks %s in vain.\n", owner->name, target->name);
    }
}