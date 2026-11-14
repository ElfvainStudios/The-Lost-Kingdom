#include "GameEngine.h"
#include "../Systems/CombatSystem.h"
#include "../Systems/DialogueSystem.h"
#include "../Systems/SaveSystem.h"
#include "../Components/Chapters/Prologue/prologue.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <limits>
#include <string>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

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

void enableTerminalFeatures() {
#ifdef _WIN32
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    if (output != INVALID_HANDLE_VALUE) {
        DWORD mode = 0;
        if (GetConsoleMode(output, &mode)) {
            SetConsoleMode(output, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
    }
#endif
}

void narrator(const std::string& message) {
    std::cout << "\n[THE KEEPER OF FORGOTTEN TALES]\n" << message << "\n";
}

bool contains(const std::vector<std::string>& values, const std::string& value) {
    return std::find(values.begin(), values.end(), value) != values.end();
}
}

void GameEngine::run() {
    enableTerminalFeatures();
    showIntro();

    std::cout << "\n  1. Begin a new adventure\n"
              << "  2. Continue from a save\n"
              << "  3. Leave the kingdom\n"
              << "\n> ";
    std::string menuChoice;
    if (!std::getline(std::cin, menuChoice)) return;

    if (menuChoice == "3" || trimLower(menuChoice) == "quit") return;
    if (menuChoice == "2") {
        if (!SaveSystem::load(player_, world_, quests_)) {
            std::cout << "Starting a new adventure instead.\n";
            createCharacter();
        }
    } else {
        std::cout << "\nChoose how your adventure begins:\n"
                  << "  1. Free Adventure - explore without starting the main story\n"
                  << "  2. Main Story     - hear the prologue and begin the central quest\n"
                  << "\nYou can begin the main story later with the 'story' command.\n> ";
        std::string mode;
        if (!std::getline(std::cin, mode)) return;
        createCharacter();
        if (mode == "2") startStory();
        else narrator("The roads are open, traveller. No prophecy binds your steps. "
                      "Explore, learn, trade, and make this world your own.");
    }

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

void GameEngine::showIntro() const {
    std::cout << "\033[2J\033[H";
    std::cout << R"(
 ______________________________________________________________
|                                                              |
|   THE LOST KINGDOM                                           |
|                                                              |
|          /\\                         .       *               |
|         /  \\       .        *               .                |
|        / /\\ \\             THE OLD LANDS                      |
|       / /  \\ \\       .                         *              |
|      /_/____\\_\\                    .                         |
|         ||||             .                                    |
|    _____||||_____        *          .                         |
|   /             \\                                             |
|  /  FORGOTTEN    \\        A CHOICE-DRIVEN FANTASY RPG         |
| /     REALMS      \\                                           |
|/__________________\\__________________________________________|
|                                                              |
|  The roads remember. The ruins are waiting. Your choices     |
|  will shape the tale - or you may write one of your own.     |
|______________________________________________________________|

)";
}

void GameEngine::createCharacter() {
    std::string name;
    do {
        std::cout << "What is your character's name? ";
        if (!std::getline(std::cin, name)) {
            running_ = false;
            return;
        }
        name = name.substr(0, 40);
        if (trimLower(name).empty()) std::cout << "Please enter a name.\n";
    } while (trimLower(name).empty());
    player_.name = name;

    std::cout << "\nChoose your path:\n"
              << "  1. Guardian  - sturdy, resilient and dependable\n"
              << "  2. Ranger    - balanced and skilled in the wild\n"
              << "  3. Arcanist  - powerful, with greater reserves of mana\n";
    std::string choice;
    do {
        std::cout << "> ";
        if (!std::getline(std::cin, choice)) {
            running_ = false;
            return;
        }
        if (choice != "1" && choice != "2" && choice != "3")
            std::cout << "Choose 1, 2 or 3.\n";
    } while (choice != "1" && choice != "2" && choice != "3");

    player_.knownSpells.clear();
    if (choice == "1") {
        player_.className = "Guardian";
        player_.maxHealth = player_.health = 40;
        player_.attack = 4;
        player_.defence = 4;
        player_.maxMana = player_.mana = 8;
    } else if (choice == "3") {
        player_.className = "Arcanist";
        player_.maxHealth = player_.health = 24;
        player_.attack = 8;
        player_.defence = 1;
        player_.maxMana = player_.mana = 24;
        player_.knownSpells = {"Arcane Spark", "Minor Mend"};
    } else {
        player_.className = "Ranger";
        player_.maxHealth = player_.health = 30;
        player_.attack = 6;
        player_.defence = 2;
        player_.maxMana = player_.mana = 12;
    }
    player_.storyStarted = false;
    player_.experience = 0;
    player_.level = 1;
    player_.gold = 10;
    player_.inventory = {"Old traveler's map", "Small healing potion"};
    world_.setCurrent("village");
    quests_.restore(false, false, 0);
    std::cout << "\nWelcome, " << player_.name << " the " << player_.className
              << ". Your adventure is yours to shape.\n";
}

void GameEngine::startStory() {
    if (player_.storyStarted) {
        narrator("The tale has already begun. Its next chapter awaits you in the world.");
        return;
    }
    prologue();
    player_.storyStarted = true;
    narrator("The prologue is told. The world remains open to you; follow the main tale "
             "when you are ready, and wander whenever you wish.");
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
    std::cout << "\nActions:\n"
              << "  travel    - travel to a connected location\n"
              << "  explore   - search your current surroundings\n"
              << "  talk      - speak with people or spirits nearby\n"
              << "  fight     - begin a turn-based encounter\n"
              << "  quests    - view your quest journal\n"
              << "  story     - begin the main story whenever you choose\n"
              << "  map       - view the 2D world map\n"
              << "  status    - view character stats\n"
              << "  inventory - view carried items\n"
              << "  read      - read a book in your inventory\n"
              << "  spells    - view learned magic\n"
              << "  cast      - cast a learned spell\n"
              << "  use       - use a carried consumable\n"
              << "  rest      - recover health and mana in the village\n"
              << "  save/load - save or restore your adventure\n"
              << "  help      - show commands\n"
              << "  quit      - end your session\n";
}

void GameEngine::handleCommand(const std::string& command) {
    if (command == "travel" || command == "move") travel();
    else if (command == "explore" || command == "look") explore();
    else if (command == "talk" || command == "speak") talk();
    else if (command == "fight" || command == "combat") fight();
    else if (command == "quests" || command == "quest" || command == "journal") quests_.printJournal();
    else if (command == "story") startStory();
    else if (command == "map") world_.printMap();
    else if (command == "status") showStatus();
    else if (command == "inventory" || command == "inv") showInventory();
    else if (command == "read" || command == "books") readBook();
    else if (command == "spells" || command == "magic") showSpells();
    else if (command == "cast") castSpell();
    else if (command == "use") useItem();
    else if (command == "rest") rest();
    else if (command == "save") SaveSystem::save(player_, world_, quests_);
    else if (command == "load") SaveSystem::load(player_, world_, quests_);
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
        std::size_t consumed = 0;
        const int index = std::stoi(choice, &consumed);
        if (consumed != choice.size() || index < 0 ||
            static_cast<std::size_t>(index) > exits.size()) {
            std::cout << "That destination is not available.\n";
            return;
        }
        if (index == 0) return;
        const std::string destination = exits[static_cast<std::size_t>(index - 1)]->id;
        if (!world_.move(destination)) std::cout << "You cannot travel there from here.\n";
        else narrator("You continue along the old roads. Each place holds stories of its own.");
    } catch (const std::exception&) {
        std::cout << "Please enter a destination number.\n";
    }
}

