#ifndef LOST_KINGDOM_SAVE_SYSTEM_H
#define LOST_KINGDOM_SAVE_SYSTEM_H

#include "../Engine/GameTypes.h"
#include "../World/WorldMap.h"
#include "QuestSystem.h"
#include <string>

namespace lostkingdom {
class SaveSystem {
public:
    static bool save(const Player& player, const WorldMap& world,
                     const QuestSystem& quests, const std::string& path = "savegame.dat");
    static bool load(Player& player, WorldMap& world, QuestSystem& quests,
                     const std::string& path = "savegame.dat");
};
}
#endif
