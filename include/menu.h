#ifndef DAP_MENU_H
#define DAP_MENU_H

#include <Arduino.h>

struct Menu_Option;
class Menu;
enum Option_Type {
    FUNCTIONAL,
    STATIC_NESTING,
    DYNAMIC_NESTING
};

typedef void (*Functional_Call)(Menu_Option*);
typedef Menu* (*Menu_Gen)(Menu_Option*, Menu*);

struct Menu_Option {
    String text;
    Option_Type type;
    Functional_Call func = NULL;
    Menu *child = NULL;
    Menu_Gen gen = NULL;
};

class Menu {
    private:
        std::vector<Menu_Option> options;

    public:
        int current_option;
        Menu *prev;

        Menu();

        Menu_Option* getOption(int index);
        void addOption(String text, Functional_Call func = NULL, Menu* child = NULL, Menu_Gen gen = NULL);
        void clearOptions();
        int getOptionsLength();

        bool moveSelectedUp();
        bool moveSelectedDown();
};

Menu* setupMainMenu();

extern Menu* root_menu;
extern Menu* current_menu;
extern String current_dir;

void moveIntoDirectory(String next);
String moveOutOfDirectory();

#endif