void GameEngine::explore() {
    const Location* location = world_.current();
    if (!location) return;
    std::cout << "\nYou take your time exploring " << location->name << "...\n";
    if (location->id == "village") {
        std::cout << "Villagers gather near the old well. Elder Rowan may have work for you.\n";
    } else if (location->id == "forest") {
        if (!contains(player_.inventory, "Moon herb")) {
            std::cout << "Beneath an ancient oak, you find a moon herb and carefully gather it.\n";
            player_.inventory.push_back("Moon herb");
            quests_.gatherHerb(player_);
        } else {
            std::cout << "You recognise the old gathering place. The herbs here need time to regrow.\n";
        }
        if (!contains(player_.inventory, "Field Guide to Moon Herbs")) {
            player_.inventory.push_back("Field Guide to Moon Herbs");
            std::cout << "You discover a weathered field guide tucked beneath a root.\n";
        }
    } else if (location->id == "ruins") {
        std::cout << "A rune glows faintly: 'The crown sleeps where moonlight touches stone.'\n";
        if (!contains(player_.inventory, "Codex of the First Seal")) {
            player_.inventory.push_back("Codex of the First Seal");
            std::cout << "Behind a loose stone, you find the Codex of the First Seal.\n";
        }
    } else if (location->id == "lake") {
        const int before = player_.health;
        player_.health = std::min(player_.maxHealth, player_.health + 5);
        player_.mana = std::min(player_.maxMana, player_.mana + 3);
        std::cout << "The water restores " << player_.health - before
                  << " health and a little mana.\n";
    } else {
        std::cout << "You discover old tracks and fragments of forgotten stories.\n";
    }
}

