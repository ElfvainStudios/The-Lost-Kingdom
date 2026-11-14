#include "SaveSystem.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

namespace lostkingdom {

bool SaveSystem::save(const Player& player, const WorldMap& world,
                      const QuestSystem& quests, const std::string& path) {
    std::ofstream out(path);
    if (!out) {
        std::cout << "Could not open the save file for writing.\n";
        return false;
    }
    out << "LOST_KINGDOM_SAVE 1\n"
        << std::quoted(player.name) << ' ' << std::quoted(player.className) << '\n'
        << player.level << ' ' << player.experience << ' ' << player.health << ' '
        << player.maxHealth << ' ' << player.attack << ' ' << player.defence << ' '
        << player.gold << '\n'
        << std::quoted(world.currentId()) << '\n'
        << player.inventory.size() << '\n';
    for (const auto& item : player.inventory) out << std::quoted(item) << '\n';
    out << quests.accepted() << ' ' << quests.completed() << ' ' << quests.herbProgress() << '\n';
    const auto discovered = world.discoveredIds();
    out << discovered.size() << '\n';
    for (const auto& id : discovered) out << std::quoted(id) << '\n';
    if (!out) {
        std::cout << "A problem occurred while writing the save file.\n";
        return false;
    }
    std::cout << "Your adventure has been saved to " << path << ".\n";
    return true;
}

bool SaveSystem::load(Player& player, WorldMap& world, QuestSystem& quests,
                      const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        std::cout << "No save file was found at " << path << ".\n";
        return false;
    }

    std::string magic;
    int version = 0;
    Player loaded;
    std::string locationId;
    if (!(in >> magic >> version) || magic != "LOST_KINGDOM_SAVE" || version != 1 ||
        !(in >> std::quoted(loaded.name) >> std::quoted(loaded.className)
             >> loaded.level >> loaded.experience >> loaded.health >> loaded.maxHealth
             >> loaded.attack >> loaded.defence >> loaded.gold
             >> std::quoted(locationId))) {
        std::cout << "The save file is invalid or uses an unsupported version.\n";
        return false;
    }

    std::size_t count = 0;
    if (!(in >> count) || count > 1000) return false;
    loaded.inventory.clear();
    for (std::size_t i = 0; i < count; ++i) {
        std::string item;
        if (!(in >> std::quoted(item))) return false;
        loaded.inventory.push_back(item);
    }

    bool accepted = false, completed = false;
    int progress = 0;
    if (!(in >> accepted >> completed >> progress)) return false;
    std::size_t discoveredCount = 0;
    if (!(in >> discoveredCount) || discoveredCount > 1000) return false;
    std::vector<std::string> discovered;
    for (std::size_t i = 0; i < discoveredCount; ++i) {
        std::string id;
        if (!(in >> std::quoted(id))) return false;
        discovered.push_back(id);
    }
    if (loaded.level < 1 || loaded.experience < 0 || loaded.maxHealth < 1 ||
        loaded.health < 0 || loaded.health > loaded.maxHealth ||
        loaded.attack < 0 || loaded.defence < 0 || loaded.gold < 0 ||
        progress < 0 || progress > 3 || (accepted && completed) ||
        !world.setCurrent(locationId)) {
        std::cout << "The save data contains invalid character or location values.\n";
        return false;
    }

    player = loaded;
    world.restoreDiscovered(discovered);
    quests.restore(accepted, completed, progress);
    std::cout << "Welcome back, " << player.name << ". Your adventure has been restored.\n";
    return true;
}
}
