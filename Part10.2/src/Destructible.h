//
// Created by Master on 16/06/2019.
//

#ifndef MOGUE_DESTRUCTIBLE_H
#define MOGUE_DESTRUCTIBLE_H

class Destructible : public Persistent {
public:
    float maxHp;
    float hp;
    float defense;
    const char *corpseName;

public:
    Destructible(float maxHp, float defense, const char *corpseName);
    virtual ~Destructible();
    inline bool isDead() { return hp <= 0; };

    float takeDamage(Actor *owner, float damage);

    virtual void die(Actor *owner);

    float heal(float amount);
    void load(TCODZip &zip) override;
    void save(TCODZip &zip) override;

    static Destructible *create(TCODZip &zip);

protected:
    enum DestructibleType {
        MONSTER,PLAYER
    };

};

class MonsterDestructible : public Destructible {
public:
    MonsterDestructible(float maxHp, float defense, const char *corpseName);
    void die(Actor *owner);
    void save(TCODZip &zip);
};

class PlayerDestructible : public Destructible {
public:
    PlayerDestructible(float maxHp, float defense, const char *corpseName);
    void die(Actor *owner);
    void save(TCODZip &zip);
};


#endif //MOGUE_DESTRUCTIBLE_H