void GameEngine::talk() {
    DialogueSystem::talk(world_.currentId(), player_, quests_);
}

void GameEngine::fight() {
    CombatSystem::encounter(player_, world_.currentId());
    if (player_.health == 1) world_.setCurrent("village");
}

void GameEngine::rest() {
    if (world_.currentId() != "village") {
        std::cout << "It is not safe to rest here. Find shelter in Willowmere Village.\n";
        return;
    }
    player_.health = player_.maxHealth;
    player_.mana = player_.maxMana;
    std::cout << "You rest at the village inn and recover all your health and mana.\n";
}

void GameEngine::showStatus() const {
    std::cout << "\n" << player_.name << " the " << player_.className << "\n"
              << "Level: " << player_.level << "  XP: " << player_.experience << "\n"
              << "Health: " << player_.health << "/" << player_.maxHealth << "\n"
              << "Mana: " << player_.mana << "/" << player_.maxMana << "\n"
              << "Attack: " << player_.attack << "  Defence: " << player_.defence << "\n"
              << "Gold: " << player_.gold << "\n"
              << "Main story: " << (player_.storyStarted ? "started" : "not started") << "\n";
}

void GameEngine::showInventory() const {
    std::cout << "\nInventory (" << player_.inventory.size() << " items)\n";
    if (player_.inventory.empty()) std::cout << " - Your inventory is empty.\n";
    for (const auto& item : player_.inventory) std::cout << " - " << item << "\n";
}

void GameEngine::showSpells() const {
    std::cout << "\n========== SPELLBOOK ==========\n"
              << "Mana: " << player_.mana << "/" << player_.maxMana << "\n";
    if (player_.knownSpells.empty()) {
        std::cout << "You have not learned any spells. Search for old books and grimoires.\n";
        return;
    }
    for (const auto& spell : player_.knownSpells) std::cout << " - " << spell << "\n";
    std::cout << "Use 'cast mend' to restore health. Arcane Spark can be used in combat.\n";
}

