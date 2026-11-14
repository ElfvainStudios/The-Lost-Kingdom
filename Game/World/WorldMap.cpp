#include "WorldMap.h"
#include <iostream>
#include <iomanip>

namespace lostkingdom {

WorldMap::WorldMap() {
    locations_.emplace("village", Location{
        "village", "Willowmere Village",
        "A quiet settlement nestled beneath ancient trees. Lanterns glow in cottage windows.",
        0, 0, {"forest", "crossroads"}, true
    });
    locations_.emplace("forest", Location{
        "forest", "Whispering Forest",
        "Silver leaves stir overhead. Somewhere in the undergrowth, something watches you.",
        0, 1, {"village", "ruins"}, false
    });
    locations_.emplace("crossroads", Location{
        "crossroads", "Old King's Crossroads",
        "Four moss-covered roads meet around a weathered stone marker.",
        1, 0, {"village", "lake"}, false
    });
    locations_.emplace("ruins", Location{
        "ruins", "Sunken Ruins",
        "Broken pillars rise from the roots. Faded runes hint at a forgotten kingdom.",
        0, 2, {"forest"}, false
    });
    locations_.emplace("lake", Location{
        "lake", "Moonwell Lake",
        "A still lake reflects the sky. Blue lights drift across its surface.",
        2, 0, {"crossroads"}, false
    });
    currentId_ = "village";
}

const Location* WorldMap::current() const {
    const auto it = locations_.find(currentId_);
    return it == locations_.end() ? nullptr : &it->second;
}

Location* WorldMap::current() {
    auto it = locations_.find(currentId_);
    return it == locations_.end() ? nullptr : &it->second;
}

bool WorldMap::move(const std::string& destinationId) {
    Location* here = current();
    if (!here) return false;
    for (const auto& exit : here->exits) {
        if (exit == destinationId && locations_.count(exit)) {
            currentId_ = exit;
            locations_[exit].discovered = true;
            return true;
        }
    }
    return false;
}

std::vector<const Location*> WorldMap::availableExits() const {
    std::vector<const Location*> result;
    const Location* here = current();
    if (!here) return result;
    for (const auto& id : here->exits) {
        const auto it = locations_.find(id);
        if (it != locations_.end()) result.push_back(&it->second);
    }
    return result;
}

void WorldMap::printMap() const {
    const auto marker = [this](const std::string& id) {
        const auto it = locations_.find(id);
        if (id == currentId_) return '@';
        return it != locations_.end() && it->second.discovered ? '*' : '?';
    };

    std::cout << "\n                 THE KNOWN LANDS\n";
    std::cout << "                  [" << marker("ruins") << "] Sunken Ruins\n";
    std::cout << "                       |\n";
    std::cout << "              [" << marker("forest") << "] Whispering Forest\n";
    std::cout << "                       |\n";
    std::cout << "[" << marker("village") << "] Willowmere Village -- ["
              << marker("crossroads") << "] Old King's Crossroads -- ["
              << marker("lake") << "] Moonwell Lake\n";
    std::cout << "\n@ = your location   * = discovered   ? = unexplored\n";
}

const std::string& WorldMap::currentId() const { return currentId_; }

bool WorldMap::setCurrent(const std::string& locationId) {
    auto it = locations_.find(locationId);
    if (it == locations_.end()) return false;
    currentId_ = locationId;
    it->second.discovered = true;
    return true;
}

std::vector<std::string> WorldMap::discoveredIds() const {
    std::vector<std::string> ids;
    for (const auto& entry : locations_) {
        if (entry.second.discovered) ids.push_back(entry.first);
    }
    return ids;
}

void WorldMap::restoreDiscovered(const std::vector<std::string>& ids) {
    for (auto& entry : locations_) entry.second.discovered = false;
    for (const auto& id : ids) {
        auto it = locations_.find(id);
        if (it != locations_.end()) it->second.discovered = true;
    }
    if (locations_.count(currentId_)) locations_[currentId_].discovered = true;
}

} // namespace lostkingdom
