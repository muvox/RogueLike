//
// Created by Master on 16/06/2019.
//

#include <math.h>
#include "main.h"

// how many turns the monster chases the player
// after losing his sight
static const int TRACKING_TURNS = 3;

void PlayerAi::update(Actor *owner) {
    if (owner->destructible && owner->destructible->isDead()) {
        return;
    }

    int dx = 0, dy = 0;
    switch (engine.lastKey.vk) {
        case TCODK_UP :
            dy = -1;
            break;
        case TCODK_DOWN :
            dy = 1;
            break;
        case TCODK_LEFT :
            dx = -1;
            break;
        case TCODK_RIGHT :
            dx = 1;
            break;
        case TCODK_CHAR :
            handleActionKey(owner, engine.lastKey.c);
            break;
        default:
            break;
    }
    if (dx != 0 || dy != 0) {
        engine.gameStatus = Engine::NEW_TURN;
        if (moveOrAttack(owner, owner->x + dx, owner->y + dy)) {
            engine.map->computeFov();
        }
    }
}

void PlayerAi::handleActionKey(Actor *owner, int ascii) {
    switch (ascii) {
        case 'g' : //pick up item
        {
            itemPickup(owner);
        }
            break;
        case 'i' : //open inventory
        {
            Actor *actor = choseFromInventory(owner);
            if (actor) {
                actor->pickable->use(actor, owner);
                engine.gameStatus = Engine::NEW_TURN;
            }

        }
            break;
        case 'd': {
            Actor *actor = choseFromInventory(owner);
            if (actor) {
                actor->pickable->drop(actor, owner);
                engine.gameStatus = Engine::NEW_TURN;
            }
        }
            break;
    }
}

void PlayerAi::itemPickup(Actor *owner) {
    bool found = false;
    for (auto *actor : engine.actors) {
        if (actor->pickable && actor->x == owner->x && actor->y == owner->y) {
            if (actor->pickable->pick(actor, owner)) {
                found = true;
                engine.gui->message(TCODColor::lightGrey, "You pick up the %s.", actor->name);
                break;
            } else if (!found) {
                found = true;
                engine.gui->message(TCODColor::red, "Your inventory is full.");
            }
        }
    }
    if (!found) {
        engine.gui->message(TCODColor::lightGrey, "There's nothing here to pick.");
    }
    engine.gameStatus = Engine::NEW_TURN;
}

Actor *PlayerAi::choseFromInventory(Actor *owner) {
    static const int INVENTORY_WIDTH = 50;
    static const int INVENTORY_HEIGHT = 28;
    static TCODConsole con(INVENTORY_WIDTH, INVENTORY_HEIGHT);

    // display the inventory frame
    con.setDefaultForeground(TCODColor(200, 180, 50));
    con.printFrame(0, 0, INVENTORY_HEIGHT, INVENTORY_HEIGHT, true, TCOD_BKGND_DEFAULT, "inventory");

    //display the items with their keyboard shortcut
    con.setDefaultForeground(TCODColor::white);
    int shortcut = 'a';
    int y = 1;
    for (auto *actor: owner->container->inventory) {
        con.printf(2, y, "(%c) %s", shortcut, actor->name);
        y++;
        shortcut++;
    }

    //blit the inventory console on to the root console
    TCODConsole::blit(&con, 0, 0, INVENTORY_WIDTH, INVENTORY_HEIGHT,
                      TCODConsole::root, engine.screenWidth / 2 - INVENTORY_WIDTH / 2,
                      engine.screenHeight / 2 - INVENTORY_HEIGHT / 2);
    TCODConsole::flush();

    //wait for a key press
    TCOD_key_t key;
    TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL, true);

    //check for a char a-z
    if (key.vk == TCODK_CHAR) {
        int actorIndex = key.c - 'a';


        if (actorIndex >= 0 && actorIndex < owner->container->inventory.size()) {
            return owner->container->inventory.get(actorIndex);
        }

    }
    return NULL;

}

bool PlayerAi::moveOrAttack(Actor *owner, int targetx, int targety) {


    if (engine.map->isWall(targetx, targety)) return false;
    //check for something to attack
    for (auto *actor : engine.actors) {
        if (actor->destructible && !actor->destructible->isDead()
            && actor->x == targetx && actor->y == targety) {
            owner->attacker->attack(owner, actor);
            return false;
        }
    }

    // check for corpses or items
    for (auto *actor : engine.actors) {
        bool corpseOrItem = (actor->destructible && actor->destructible->isDead()) || actor->pickable;

        if (corpseOrItem && actor->x == targetx && actor->y == targety) {
            engine.gui->message(TCODColor::lightGrey, "There's a %s here", actor->name);
        }
    }

    owner->x = targetx;
    owner->y = targety;
    return true;

}

void MonsterAi::update(Actor *owner) {
    if (owner->destructible && owner->destructible->isDead()) {
        return;
    }

    if (engine.map->isInFov(owner->x, owner->y)) {
        //we can see the player. Lets move towards him
        moveCount = TRACKING_TURNS;
    } else {
        moveCount--;
    }

    if (moveCount > 0) {
        moveOrAttack(owner, engine.player->x, engine.player->y);
    }
}

void MonsterAi::moveOrAttack(Actor *owner, int targetx, int targety) {

    int dx = targetx - owner->x;
    int dy = targety - owner->y;

    int stepdx = (dx > 0 ? 1 : -1);
    int stepdy = (dy > 0 ? 1 : -1);

    float distance = sqrtf(dx * dx + dy * dy);

    if (distance >= 2) {
        dx = (int) (round(dx / distance));
        dy = (int) (round(dy / distance));
        if (engine.map->canWalk(owner->x + dx, owner->y + dy)) {
            owner->x += dx;
            owner->y += dy;
        } else if (engine.map->canWalk(owner->x + stepdx, owner->y)) {
            owner->x += stepdx;
        } else if (engine.map->canWalk(owner->x, owner->y + stepdy)) {
            owner->y += stepdy;
        }
    } else if (owner->attacker) {
        owner->attacker->attack(owner, engine.player);
    }

}

ConfusedMonsterAi::ConfusedMonsterAi(int nbTurns, Ai *oldAi) : nbTurns(nbTurns), oldAi(oldAi) {}

void ConfusedMonsterAi::update(Actor *owner) {
    //random direction
    TCODRandom *rng = TCODRandom::getInstance();
    int dx = rng->getInt(-1, 1);
    int dy = rng->getInt(-1, 1);

    if (dx != 0 || dy != 0) {
        int destx = owner->x + dx;
        int desty = owner->y + dy;
        //check if destination is walkable
        if (engine.map->canWalk(destx, desty)) {
            owner->x = destx;
            owner->y = desty;
        } else { //if target destination is a actor, attack it
            Actor *actor = engine.getActor(destx, desty);
            if (actor) {
                owner->attacker->attack(owner, actor);
            }
        }
    }
    //decrease the confusion time
    nbTurns--;

    if (nbTurns == 0) {
        owner->ai = oldAi;
        delete this;
    }
}