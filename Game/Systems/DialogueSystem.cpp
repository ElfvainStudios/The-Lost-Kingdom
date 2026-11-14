#include "DialogueSystem.h"
#include <iostream>

namespace lostkingdom {

void DialogueSystem::talk(const std::string& locationId, const Player& player, QuestSystem& quests) {
    if (locationId == "village") {
        std::cout << "\nElder Rowan: 'Welcome to Willowmere, " << player.name
                  << ". The forest herbs that protect our people are running scarce.'\n";
        if (quests.completed()) {
            std::cout << "Elder Rowan: 'You have our gratitude. The village is safer because of you.'\n";
        } else if (quests.accepted()) {
            std::cout << "Elder Rowan: 'Please bring us three moon herbs from the Whispering Forest.'\n";
        } else {
            std::cout << "Elder Rowan: 'Would you gather three moon herbs for our healers? (yes/no): ";
            std::string answer;
            if (std::getline(std::cin, answer) &&
                (answer == "yes" || answer == "y" || answer == "YES" || answer == "Y")) {
                quests.accept();
                std::cout << "Quest accepted: A Village in Need.\n";
            } else {
                std::cout << "Elder Rowan: 'Perhaps another time, traveller.'\n";
            }
        }
    } else if (locationId == "forest") {
        std::cout << "\nA hooded wanderer watches the trees.\n"
                  << "Wanderer: 'The old stones to the north are older than the kingdom itself.'\n";
    } else if (locationId == "ruins") {
        std::cout << "\nA voice echoes between the pillars: 'Seek the crown beneath the moonwell.'\n";
    } else if (locationId == "lake") {
        std::cout << "\nA pale spirit rises above the water: 'The lake remembers every name it has heard.'\n";
    } else {
        std::cout << "\nNo one is here to speak with you.\n";
    }
}
}
