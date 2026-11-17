#include "menu.h"

#include "dap_config.h"

Menu* root_menu = NULL;
Menu* current_menu  = NULL;

String current_dir = "";

Menu::Menu() {
    current_option = 0;
    top_option = 0;
    prev = NULL;
    exit = NULL;
}

Menu::Menu(Exit_Call e) {
    current_option = 0;
    top_option = 0;
    prev = NULL;
    exit = e;
}

Menu_Option* Menu::getOption(int index) {
    return &options[index];
}

void Menu::addOption(String text, Functional_Call func, Menu* child, Menu_Gen gen) {
    Option_Type type;
    if (child != NULL && gen == NULL) {
        type = STATIC_NESTING;
    }else if (child == NULL && gen != NULL) {
        type = DYNAMIC_NESTING;
    }else if (func != NULL) {
        type = FUNCTIONAL;   
    }else {
        Serial.printf("Option '%s' was not applicable to an option type, not adding\n", text.c_str());
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
    Menu* m = new Menu();

    for (int i = 0; i < prev->getOptionsLength(); i++) {
        m->addOption(o->text + " -> " + prev->getOption(i)->text, [](Menu_Option *o) { Serial.println(o->text); }, NULL, NULL);
    }

    return m;
}

void enterDirHelper(Menu_Option* o) {
    moveIntoDirectory(o->text);
}

void exitDirHelper(Menu* m) {
    Serial.println("in exit func");
    moveOutOfDirectory();
}

void playAudioHelper(Menu_Option* o) {
    if (!audio->playFile(current_dir + "/" + o->text)) Serial.println("Failed to pplay audio file");
}

Menu* SD_Generator(Menu_Option *o, Menu* prev) {
    Menu* m = new Menu(&exitDirHelper);

    File root = storage->getFS()->open(current_dir);
    Serial.println(current_dir);
    while (true) {
        File entry = root.openNextFile();
        if (!entry) break;
        Serial.println(entry.name());

        if (entry.isDirectory()) {
            m->addOption(entry.name(), &enterDirHelper, NULL, &SD_Generator);
        }else {
            m->addOption(entry.name(), &playAudioHelper, NULL, NULL);
        }
    }

    return m;
}

Menu* setupMainMenu() {
    Menu* m = new Menu();

    m->addOption("Functional", [](Menu_Option* o) { Serial.println(o->text); }, NULL, NULL);
    m->addOption("Static Nesting", NULL, settingsMenu(), NULL);
    m->addOption("File Browse", &enterDirHelper, NULL, &SD_Generator);

    return m;
}