/**
 * ================================================
 * THE LOST KINGDOM
 * ================================================
 * Developer: Darren Van Der Velde
 * Language:  C++
 * Platform:  Windows (Microsoft SAPI narration)
 *
 * Complete playable Prologue + Chapter One
 * Now featuring the Demon Lord as the main threat.
 * ================================================
 */

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <sapi.h>

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <algorithm>

#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "sapi.lib")

using namespace std;

/**
 * Narrator System
 */
class Narrator
{
private:
    ISpVoice* voice = nullptr;
    bool comInitialized = false;
    bool enabled = true;

public:
    bool initialize()
    {
        HRESULT result = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

        if (FAILED(result) && result != RPC_E_CHANGED_MODE)
            return false;

        if (result != RPC_E_CHANGED_MODE)
            comInitialized = true;

        HRESULT hr = CoCreateInstance(
            CLSID_SpVoice,
            nullptr,
            CLSCTX_ALL,
            IID_ISpVoice,
            reinterpret_cast<void**>(&voice)
        );

        if (FAILED(hr) || voice == nullptr)
            return false;

        voice->SetRate(-3);
        voice->SetVolume(90);
        return true;
    }

    void setEnabled(bool on) { enabled = on; }

    void speak(const wstring& dialogue)
    {
        wcout << L"\n[NARRATOR] " << dialogue << L"\n";
        wcout.flush();

        if (!enabled || voice == nullptr)
            return;

        voice->Speak(dialogue.c_str(), SPF_DEFAULT, nullptr);
        Sleep(280);
    }

    void speakLines(const vector<wstring>& lines)
    {
        for (const auto& line : lines)
            speak(line);
    }

    ~Narrator()
    {
        if (voice != nullptr)
        {
            voice->Release();
            voice = nullptr;
        }
        if (comInitialized)
            CoUninitialize();
    }
};

wstring toWide(const string& text)
{
    return wstring(text.begin(), text.end());
}

/**
 * Inventory
 */
struct Item
{
    string name;
    string description;
};

class Inventory
{
private:
    vector<Item> items;

public:
    void add(const string& name, const string& description)
    {
        items.push_back({ name, description });
        cout << "\n  [+] Obtained: " << name << "\n";
    }

    bool has(const string& name) const
    {
        for (const auto& i : items)
            if (i.name == name) return true;
        return false;
    }

    void show() const
    {
        cout << "\n---------- INVENTORY ----------\n";
        if (items.empty())
        {
            cout << "  (empty)\n";
        }
        else
        {
            for (size_t i = 0; i < items.size(); ++i)
            {
                cout << "  " << (i + 1) << ". " << items[i].name
                     << " — " << items[i].description << "\n";
            }
        }
        cout << "-------------------------------\n";
    }

    int count() const { return static_cast<int>(items.size()); }
};

/**
 * Input Helper
 */
int readChoice(int minOpt, int maxOpt)
{
    int choice = 0;
    while (true)
    {
        cout << "Choice: ";
        if (cin >> choice)
        {
            if (choice >= minOpt && choice <= maxOpt)
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return choice;
            }
            cout << "Please enter a number between "
                 << minOpt << " and " << maxOpt << ".\n";
        }
        else
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Enter a number.\n";
        }
    }
}

void pressEnter(const string& prompt = "Press ENTER to continue...")
{
    cout << "\n" << prompt;
    cin.get();
}

/**
 * Race System
 */
string getRaceName(int raceId, int subRaceId)
{
    switch (raceId)
    {
    case 1: return "Human";
    case 2: return "Elf";
    case 3: return "Dwarf";
    case 4: return "Fairy";
    case 5: return "Dark Elf";
    case 6:
        switch (subRaceId)
        {
        case 1: return "Demi-Human (Fox)";
        case 2: return "Demi-Human (Cat)";
        case 3: return "Demi-Human (Bull)";
        case 4: return "Demi-Human (Tiger)";
        default: return "Demi-Human";
        }
    case 7:
        switch (subRaceId)
        {
        case 1: return "Fire Elemental";
        case 2: return "Water Elemental";
        case 3: return "Earth Elemental";
        case 4: return "Wind Elemental";
        default: return "Elemental";
        }
    default: return "Unknown Race";
    }
}

