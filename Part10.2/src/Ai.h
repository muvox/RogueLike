//
// Created by Master on 16/06/2019.
//

#ifndef MOGUE_AI_H
#define MOGUE_AI_H


class Ai : public Persistent {
public:
    virtual void update(Actor *owner)=0;
    static Ai *create(TCODZip &zip);
protected:
    enum AiType{
        MONSTER, CONFUSED_MONSTER, PLAYER
    };
};

class PlayerAi : public Ai {
public:
    void update(Actor *owner);
    static void itemPickup(Actor *owner);
    void load(TCODZip &zip);
    void save(TCODZip &zip);

protected:
    bool moveOrAttack(Actor *owner, int targetx, int targety);
    void handleActionKey(Actor *owner, int ascii); //!TODO: changed this from public to protected. In case of crash return it there!
    Actor *choseFromInventory(Actor *owner);
};

class MonsterAi : public Ai{
public:
    void update(Actor *owner);
    void load(TCODZip &zip);
    void save(TCODZip &zip);

protected:
    void moveOrAttack(Actor *owner, int targetx, int targety);
    int moveCount;

};

class ConfusedMonsterAi : public Ai{
public:
    ConfusedMonsterAi(int nbTurns, Ai *oldAi);
    void update(Actor *owner);
    void load(TCODZip &zip);
    void save(TCODZip &zip);
protected:
    int nbTurns;
    Ai *oldAi;
};

#endif //MOGUE_AI_H
