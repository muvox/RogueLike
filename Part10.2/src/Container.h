//
// Created by Master on 17/06/2019.
//

#ifndef MOGUE_CONTAINER_H
#define MOGUE_CONTAINER_H


class Container : public Persistent{
public:
    int size;   //max number of actors to be contained
    TCODList<Actor *> inventory;

    Container(int size);
    ~Container();
    bool add(Actor *actor);
    void remove(Actor *actor);
    void load(TCODZip &zip);
    void save(TCODZip &zip);

};


#endif //MOGUE_CONTAINER_H
