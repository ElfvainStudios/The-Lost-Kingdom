#include "CombatSystem.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <random>
#include <string>

namespace lostkingdom {

bool CombatSystem::encounter(Player& player, const std::string& locationId) {
    std::string enemyName = "Wildling";
    int enemyHealth = 18;
    int enemyAttack = 5;
    int rewardXp = 18;
    int rewardGold = 6;

    if (locationId == "forest") {
        enemyName = "Thorn Wolf";
        enemyHealth = 22;
        enemyAttack = 6;
        rewardXp = 25;
        rewardGold = 8;
    } else if (locationId == "ruins") {
        enemyName = "Restless Sentinel";
        enemyHealth = 34;
        enemyAttack = 8;
        rewardXp = 40;
        rewardGold = 14;
    } else if (locationId == "lake") {
        enemyName = "Mist Wisp";
        enemyHealth = 16;
        enemyAttack = 7;
        rewardXp = 22;
        rewardGold = 9;
    } else if (locationId == "crossroads") {
        enemyName = "Roadside Bandit";
        enemyHealth = 20;
        enemyAttack = 5;
        rewardXp = 20;
        rewardGold = 10;
    } else {
        std::cout << "There is nobody here looking for a fight.\n";
        return true;
    }

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> variation(-1, 2);
    bool defending = false;
    std::cout << "\nA " << enemyName << " blocks your path!\n";

    while (enemyHealth > 0 && player.health > 0) {
        std::cout << "\n" << enemyName << " HP: " << enemyHealth
                  << " | Your HP: " << player.health << "/" << player.maxHealth
                  << "\nChoose: [A]ttack, [D]efend, [P]otion, [R]un: ";
        std::string action;
        if (!std::getline(std::cin, action)) return false;
        if (action.empty()) continue;
        const char choice = static_cast<char>(std::tolower(static_cast<unsigned char>(action[0])));
        if (choice == 'r') {
            std::cout << "You retreat from the encounter.\n";
            return true;
        }
        if (choice == 'd') {
            defending = true;
            std::cout << "You brace for the next attack.\n";
        } else if (choice == 'p') {
            auto potion = std::find(player.inventory.begin(), player.inventory.end(), "Small healing potion");
            if (potion == player.inventory.end()) {
                std::cout << "You have no healing potions.\n";
                continue;
            }
            const int before = player.health;
            if (before == player.maxHealth) {
                std::cout << "Your health is already full; you save the potion.\n";
                continue;
            }
            player.inventory.erase(potion);
            player.health = std::min(player.maxHealth, player.health + 15);
            std::cout << "You recover " << player.health - before << " health.\n";
        } else if (choice == 'a') {
            const int damage = std::max(1, player.attack + variation(rng));
            enemyHealth = std::max(0, enemyHealth - damage);
            std::cout << "You strike the " << enemyName << " for " << damage << " damage.\n";
            if (enemyHealth == 0) break;
        } else {
            std::cout << "Choose attack, defend, potion or run.\n";
            continue;
        }

        int incoming = std::max(1, enemyAttack + variation(rng) - player.defence);
        if (defending) {
            incoming = std::max(0, incoming / 2);
            defending = false;
        }
        player.health = std::max(0, player.health - incoming);
        std::cout << enemyName << " hits you for " << incoming << " damage.\n";
    }

    if (player.health <= 0) {
        player.health = 1;
        player.gold = std::max(0, player.gold - 5);
        std::cout << "You fall unconscious and awaken in Willowmere Village. "
                  << "You lose 5 gold but survive.\n";
        return false;
    }

    player.experience += rewardXp;
    player.gold += rewardGold;
    std::cout << "Victory! You gain " << rewardXp << " XP and "
              << rewardGold << " gold.\n";
    while (player.experience >= player.level * 50) {
        player.experience -= player.level * 50;
        ++player.level;
        player.maxHealth += 5;
        player.attack += 1;
        player.health = player.maxHealth;
        std::cout << "Level up! You are now level " << player.level << ".\n";
    }
    return true;
}
}