void applyRaceBonuses(int raceId, int subRaceId, int& health, int& attack, int& defence, int& luck)
{
    switch (raceId)
    {
    case 1: // Human
        health += 10;
        luck += 5;
        break;
    case 2: // Elf
        attack += 5;
        luck += 8;
        break;
    case 3: // Dwarf
        health += 20;
        defence += 10;
        break;
    case 4: // Fairy
        luck += 15;
        attack += 3;
        health -= 10;
        break;
    case 5: // Dark Elf
        attack += 10;
        luck += 5;
        break;
    case 6: // Demi-Human
        if (subRaceId == 3) { health += 25; defence += 5; }      // Bull
        else if (subRaceId == 4) { attack += 12; }               // Tiger
        else if (subRaceId == 1) { luck += 12; }                 // Fox
        else { luck += 8; attack += 5; }                         // Cat
        break;
    case 7: // Elemental
        if (subRaceId == 1) attack += 15;                        // Fire
        else if (subRaceId == 2) { health += 15; defence += 5; } // Water
        else if (subRaceId == 3) defence += 15;                  // Earth
        else { luck += 10; attack += 5; }                        // Wind
        break;
    }
}

/**
 * Player
 */
struct Player
{
    string name;
    string raceName;
    int health = 100;
    int maxHealth = 100;
    int gold = 0;
    int attack = 15;
    int defence = 10;
    int luck = 10;
    Inventory inventory;

    void showStats() const
    {
        cout << "\n=================================\n";
        cout << "         CHARACTER STATS\n";
        cout << "=================================\n";
        cout << "Adventurer: " << name << "\n";
        cout << "Race:       " << raceName << "\n";
        cout << "Health:     " << health << " / " << maxHealth << " HP\n";
        cout << "Gold:       " << gold << "g\n";
        cout << "Attack:     " << attack << "\n";
        cout << "Defence:    " << defence << "\n";
        cout << "Luck:       " << luck << "\n";
        cout << "=================================\n";
    }

    bool isAlive() const { return health > 0; }

    void damage(int amount)
    {
        int taken = max(1, amount - defence / 3);
        health = max(0, health - taken);
        cout << "\n  [-] You take " << taken << " damage. (HP: " << health << ")\n";
    }

    void heal(int amount)
    {
        health = min(maxHealth, health + amount);
        cout << "\n  [+] Restored " << amount << " HP. (HP: " << health << ")\n";
    }
};

/**
 * Demon Lord
 */
struct DemonLord
{
    string name = "Malakar, the Demon Lord";
    int health = 180;
    int maxHealth = 180;
    int attack = 28;
    int defence = 12;

    bool isAlive() const { return health > 0; }

    void showStatus() const
    {
        cout << "\n---------- DEMON LORD ----------\n";
        cout << "  " << name << "\n";
        cout << "  Health: " << health << " / " << maxHealth << " HP\n";
        cout << "--------------------------------\n";
    }

    void takeDamage(int amount)
    {
        int taken = max(1, amount - defence / 4);
        health = max(0, health - taken);
        cout << "\n  [!] " << name << " takes " << taken << " damage! (HP: " << health << ")\n";
    }

    // Returns damage dealt to the player
    int attackPlayer(Player& player, Narrator& narrator)
    {
        int roll = rand() % 100;

        if (roll < 20)
        {
            // Heavy attack
            narrator.speak(L"Malakar raises a claw of black fire and brings it down with crushing force!");
            int dmg = attack + 8 + (rand() % 6);
            player.damage(dmg);
            return dmg;
        }
        else if (roll < 45)
        {
            // Dark bolt
            narrator.speak(L"Shadows gather around Malakar's hand. A bolt of pure darkness streaks toward you!");
            int dmg = attack + (rand() % 8);
            player.damage(dmg);
            return dmg;
        }
        else
        {
            // Normal swipe
            narrator.speak(L"The Demon Lord lashes out with a clawed hand!");
            int dmg = attack + (rand() % 5);
            player.damage(dmg);
            return dmg;
        }
    }
};

