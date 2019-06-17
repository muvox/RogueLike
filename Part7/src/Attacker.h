//
// Created by Master on 16/06/2019.
//
class Attacker {
public:
    float power;

public:
    Attacker(float power);
    void attack(Actor *owner, Actor *target);
};


