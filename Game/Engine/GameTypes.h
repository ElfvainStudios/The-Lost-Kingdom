#ifndef LOST_KINGDOM_GAME_TYPES_H
#define LOST_KINGDOM_GAME_TYPES_H

#include <string>
#include <vector>

namespace lostkingdom {

struct Player {
    std::string name = "Wanderer";
    std::string className = "Wayfarer";
    int level = 1;
    int experience = 0;
    int health = 30;
    int maxHealth = 30;
    int attack = 5;
    int defence = 2;
    int gold = 10;
    std::vector<std::string> inventory{"Old traveler's map", "Small healing potion"};
};

struct Location {
    std::string id;
    std::string name;
    std::string description;
    int x = 0;
    int y = 0;
    std::vector<std::string> exits;
    bool discovered = false;
};

} // namespace lostkingdom

#endif
