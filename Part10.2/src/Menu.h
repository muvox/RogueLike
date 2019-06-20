//
// Created by Master on 20/06/2019.
//

#ifndef MOGUE_MENU_H
#define MOGUE_MENU_H


class Menu {
public:
    enum MenuItemCode{
        NONE,
        NEW_GAME,
        CONTINUE,
        EXIT
    };
    ~Menu();
    void clear();
    void addItem(MenuItemCode code, const char *label);
    MenuItemCode pick();

protected:
    struct MenuItem{
        MenuItemCode code;
        const char *label;
    };
    TCODList<MenuItem *> items;

};


#endif //MOGUE_MENU_H