/**
 * Simple turn-based battle against the Demon Lord
 */
bool fightDemonLord(Narrator& narrator, Player& player, DemonLord& lord)
{
    system("cls");
    cout << "\n=================================\n";
    cout << "      BATTLE: THE DEMON LORD\n";
    cout << "=================================\n";

    narrator.speakLines({
        L"The air turns cold. The shadows themselves seem to bow.",
        L"From the deepest corner of the library rises a figure of black iron and crimson flame.",
        L"Malakar, the Demon Lord, stands before you — the same darkness that ended the Lost Kingdom a thousand years ago."
    });

    lord.showStatus();
    player.showStats();

    bool fled = false;

    while (player.isAlive() && lord.isAlive() && !fled)
    {
        cout << "\nWhat will you do?\n\n";
        cout << "1. Attack\n";
        cout << "2. Defend (reduce next damage)\n";
        cout << "3. Use item\n";
        cout << "4. Try to flee\n";

        int action = readChoice(1, 4);

        if (action == 1)
        {
            int dmg = player.attack + (rand() % 8) - lord.defence / 5;
            dmg = max(3, dmg);

            // Bonus damage if player has the map or key (story power)
            if (player.inventory.has("Complete Ancient Map") || player.inventory.has("Silver Rune Key"))
            {
                dmg += 6;
                cout << "\n  The ancient power reacts to your presence! Extra damage!\n";
            }

            lord.takeDamage(dmg);
            narrator.speak(L"Your strike lands true against the Demon Lord's armour.");
        }
        else if (action == 2)
        {
            cout << "\n  You raise your guard, bracing for the next blow.\n";
            // Temporary defence boost handled by reducing next damage
            int originalDef = player.defence;
            player.defence += 8;

            if (lord.isAlive())
            {
                lord.attackPlayer(player, narrator);
            }

            player.defence = originalDef;
            continue; // skip normal enemy turn because we already resolved it
        }
        else if (action == 3)
        {
            if (player.inventory.has("Blue Healing Vial"))
            {
                player.heal(40);
                cout << "\n  You drink the Blue Healing Vial. It is consumed.\n";
                // Note: in a full game you would remove the item. For simplicity we just use it once.
            }
            else
            {
                cout << "\n  You have no usable combat items right now.\n";
            }
        }
        else if (action == 4)
        {
            if (rand() % 100 < player.luck + 15)
            {
                narrator.speak(L"You break away from the Demon Lord's gaze and race for the door!");
                fled = true;
            }
            else
            {
                narrator.speak(L"Malakar's presence freezes your legs. There is no escape this time.");
            }
        }

        // Demon Lord's turn
        if (lord.isAlive() && player.isAlive() && !fled)
        {
            lord.attackPlayer(player, narrator);
        }

        if (player.isAlive() && lord.isAlive())
        {
            lord.showStatus();
            cout << "  Your HP: " << player.health << " / " << player.maxHealth << "\n";
        }
    }

    if (!player.isAlive())
    {
        narrator.speak(L"Your vision darkens. The Demon Lord's laughter is the last thing you hear...");
        cout << "\n*** You have been defeated by Malakar, the Demon Lord. ***\n";
        return false;
    }

    if (fled)
    {
        narrator.speak(L"You escape into the rain, heart pounding. The Demon Lord remains... for now.");
        cout << "\n*** You fled the battle. The Demon Lord still lives. ***\n";
        return true;
    }

    // Victory
    narrator.speakLines({
        L"With a final cry, Malakar staggers. Black fire flickers and dies.",
        L"The Demon Lord falls to one knee, then collapses into shadow and ash.",
        L"Yet even as the form dissolves, a cold voice echoes in your mind...",
        L"This... is only the beginning, little seeker. The seal is broken. I will return."
    });

    player.gold += 50;
    player.inventory.add("Demon Lord's Shard", "A fragment of pure darkness. It still pulses with power.");
    cout << "\n  [+] Gained 50 gold and a Demon Lord's Shard!\n";
    cout << "\n*** Victory! You have defeated the Demon Lord's manifestation. ***\n";
    return true;
}

