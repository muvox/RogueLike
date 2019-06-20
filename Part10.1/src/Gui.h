//
// Created by Master on 16/06/2019.
//

#ifndef MOGUE_GUI_H
#define MOGUE_GUI_H


class Gui : public Persistent {
public:
    Gui();
    ~Gui();
    void render();
    void message(const TCODColor &col, const char *text,  ...);
    void load(TCODZip &zip);
    void save(TCODZip &zip);

protected:
    TCODConsole *con;
    void renderMouseLook();
    void renderBar(int x, int y, int width, const char *name, float value, float maxValue, const TCODColor &barColor, const TCODColor &backColor);

    struct Message{
        char *text;
        TCODColor col;
        Message(const char *text, const TCODColor &col);
        ~Message();
    };

    TCODList<Message *> log;
};


#endif //MOGUE_GUI_H
