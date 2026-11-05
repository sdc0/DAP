#include "menu.h"

Menu* root_menu = NULL;
Menu* current_menu  = NULL;

String current_dir = "";

Menu::Menu() {
    current_option = 0;
    prev = NULL;
}

Menu_Option* Menu::getOption(int index) {
    return &options[index];
}

void Menu::addOption(String text, Functional_Call func, Menu* child, Menu_Gen gen) {
    Option_Type type;
    if (func != NULL && child == NULL && gen == NULL) {
        type = FUNCTIONAL;
    }else if (func == NULL && child != NULL && gen == NULL) {
        type = STATIC_NESTING;
    }else if (func == NULL && child == NULL && gen != NULL) {
        type = DYNAMIC_NESTING;
    }else {
        Serial.printf("Option '%s' was not applicable to an option type, not adding\n", text);
        return;
    }

    options.push_back({text, type, func, child, gen});
}

void Menu::clearOptions() {
    options.clear();
    current_option = 0;
}

int Menu::getOptionsLength() {
    return options.size();
}

void moveIntoDirectory(String next) {
    if (current_dir == "") {
        current_dir = "/";
        return;
    }

    if (current_dir == "/") current_dir = "";
    current_dir += "/" + next;
}

String moveOutOfDirectory() {
    if (current_dir == "") return "";
    if (current_dir == "/") {
        current_dir = "";
        return "/";
    }

    int start = current_dir.lastIndexOf('/');

    if (start <= -1) return "";

    String toReturn = current_dir.substring(start + 1, current_dir.length());
    current_dir = current_dir.substring(0, start);

    return toReturn;
}

Menu* setupMainMenu() {
    Menu *m = new Menu();
}