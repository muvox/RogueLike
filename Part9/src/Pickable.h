//
// Created by Master on 17/06/2019.
//

#ifndef MOGUE_PICKABLE_H
#define MOGUE_PICKABLE_H


class Pickable {
public:
    bool pick(Actor *owner, Actor *wearer);
    void drop(Actor *owner, Actor *wearer);

    virtual bool use(Actor *owner, Actor *wearer);
};

class Healer : public Pickable {
public:
    float amount; //amount of hp it can heal

    Healer(float amount);

    bool use(Actor *owner, Actor *wearer);
};

class LightningBolt : public Pickable {
public:
    float range, damage;

    LightningBolt(float range, float damage);

    bool use(Actor *owner, Actor *wearer);
};

class Fireball : public LightningBolt {
public:
    Fireball(float range, float damage);

    bool use(Actor *owner, Actor *wearer);
};

class Confuser : public Pickable {
public:
    int nbTurns;
    float range;
    Confuser(int nbTurns, float range);
    bool use(Actor *owner, Actor *wearer);
};


#endif //MOGUE_PICKABLE_H
