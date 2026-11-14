#ifndef LOST_KINGDOM_GAME_ENGINE_H
#define LOST_KINGDOM_GAME_ENGINE_H

#include "GameTypes.h"
#include "../World/WorldMap.h"
#include <string>

namespace lostkingdom {

class GameEngine {
public:
    void run();

private:
    Player player_;
    WorldMap world_;
    bool running_ = true;

    void showIntro();
    void showLocation() const;
    void showStatus() const;
    void showInventory() const;
    void showHelp() const;
    void showChoices() const;
    void handleCommand(const std::string& command);
    void travel();
    void explore();
    void rest();
    void createCharacter();
};

} // namespace lostkingdom

#endif
