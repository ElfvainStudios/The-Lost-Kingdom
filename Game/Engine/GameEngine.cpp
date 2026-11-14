#include "GameEngine.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <limits>

namespace lostkingdom {

namespace {
std::string trimLower(std::string value) {
    value.erase(value.begin(), std::find_if(value.begin(), value.end(),
        [](unsigned char c) { return !std::isspace(c); }));
    value.erase(std::find_if(value.rbegin(), value.rend(),
        [](unsigned char c) { return !std::isspace(c); }).base(), value.end());
    std::transform(value.begin(), value.end(), value.begin(),
        [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return value;
}
}

void GameEngine::run() {
    showIntro();
    createCharacter();
    while (running_) {
        showLocation();
        showChoices();
        std::cout << "\n> ";
        std::string command;
        if (!std::getline(std::cin, command)) break;
        handleCommand(trimLower(command));
    }
    std::cout << "\nYour journey pauses here. Until next time, traveller.\n";
}

void GameEngine::showIntro() {
    std::cout << R"(
============================================================
                     THE LOST KINGDOM
                A text-based fantasy RPG
============================================================

The old kingdom has vanished from every map.
Its roads remain, its ruins still whisper, and the forest
remembers a name that the people have long forgotten...

Your story begins in Willowmere Village.
)";
}

void GameEngine::createCharacter() {
    std::cout << "What is your character's name? ";
    std::string name;
    std::getline(std::cin, name);
    if (!name.empty()) player_.name = name;

    std::cout << "\nChoose your path:\n"
              << "  1. Guardian  - sturdy, resilient and dependable\n"
              << "  2. Ranger    - balanced and skilled in the wild\n"
              << "  3. Arcanist  - powerful, but less durable\n"
              << "> ";
    std::string choice;
    std::getline(std::cin, choice);
    if (choice == "1") {
        player_.className = "Guardian";
        player_.maxHealth = player_.health = 40;
        player_.attack = 4;
        player_.defence = 4;
    } else if (choice == "3") {
        player_.className = "Arcanist";
        player_.maxHealth = player_.health = 24;
        player_.attack = 8;
        player_.defence = 1;
    } else {
        player_.className = "Ranger";
        player_.maxHealth = player_.health = 30;
        player_.attack = 6;
        player_.defence = 2;
    }
    std::cout << "\nWelcome, " << player_.name << " the " << player_.className
              << ". Your adventure begins...\n";
}

void GameEngine::showLocation() const {
    const Location* location = world_.current();
    if (!location) return;
    std::cout << "\n------------------------------------------------------------\n"
              << location->name << "\n"
              << "------------------------------------------------------------\n"
              << location->description << "\n";
}

void GameEngine::showChoices() const {
    std::cout << "\nWhat would you like to do?\n"
              << "  travel    - travel to a connected location\n"
              << "  explore   - search your current surroundings\n"
              << "  map       - view the 2D world map\n"
              << "  status    - view character stats\n"
              << "  inventory - view carried items\n"
              << "  rest      - recover health (safe places only)\n"
              << "  help      - show commands\n"
              << "  quit      - end your session\n";
}

void GameEngine::handleCommand(const std::string& command) {
    if (command == "travel" || command == "move") travel();
    else if (command == "explore" || command == "look") explore();
    else if (command == "map") world_.printMap();
    else if (command == "status") showStatus();
    else if (command == "inventory" || command == "inv") showInventory();
    else if (command == "rest") rest();
    else if (command == "help") showHelp();
    else if (command == "quit" || command == "exit") running_ = false;
    else if (!command.empty()) std::cout << "Unknown command. Type 'help' to see your options.\n";
}

void GameEngine::travel() {
    const auto exits = world_.availableExits();
    if (exits.empty()) {
        std::cout << "There are no paths leading onward.\n";
        return;
    }
    std::cout << "\nAvailable destinations:\n";
    for (std::size_t i = 0; i < exits.size(); ++i)
        std::cout << "  " << (i + 1) << ". " << exits[i]->name << "\n";
    std::cout << "Choose a destination number (or 0 to cancel): ";
    std::string choice;
    std::getline(std::cin, choice);
    try {
        const int index = std::stoi(choice);
        if (index == 0) return;
        if (index < 1 || static_cast<std::size_t>(index) > exits.size()) {
            std::cout << "That destination is not available.\n";
            return;
        }
        const std::string destination = exits[static_cast<std::size_t>(index - 1)]->id;
        world_.move(destination);
    } catch (...) {
        std::cout << "Please enter a destination number.\n";
    }
}

void GameEngine::explore() {
    const Location* location = world_.current();
    if (!location) return;
    std::cout << "\nYou take your time exploring " << location->name << "...\n";
    if (location->id == "village")
        std::cout << "A villager mentions strange lights beyond the Whispering Forest.\n";
    else if (location->id == "forest")
        std::cout << "You find a handful of herbs beneath an old oak. You add them to your inventory.\n";
    else if (location->id == "ruins")
        std::cout << "A rune glows faintly: 'The crown sleeps where moonlight touches stone.'\n";
    else if (location->id == "lake")
        std::cout << "The water restores your spirit. You recover 5 health.\n";
    else
        std::cout << "You discover old tracks and fragments of forgotten stories.\n";
    if (location->id == "forest") player_.inventory.push_back("Forest herbs");
    if (location->id == "lake") player_.health = std::min(player_.maxHealth, player_.health + 5);
}

void GameEngine::rest() {
    if (world_.currentId() != "village") {
        std::cout << "It is not safe to rest here. Find shelter in Willowmere Village.\n";
        return;
    }
    player_.health = player_.maxHealth;
    std::cout << "You rest at the village inn and recover all your health.\n";
}

void GameEngine::showStatus() const {
    std::cout << "\n" << player_.name << " the " << player_.className << "\n"
              << "Level: " << player_.level << "  XP: " << player_.experience << "\n"
              << "Health: " << player_.health << "/" << player_.maxHealth << "\n"
              << "Attack: " << player_.attack << "  Defence: " << player_.defence << "\n"
              << "Gold: " << player_.gold << "\n";
}

void GameEngine::showInventory() const {
    std::cout << "\nInventory (" << player_.inventory.size() << " items)\n";
    for (const auto& item : player_.inventory) std::cout << " - " << item << "\n";
}

void GameEngine::showHelp() const {
    std::cout << "\nCommands are words, not real-time controls. Explore the world at your own pace.\n"
              << "Use 'travel' to select a connected location, 'explore' to inspect it,\n"
              << "and 'map', 'status' or 'inventory' to review your progress.\n";
}

} // namespace lostkingdom
