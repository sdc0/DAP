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

bool Menu::moveSelectedUp() {
    if (current_option <= 0) return false;
    current_option--;
    return true;
}

bool Menu::moveSelectedDown() {
    if (current_option >= getOptionsLength() - 1) return false;
    current_option++;
    return true;
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


Menu* settingsMenu() {
    Menu* m = new Menu();

    for (int i = 0; i < 5; i++) {
        m->addOption("Functional " + i, [](Menu_Option* o) { Serial.println(o->text); }, NULL, NULL);
    }

    return m;
}

Menu* generator(Menu_Option* o, Menu* prev) {
    Menu *m = new Menu();

    for (int i = 0; i < prev->getOptionsLength(); i++) {
        m->addOption(o->text + " -> " + prev->getOption(i)->text, [](Menu_Option *o) { Serial.println(o->text); }, NULL, NULL);
    }

    return m;
}

Menu* setupMainMenu() {
    Menu *m = new Menu();

    m->addOption("Functional", [](Menu_Option* o) { Serial.println(o->text); }, NULL, NULL);
    m->addOption("Static Nesting", NULL, settingsMenu(), NULL);
    m->addOption("Dynamic Nesting", NULL, NULL, generator);

    return m;
}