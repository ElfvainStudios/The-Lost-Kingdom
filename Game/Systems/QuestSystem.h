#ifndef LOST_KINGDOM_QUEST_SYSTEM_H
#define LOST_KINGDOM_QUEST_SYSTEM_H

#include "../Engine/GameTypes.h"
#include <iosfwd>
#include <string>

namespace lostkingdom {

class QuestSystem {
public:
    bool accepted() const;
    bool completed() const;
    int herbProgress() const;
    void accept();
    void gatherHerb(Player& player);
    void printJournal() const;
    void restore(bool accepted, bool completed, int progress);
private:
    bool accepted_ = false;
    bool completed_ = false;
    int herbProgress_ = 0;
    static constexpr int herbGoal_ = 3;
};

} // namespace lostkingdom
#endif
