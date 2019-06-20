//
// Created by Master on 17/06/2019.
//

#include "main.h"

bool Pickable::pick(Actor *owner, Actor *wearer) {
    if (wearer->container && wearer->container->add(owner)) {
        engine.actors.remove(owner);
        return true;
    }
    return false;

}

bool Pickable::use(Actor *owner, Actor *wearer) {
    if (wearer->container) {
        wearer->container->remove(owner);
        delete owner;
        return true;
    }
    return false;
}

void Pickable::drop(Actor *owner, Actor *wearer) {
    if (wearer->container) {
        wearer->container->remove(owner);
        engine.actors.push(owner);
        owner->x = wearer->x;
        owner->y = wearer->y;
        engine.gui->message(TCODColor::lightGrey, "%s drops a %s", wearer->name, owner->name);
    }
}

Pickable *Pickable::create(TCODZip &zip) {
    PickableType type=(PickableType)zip.getInt();
    Pickable *pickable=NULL;
    switch(type) {
        case HEALER : pickable = new Healer(0); break;
        case LIGHTNING_BOLT : pickable = new LightningBolt(0,0); break;
        case CONFUSER : pickable=new Confuser(0,0); break;
        case FIREBALL : pickable=new Fireball(0,0); break;
    }
    pickable->load(zip);
    return pickable;
}


Healer::Healer(float amount) : amount(amount) {}

bool Healer::use(Actor *owner, Actor *wearer) {
    if (wearer->destructible) {
        float amountHealed = wearer->destructible->heal(amount);
        if (amountHealed > 0) {
            return Pickable::use(owner, wearer);
        }
    }
    return false;
}

void Healer::load(TCODZip &zip) {
    amount = zip.getFloat();
}

void Healer::save(TCODZip &zip) {
    zip.putInt(HEALER);
    zip.putFloat(amount);
}

LightningBolt::LightningBolt(float range, float damage) : range(range), damage(damage) {}

bool LightningBolt::use(Actor *owner, Actor *wearer) {
    Actor *closestMonster = engine.getClosestMonster(wearer->x, wearer->y, range);
    if (!closestMonster) {
        engine.gui->message(TCODColor::lightGrey, "No enemy close enough to strike.");
        return false;
    }

    //hit closest monster for <damage> hit points
    engine.gui->message(TCODColor::lightBlue,
                        "A lightning bolt strikes the %s with a loud thunder!\n""The damage is %g hit poitns.",
                        closestMonster->name, damage);
    closestMonster->destructible->takeDamage(closestMonster, damage);

    return Pickable::use(owner, wearer);

}

void LightningBolt::load(TCODZip &zip) {
    range = zip.getFloat();
    damage = zip.getFloat();
}

void LightningBolt::save(TCODZip &zip) {
    zip.putInt(LIGHTNING_BOLT);
    zip.putFloat(range);
    zip.putFloat(damage);
}

Fireball::Fireball(float range, float damage) : LightningBolt(range, damage) {}

bool Fireball::use(Actor *owner, Actor *wearer) {
    engine.gui->message(TCODColor::cyan, "Left-click a target tile for the fireball,\nor right-click to cancel.");
    int x, y;
    if (!engine.pickATile(&x, &y)) {
        return false;
    }
    // burn everything in <range> (including player)
    engine.gui->message(TCODColor::orange, "The fireball explodes, burning everything within %g tiles!", range);
    for (auto *actor : engine.actors) {
        if (actor->destructible && !actor->destructible->isDead() && actor->getDistance(x, y) <= range) {
            engine.gui->message(TCODColor::orange, "The %s gets burned for %g hit points.", actor->name, damage);
            actor->destructible->takeDamage(actor, damage);
        }
    }
    return Pickable::use(owner, wearer);
}

void Fireball::save(TCODZip &zip) {
    zip.putInt(FIREBALL);
    zip.putFloat(range);
    zip.putFloat(damage);
}

Confuser::Confuser(int nbTurns, float range) : nbTurns(nbTurns), range(range) {}

bool Confuser::use(Actor *owner, Actor *wearer) {
    engine.gui->message(TCODColor::cyan, "Left click an enemy to confuse it,\nor right click to cancel.");
    int x, y;
    if (!engine.pickATile(&x, &y, range)) {
        return false;
    }

    Actor *actor = engine.getActor(x, y);
    if (!actor) {
        return false;
    }

    //apply confusion ai for <nbTurns>
    Ai *confusedAi = new ConfusedMonsterAi(nbTurns, actor->ai);
    actor->ai = confusedAi;

    engine.gui->message(TCODColor::lightGreen, "The eyes of the %s look vacant\nas he starts to stumble around!",
                        actor->name);
    return Pickable::use(owner, wearer);
}

void Confuser::load(TCODZip &zip) {
    nbTurns = zip.getInt();
    range = zip.getFloat();
}

void Confuser::save(TCODZip &zip) {
    zip.putInt(CONFUSER);
    zip.putInt(nbTurns);
    zip.putFloat(range);
}









