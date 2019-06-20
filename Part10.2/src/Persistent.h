//
// Created by Master on 20/06/2019.
//

#ifndef MOGUE_PERSISTENT_H
#define MOGUE_PERSISTENT_H


class Persistent {
public:
    virtual void load(TCODZip &zip) = 0;
    virtual void save(TCODZip &zip) = 0;
};


#endif //MOGUE_PERSISTENT_H