/**
 * Prologue
 */
bool playPrologue(Narrator& narrator, Player& player)
{
    system("cls");

    cout << "\n=================================\n";
    cout << "           PROLOGUE\n";
    cout << "      The Forgotten Age\n";
    cout << "=================================\n";

    narrator.speakLines({
        L"Chapter Zero. The Forgotten Age.",
        L"Long ago, the Lost Kingdom stood as the greatest empire in the world.",
        L"Its kings wielded a sword said to possess the power of the gods.",
        L"But darkness came from beyond the mountains — the Demon Lord Malakar.",
        L"The kingdom fell. Its people vanished. And the legendary sword was lost.",
        L"Now, after a thousand years, the ancient seal has begun to break.",
        L"And you, " + toWide(player.name) + L", may be the only one who can stop the Demon Lord's return."
    });

    pressEnter();

    system("cls");
    cout << "\n=================================\n";
    cout << "        A VISION IN THE DARK\n";
    cout << "=================================\n";

    narrator.speakLines({
        L"In the dead of night, a vision finds you.",
        L"You stand before a crumbling throne of stone and gold.",
        L"A voice, neither male nor female, whispers from the dust.",
        L"Seeker... the map has waited for one who would listen.",
        L"Choose wisely. The path you take will shape what remains of this land."
    });

    cout << "\nThe voice offers you a gift. What do you reach for?\n\n";
    cout << "1. A silver key etched with runes\n";
    cout << "2. A small vial of glowing blue liquid\n";
    cout << "3. A folded scrap of parchment\n";
    cout << "4. Nothing — you refuse the vision\n";

    int gift = readChoice(1, 4);

    switch (gift)
    {
    case 1:
        player.inventory.add("Silver Rune Key", "Cold to the touch. Opens something old.");
        narrator.speak(L"The key settles into your palm as if it always belonged there.");
        break;
    case 2:
        player.inventory.add("Blue Healing Vial", "A single dose of restorative elixir.");
        player.heal(15);
        narrator.speak(L"Warmth spreads through your veins as the vision fades.");
        break;
    case 3:
        player.inventory.add("Scrap of Prophecy", "Words in an ancient tongue: 'Where the library sleeps, the map wakes.'");
        narrator.speak(L"The parchment burns the prophecy into your memory.");
        break;
    default:
        narrator.speak(L"You turn away. The vision dissolves... but something still watches.");
        player.luck = max(1, player.luck - 2);
        cout << "\n  Your luck feels slightly diminished.\n";
        break;
    }

    pressEnter();

    narrator.speakLines({
        L"Dawn breaks. The vision is gone — but the weight of it remains.",
        L"Your journey into the Lost Kingdom begins with a single step.",
        L"Seek the abandoned library. The map is said to rest there still.",
        L"But beware... the Demon Lord's shadow already stirs within its walls."
    });

    cout << "\n*** Prologue complete ***\n";
    pressEnter("Press ENTER to begin Chapter One...");
    return true;
}

// ================================================
// CHAPTER ONE — The Abandoned Library
// ================================================