void GameEngine::readBook() {
    std::vector<std::string> books;
    for (const auto& item : player_.inventory) {
        if (item == "Field Guide to Moon Herbs" || item == "Codex of the First Seal")
            books.push_back(item);
    }
    if (books.empty()) {
        std::cout << "You have no readable books. Search old ruins and hidden places.\n";
        return;
    }
    std::cout << "\nBooks in your possession:\n";
    for (std::size_t i = 0; i < books.size(); ++i)
        std::cout << "  " << (i + 1) << ". " << books[i] << "\n";
    std::cout << "Choose a book (0 to cancel): ";
    std::string input;
    std::getline(std::cin, input);
    try {
        std::size_t used = 0;
        int index = std::stoi(input, &used);
        if (used != input.size() || index < 0 || static_cast<std::size_t>(index) > books.size()) {
            std::cout << "Invalid book selection.\n";
            return;
        }
        if (index == 0) return;
        const std::string& book = books[static_cast<std::size_t>(index - 1)];
        if (book == "Field Guide to Moon Herbs") {
            narrator("The guide describes healing plants that thrive beneath moonlight. "
                     "Its final pages teach the gentle spell Minor Mend.");
            if (!contains(player_.knownSpells, "Minor Mend")) {
                player_.knownSpells.push_back("Minor Mend");
                std::cout << "You learned Minor Mend.\n";
            } else std::cout << "You already know the spell written here.\n";
        } else {
            narrator("The Codex tells of seven seals forged to imprison Malachar. "
                     "The first seal was bound to the old royal bloodline. "
                     "A margin note describes the spell Arcane Spark.");
            if (!contains(player_.knownSpells, "Arcane Spark")) {
                player_.knownSpells.push_back("Arcane Spark");
                std::cout << "You learned Arcane Spark.\n";
            } else std::cout << "You already know the spell written here.\n";
        }
    } catch (const std::exception&) {
        std::cout << "Please enter a book number.\n";
    }
}

void GameEngine::castSpell() {
    std::cout << "Cast which spell? (mend): ";
    std::string spell;
    if (!std::getline(std::cin, spell)) return;
    spell = trimLower(spell);
    if (spell == "mend" || spell == "minor mend") {
        if (!contains(player_.knownSpells, "Minor Mend")) {
            std::cout << "You have not learned Minor Mend. Find and read a spellbook.\n";
            return;
        }
        if (player_.mana < 4) {
            std::cout << "You do not have enough mana.\n";
            return;
        }
        if (player_.health == player_.maxHealth) {
            std::cout << "Your health is already full.\n";
            return;
        }
        const int before = player_.health;
        player_.mana -= 4;
        player_.health = std::min(player_.maxHealth, player_.health + 12);
        std::cout << "A soft light closes your wounds. You recover "
                  << player_.health - before << " health.\n";
    } else {
        std::cout << "That spell cannot be cast here. Arcane Spark is available during combat.\n";
    }
}

void GameEngine::useItem() {
    std::cout << "Use which item? (potion): ";
    std::string item;
    if (!std::getline(std::cin, item)) return;
    item = trimLower(item);
    if (item != "potion" && item != "small healing potion") {
        std::cout << "That item cannot be used with this command.\n";
        return;
    }
    auto potion = std::find(player_.inventory.begin(), player_.inventory.end(), "Small healing potion");
    if (potion == player_.inventory.end()) {
        std::cout << "You do not have a healing potion.\n";
        return;
    }
    if (player_.health == player_.maxHealth) {
        std::cout << "Your health is already full; you save the potion.\n";
        return;
    }
    const int before = player_.health;
    player_.inventory.erase(potion);
    player_.health = std::min(player_.maxHealth, player_.health + 15);
    std::cout << "You recover " << player_.health - before << " health.\n";
}

void GameEngine::showHelp() const {
    std::cout << "\nExplore at your own pace. The main storyline is optional and can be started with 'story'.\n"
              << "travel: choose a connected destination | explore: search the current area\n"
              << "talk: speak to local characters | fight: start a turn-based battle\n"
              << "quests: read objectives | story: begin the central narrative\n"
              << "inventory: view items | read: study books | spells/cast: manage and use magic\n"
              << "use: consume a potion | status: view attributes | map: view the 2D world\n"
              << "save/load: store or restore your adventure | rest: recover in Willowmere\n";
}

} // namespace lostkingdom
