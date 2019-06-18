//
// Created by Master on 16/06/2019.
//

#ifndef MOGUE_DESTRUCTIBLE_H
#define MOGUE_DESTRUCTIBLE_H

class Destructible {
public:
    float maxHp;
    float hp;
    float defense;
    const char *corpseName;

public:
    Destructible(float maxHp, float defense, const char *corpseName);

    inline bool isDead() { return hp <= 0; };

    float takeDamage(Actor *owner, float damage);

    virtual void die(Actor *owner);

    float heal(float amount);


};

class MonsterDestructible : public Destructible {
public:
    MonsterDestructible(float maxHp, float defense, const char *corpseName);
    void die(Actor *owner);
};

class PlayerDestructible : public Destructible {
public:
    PlayerDestructible(float maxHp, float defense, const char *corpseName);
    void die(Actor *owner);
};


#endif //MOGUE_DESTRUCTIBLE_H
