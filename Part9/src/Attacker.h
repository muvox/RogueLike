//
// Created by Master on 16/06/2019.
//
#ifndef MOGUE_ATTACKER_H
#define MOGUE_ATTACKER_H

class Attacker {
public:
    float power;

public:
    Attacker(float power);
    void attack(Actor *owner, Actor *target);
};


#endif //MOGUE_ATTACKER_H