bool playChapterOne(Narrator& narrator, Player& player)
{
    system("cls");

    cout << "\n=================================\n";
    cout << "         CHAPTER ONE\n";
    cout << "      The Abandoned Library\n";
    cout << "=================================\n";

    narrator.speakLines({
        L"Rain drums against the broken roof of an abandoned library on the edge of the wilds.",
        L"Ivy has claimed the stone steps. The great oak doors hang half-open, as if inviting you in.",
        L"Inside, dust motes float in thin shafts of grey light. Shelves lean like tired soldiers.",
        L"Somewhere deeper in the stacks, something shifts — paper, or a footstep...",
        L"Or the restless breath of something far older."
    });

    bool mapFound = false;
    bool exploredDesk = false;
    bool exploredStacks = false;
    bool metStranger = false;
    bool chapterComplete = false;
    bool demonLordAppeared = false;

    while (player.isAlive() && !chapterComplete)
    {
        player.showStats();

        cout << "\nYou stand in the main hall of the library. What do you do?\n\n";
        cout << "1. Search the librarian's desk\n";
        cout << "2. Explore the deeper book stacks\n";
        cout << "3. Investigate the noise in the shadows\n";
        cout << "4. Check your inventory\n";
        cout << "5. Leave the library (end Chapter One early)\n";

        int choice = readChoice(1, 5);

        if (choice == 1)
        {
            if (exploredDesk)
            {
                cout << "\nYou already searched the desk thoroughly. Nothing new remains.\n";
            }
            else
            {
                exploredDesk = true;
                narrator.speakLines({
                    L"You brush dust from a heavy oak desk.",
                    L"Drawers stick, then give. Inside: a half-melted candle, a rusted old pen, dried ink, and a brass compass."
                });
                player.inventory.add("Old Brass Compass", "Still points to true north — mostly.");
                player.inventory.add("Rusted Old Pen", "Does not work but has a very cool design.");
                player.gold += 6;
                cout << "\n  [+] Found 6 gold coins in a cracked drawer.\n";

                if (player.inventory.has("Scrap of Prophecy") || player.luck >= 15)
                {
                    narrator.speak(L"A false bottom in the drawer clicks open. Beneath it lies a sealed tube.");
                    cout << "\nInside the tube is a fragment of an aged map — not complete, but marked with a red X deep in the forest.\n";
                    player.inventory.add("Map Fragment", "Part of the ancient map of the Lost Kingdom.");
                    mapFound = true;
                }
                else
                {
                    narrator.speak(L"You sense something more is hidden here... but it will not yield to you yet.");
                }
            }
        }
        else if (choice == 2)
        {
            if (exploredStacks)
            {
                cout << "\nThe stacks are silent now. You have already walked these aisles.\n";
            }
            else
            {
                exploredStacks = true;
                narrator.speakLines({
                    L"You move between tall shelves. Titles in dead languages stare back at you.",
                    L"One book lies open on the floor. Its pages show a kingdom ringed by mountains — and a black mark over the capital.",
                    L"The mark is shaped like a clawed hand. The Demon Lord's seal."
                });

                cout << "\nDo you take the book?\n";
                cout << "1. Yes — take the chronicle\n";
                cout << "2. No — leave it\n";
                int take = readChoice(1, 2);
                if (take == 1)
                {
                    player.inventory.add("Kingdom Chronicle", "A partial history of the fall of the Lost Kingdom and the rise of Malakar.");
                    narrator.speak(L"Knowledge is a weapon. You close the book and slip it into your pack.");
                }
                else
                {
                    narrator.speak(L"You leave the past where it lies.");
                }

                if (rand() % 20 < player.luck)
                {
                    cout << "\nAs you turn, a loose stone under the shelf reveals a small pouch.\n";
                    player.gold += 12;
                    cout << "  [+] Found 12 gold!\n";
                }
            }
        }
        else if (choice == 3)
        {
            if (metStranger)
            {
                cout << "\nThe stranger is gone. Only dust settles in the aisle.\n";
            }
            else
            {
                metStranger = true;
                narrator.speakLines({
                    L"You follow the sound between the shelves.",
                    L"A hooded figure freezes, then slowly raises empty hands.",
                    L"I mean no harm, traveller. I came for the same thing you seek — the map.",
                    L"But something else is already here. The Demon Lord's presence... it is stronger than I feared."
                });

                cout << "\nHow do you respond?\n";
                cout << "1. Demand they leave\n";
                cout << "2. Offer to search together\n";
                cout << "3. Attack first — trust no one\n";
                cout << "4. Ask what they know about the Demon Lord\n";

                int talk = readChoice(1, 4);

                if (talk == 1)
                {
                    narrator.speak(L"The figure nods once and slips into the dark. You hear the outer door creak.");
                    cout << "\nYou are alone again. Safer, perhaps — but emptier of answers.\n";
                }
                else if (talk == 2)
                {
                    narrator.speakLines({
                        L"Together, then. The stranger helps you lift a fallen shelf.",
                        L"Behind it: a locked iron box."
                    });
                    if (player.inventory.has("Silver Rune Key"))
                    {
                        narrator.speak(L"Your silver key turns in the lock as if it were made for this moment.");
                        player.inventory.add("Complete Ancient Map", "The full map of the Lost Kingdom — rivers, ruins, and a marked temple.");
                        mapFound = true;
                        player.gold += 20;
                        cout << "\n  [+] The stranger leaves you a pouch of 20 gold for your trouble.\n";
                        narrator.speak(L"Our paths will cross again, " + toWide(player.name) + L". The temple waits in the east... and the Demon Lord knows it too.");
                    }
                    else
                    {
                        narrator.speak(L"The lock will not yield. You need a key marked with silver runes.");
                        cout << "\n(Hint: such a key might appear in a vision... or a future chapter.)\n";
                        player.inventory.add("Locked Iron Box", "Needs a silver rune key.");
                    }
                }
                else if (talk == 3)
                {
                    narrator.speak(L"Steel flashes. The stranger is quicker than they look.");
                    player.damage(18);
                    if (player.isAlive())
                    {
                        narrator.speak(L"They vanish into the stacks, wounded pride and all. You gain nothing but bruises.");
                    }
                }
                else
                {
                    narrator.speakLines({
                        L"The hooded one speaks of a temple beyond the Whispering Forest,",
                        L"of a sword sealed in stone, and of a darkness that still feeds on the kingdom's bones.",
                        L"Malakar was sealed, not destroyed. The library is one of the places the seal is weakest.",
                        L"Then they press a charcoal sketch into your hand — a path through the trees."
                    });
                    player.inventory.add("Forest Path Sketch", "A rough guide toward the Whispering Forest.");
                    if (!mapFound)
                    {
                        cout << "\nIt is not the full map, but it is a start.\n";
                    }
                }
            }
        }
        else if (choice == 4)
        {
            player.inventory.show();
            pressEnter();
        }
        else if (choice == 5)
        {
            if (!mapFound && !player.inventory.has("Forest Path Sketch") && !player.inventory.has("Map Fragment"))
            {
                cout << "\nLeave without a clue? The kingdom will not reveal itself easily.\n";
                cout << "1. Stay and search more\n";
                cout << "2. Leave anyway\n";
                if (readChoice(1, 2) == 1)
                    continue;
            }
            chapterComplete = true;
            narrator.speak(L"You step back into the rain. The library falls silent behind you.");
        }

        // Auto-complete if map is fully found → trigger Demon Lord confrontation
        if (mapFound && player.inventory.has("Complete Ancient Map") && !demonLordAppeared)
        {
            demonLordAppeared = true;
            cout << "\n*** You hold the complete map of the Lost Kingdom. ***\n";
            narrator.speak(L"The moment the map is complete, the air grows heavy. The shadows deepen.");
            narrator.speak(L"A cold voice fills the hall: \"So... the seeker has found the path.\"");

            DemonLord malakar;
            bool survived = fightDemonLord(narrator, player, malakar);

            if (!survived)
            {
                return false;
            }

            chapterComplete = true;
            narrator.speak(L"Chapter One draws to a close. The path to the Whispering Forest now lies open...");
            narrator.speak(L"But the true Demon Lord still waits beyond the seal.");
        }

        if (!player.isAlive())
        {
            narrator.speak(L"Your strength fails. Darkness takes you... for now.");
            cout << "\n*** You have fallen. Restart from the main menu when you are ready. ***\n";
            return false;
        }
    }

    cout << "\n*** End of Chapter One ***\n";
    cout << "Further chapters — the Whispering Forest, the sealed temple, and the true form of the Demon Lord — await.\n";
    player.inventory.show();
    pressEnter();
    return true;
}

