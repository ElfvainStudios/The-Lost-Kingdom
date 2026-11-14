#include "QuestSystem.h"
#include <algorithm>
#include <iostream>

namespace lostkingdom {

bool QuestSystem::accepted() const { return accepted_; }
bool QuestSystem::completed() const { return completed_; }
int QuestSystem::herbProgress() const { return herbProgress_; }

void QuestSystem::accept() {
    if (!completed_) accepted_ = true;
}

void QuestSystem::gatherHerb(Player& player) {
    if (!accepted_ || completed_) return;
    herbProgress_ = std::min(herbGoal_, herbProgress_ + 1);
    std::cout << "Quest progress: " << herbProgress_ << "/" << herbGoal_
              << " moon herbs gathered.\n";
    if (herbProgress_ >= herbGoal_) {
        completed_ = true;
        accepted_ = false;
        player.experience += 40;
        player.gold += 25;
        std::cout << "Quest completed: A Village in Need! You receive 40 XP and 25 gold.\n";
        while (player.experience >= player.level * 50) {
            player.experience -= player.level * 50;
            ++player.level;
            player.maxHealth += 5;
            player.attack += 1;
            player.health = player.maxHealth;
            std::cout << "Level up! You are now level " << player.level
                      << ". Your health is fully restored.\n";
        }
    }
}

void QuestSystem::printJournal() const {
    std::cout << "\n========== QUEST JOURNAL ==========\n";
    if (completed_) {
        std::cout << "[COMPLETE] A Village in Need\n"
                  << "The village has received the healing herbs it needed.\n";
    } else if (accepted_) {
        std::cout << "[ACTIVE] A Village in Need\n"
                  << "Gather moon herbs in the Whispering Forest.\n"
                  << "Progress: " << herbProgress_ << "/" << herbGoal_ << "\n";
    } else {
        std::cout << "No active quests. Speak to the village elder in Willowmere.\n";
    }
}

void QuestSystem::restore(bool accepted, bool completed, int progress) {
    accepted_ = accepted;
    completed_ = completed;
    herbProgress_ = std::max(0, std::min(herbGoal_, progress));
    if (completed_) accepted_ = false;
}

} // namespace lostkingdom
