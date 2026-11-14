#ifndef LOST_KINGDOM_COMBAT_SYSTEM_H
#define LOST_KINGDOM_COMBAT_SYSTEM_H

#include "../Engine/GameTypes.h"
#include <string>

namespace lostkingdom {
class CombatSystem {
public:
    // Returns true if the player survives the encounter.
    static bool encounter(Player& player, const std::string& locationId);
};
}
#endif