// ================================================
// MAIN
// ================================================

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    Narrator narrator;
    if (!narrator.initialize())
    {
        cerr << "Unable to initialise the narrator.\n";
        cerr << "Check your Windows speech system (SAPI).\n";
        cerr << "The game will continue without voice.\n";
    }

    Player player;
    int mainRace = 0;
    int subRace = 0;
    bool running = true;

    while (running)
    {
        system("cls");

        cout << "\033[38;2;255;190;55m";
        cout << R"(

                 THE LEGEND AWAKENS

           =================================

                  THE LOST KINGDOM

           =================================

        )";
        cout << "\033[0m";

        narrator.speak(L"Welcome to The Lost Kingdom.");
        narrator.speak(L"An ancient land of forgotten kings, powerful magic, and dangerous secrets.");
        narrator.speak(L"Your journey begins now, adventurer.");

        // --- Character creation ---
        cout << "\nEnter your character name: ";
        getline(cin, player.name);
        if (player.name.empty())
            player.name = "Adventurer";

        narrator.speak(L"Greetings, " + toWide(player.name) + L".");
        narrator.speak(L"Before your journey begins, you must choose your race.");

        cout << "\nSelect your preferred race:\n\n";
        cout << "1. Human\n";
        cout << "2. Elf\n";
        cout << "3. Dwarf\n";
        cout << "4. Fairy\n";
        cout << "5. Dark Elf\n";
        cout << "6. Demi-Human\n";
        cout << "7. Elemental\n";

        mainRace = readChoice(1, 7);
        subRace = 0;

        if (mainRace == 6)
        {
            cout << "\nSelect your Demi-Human sub-race:\n";
            cout << "1. Fox\n2. Cat\n3. Bull\n4. Tiger\n";
            subRace = readChoice(1, 4);
        }
        else if (mainRace == 7)
        {
            cout << "\nSelect your Elemental sub-race:\n";
            cout << "1. Fire\n2. Water\n3. Earth\n4. Wind\n";
            subRace = readChoice(1, 4);
        }

        player.raceName = getRaceName(mainRace, subRace);
        applyRaceBonuses(mainRace, subRace, player.health, player.attack, player.defence, player.luck);
        player.maxHealth = player.health;

        cout << "\n=================================\n";
        cout << "Welcome, " << player.name << "!\n";
        cout << "Race: " << player.raceName << "\n";
        cout << "=================================\n";
        player.showStats();

        narrator.speak(L"Your name is " + toWide(player.name) + L".");
        narrator.speak(L"You have chosen the path of the " + toWide(player.raceName) + L".");
        narrator.speak(L"You will set out on a journey to save mankind from the hands of the Demon Lord Malakar.");
        narrator.speak(L"You must search for the holy sword and defeat the Demon Lord before the seal fully breaks.");
        narrator.speak(L"But beware. The Demon Lord holds many dangers, and not all is as it seems.");

        // --- Main hub after creation ---
        bool inHub = true;
        while (inHub && player.isAlive())
        {
            player.showStats();
            cout << "\nWhat would you like to do?\n\n";
            cout << "1. Start Prologue & Chapter One\n";
            cout << "2. View inventory\n";
            cout << "3. Exit Game\n";

            int hub = readChoice(1, 3);

            if (hub == 1)
            {
                playPrologue(narrator, player);
                if (player.isAlive())
                    playChapterOne(narrator, player);

                cout << "\nReturn to hub?\n1. Yes\n2. Exit game\n";
                if (readChoice(1, 2) == 2)
                {
                    inHub = false;
                    running = false;
                }
            }
            else if (hub == 2)
            {
                player.inventory.show();
                pressEnter();
            }
            else
            {
                narrator.speak(L"Farewell, " + toWide(player.name) + L".");
                narrator.speak(L"The Lost Kingdom will remember your name.");
                cout << "\nThank you for playing The Lost Kingdom, " << player.name << "!\n";
                inHub = false;
                running = false;
            }
        }

        if (!player.isAlive())
        {
            cout << "\n1. Create a new character\n2. Exit\n";
            if (readChoice(1, 2) == 2)
                running = false;
            else
            {
                player = Player();
            }
        }
    }

    cout << "\nPress ENTER to close the game...";
    cin.get();
    return 0;
}