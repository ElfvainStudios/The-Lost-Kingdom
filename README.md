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

- Character creation with Guardian, Ranger and Arcanist classes
- Connected 2D world map and location-based travel
- Location exploration and discovery
- NPC dialogue and a starter quest
- Quest journal with objectives, progress and rewards
- Turn-based combat with attack, defend, potion and retreat actions
- Enemy encounters in the forest, crossroads, ruins and lake
- Character stats, experience, gold and level progression
- Inventory and healing items
- Save/load for character, inventory, location, map discovery and quest progress

## Console commands

| Command | Action |
|---|---|
| `travel` | Choose a connected destination |
| `explore` | Search the current location |
| `talk` | Speak with nearby characters |
| `fight` | Start a turn-based encounter |
| `quests` | View the quest journal |
| `map` | Display the 2D world map |
| `status` | View character attributes |
| `inventory` | View carried items |
| `rest` | Recover health in Willowmere |
| `save` / `load` | Save or restore a session |
| `help` | Show command help |
| `quit` | End the session |

During combat, enter `attack`, `defend`, `potion` or `run`.

Save data is written to `savegame.dat` in the current working directory. This local file is excluded from Git.

## Project architecture

- [Game architecture and development roadmap](./docs/GAME_ARCHITECTURE.md)
- `Game/Engine/` — game loop and shared player/world types
- `Game/World/` — connected 2D location graph
- `Game/Systems/` — combat, dialogue, quests and saving
- `Game/Components/Chapters/` — chapter-based narrative content

## Development status

The core gameplay systems are being built incrementally. The next stage is to expand the world, add more NPCs and quest chains, and develop the opening chapter into a complete playable story.
