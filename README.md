# The Lost Kingdom

A 2D-world, text-based fantasy role-playing game written in standard C++17.

Explore a connected world of villages, forests, ruins and forgotten places through a command-driven console interface. Build your character, speak with inhabitants, take quests, fight enemies and uncover the kingdom's history.

## Build requirements

- CMake 3.16 or newer
- A C++17 compatible compiler

```sh
cmake -S . -B build
cmake --build build
```

Run the generated `TheLostKingdom` executable from the build directory.

## Current gameplay

- ASCII-art terminal title screen with New Adventure, Continue and Quit options
- Optional Free Adventure or Main Story start; the main story can also be started later with `story`
- Character creation with Guardian, Ranger and Arcanist classes
- Connected 2D world map and location-based travel
- Location exploration and discovery
- NPC dialogue and a starter quest
- Quest journal with objectives, progress and rewards
- Turn-based combat with attack, defend, potion and retreat actions
- Enemy encounters in the forest, crossroads, ruins and lake
- Character stats, experience, gold and level progression
- Inventory, healing items, readable books and spell learning
- Mana, Minor Mend healing magic and Arcane Spark combat magic
- Narrator callouts for story and world transitions
- Version 2 saves preserve story state, mana and learned spells while version 1 saves remain loadable
- Save/load for character, inventory, location, map discovery and quest progress

## Console commands

| Command | Action |
|---|---|
| `travel` | Choose a connected destination |
| `explore` | Search the current location |
| `talk` | Speak with nearby characters |
| `fight` | Start a turn-based encounter |
| `quests` | View the quest journal |
| `story` | Begin the main story at any time |
| `read` | Read a book and learn its knowledge or spell |
| `spells` | View learned spells and mana |
| `cast` | Cast a learned healing spell |
| `use` | Use a carried healing potion |
| `map` | Display the 2D world map |
| `status` | View character attributes |
| `inventory` | View carried items |
| `rest` | Recover health in Willowmere |
| `save` / `load` | Save or restore a session |
| `help` | Show command help |
| `quit` | End the session |

During combat, enter `attack`, `defend`, `potion`, `magic` or `run`. `magic` casts Arcane Spark when learned. `cast` outside combat uses Minor Mend.

Save data is written to `savegame.dat` in the current working directory. This local file is excluded from Git.

## Project architecture

- [Game architecture and development roadmap](./docs/GAME_ARCHITECTURE.md)
- `Game/Engine/` — game loop and shared player/world types
- `Game/World/` — connected 2D location graph
- `Game/Systems/` — combat, dialogue, quests and saving
- `Game/Components/Chapters/` — chapter-based narrative content

## Development status

The choice-driven opening, narrator, starter books and first spells are now connected to the game loop. This remains an active development build: the main chapter quest chain, broader item/equipment catalogue, additional spell effects, automated gameplay tests and release validation are still required before a production release.
