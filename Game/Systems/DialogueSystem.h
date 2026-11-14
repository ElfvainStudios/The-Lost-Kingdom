#ifndef LOST_KINGDOM_DIALOGUE_SYSTEM_H
#define LOST_KINGDOM_DIALOGUE_SYSTEM_H

#include "../Engine/GameTypes.h"
#include "QuestSystem.h"
#include <string>

namespace lostkingdom {
class DialogueSystem {
public:
    static void talk(const std::string& locationId, const Player& player, QuestSystem& quests);
};
}
#endif
