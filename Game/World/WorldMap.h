#ifndef LOST_KINGDOM_WORLD_MAP_H
#define LOST_KINGDOM_WORLD_MAP_H

#include "../Engine/GameTypes.h"
#include <map>
#include <string>
#include <vector>

namespace lostkingdom {

class WorldMap {
public:
    WorldMap();
    const Location* current() const;
    Location* current();
    bool move(const std::string& destinationId);
    std::vector<const Location*> availableExits() const;
    void printMap() const;
    const std::string& currentId() const;
    bool setCurrent(const std::string& locationId);
    std::vector<std::string> discoveredIds() const;
    void restoreDiscovered(const std::vector<std::string>& ids);

private:
    std::map<std::string, Location> locations_;
    std::string currentId_;
};

} // namespace lostkingdom

#endif
