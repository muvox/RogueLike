//
// Created by Master on 16/06/2019.
//

#ifndef MOGUE_AI_H
#define MOGUE_AI_H


class Ai {
public:
    virtual void update(Actor *owner)=0;
};

class PlayerAi : public Ai {
public:
    void update(Actor *owner);
    void handleActionKey(Actor *owner, int ascii);
    void itemPickup(Actor *owner);

protected:
    bool moveOrAttack(Actor *owner, int targetx, int targety);
    Actor *choseFromInventory(Actor *owner);
};

class MonsterAi : public Ai{
public:
    void update(Actor *owner);

protected:
    void moveOrAttack(Actor *owner, int targetx, int targety);
    int moveCount;
};

#endif //MOGUE_AI_H
