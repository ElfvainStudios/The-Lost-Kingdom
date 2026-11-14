# The Lost Kingdom — Game Architecture

## Game format

The Lost Kingdom is a 2D-world, text-based role-playing game written in standard C++17. The world is represented by connected locations and map coordinates; the player explores by choosing commands and destinations. There is no 3D renderer, real-time movement, or graphics dependency.

## Source layout

- `Game/app.cpp` — application entry point.
- `Game/Engine/` — game loop, shared game types and session state.
- `Game/World/` — 2D location graph, map coordinates, exits and discovery.
- `Game/Systems/` — combat, dialogue, quests and save/load.
- `Game/Components/Chapters/` — chapter content and story scenes.
- `docs/` — design and technical documentation.

## Core gameplay loop

1. Present the current location and its description.
2. Accept a text command.
3. Validate the command against the current game state.
4. Resolve travel, exploration, dialogue, quest, inventory or combat actions.
5. Update player/world state and return to the location view.

## World model

Every location has a stable ID, a display name, descriptive text, 2D map coordinates and a list of connected exits. The player can only travel along defined exits. The current starter map contains Willowmere Village, Whispering Forest, Old King's Crossroads, Sunken Ruins and Moonwell Lake.

## Implemented systems

### Character and progression
- Character name and Guardian, Ranger or Arcanist starting class.
- Health, attack, defence, experience, gold and levels.
- Experience rewards from encounters and quests.
- Level-ups increase maximum health and attack.

### Exploration and travel
- Command-driven travel along connected location exits.
- Location descriptions and exploration events.
- A text representation of the 2D world map.
- Resting at the village and a small healing effect at the lake.

### Combat
- Turn-based enemy encounters in the forest, crossroads, ruins and lake.
- Attack, defend, use a healing potion or retreat.
- Enemy-specific health, attack strength and rewards.
- Defeat returns the player to Willowmere with 1 health and a small gold penalty.

### NPCs and quests
- Elder Rowan in Willowmere offers the first quest.
- The starter quest, **A Village in Need**, asks for three moon herbs from the forest.
- Quest journal displays active/completed state and progress.
- Completion grants experience and gold.

### Save and load
- `save` writes `savegame.dat` in the current working directory.
- `load` restores character attributes, inventory, current location, discovered locations and starter quest state.
- Save format is versioned so future formats can be migrated deliberately.
- Local save data and build output are ignored by Git.

## Console commands

| Command | Purpose |
|---|---|
| `travel` | Choose a connected location |
| `explore` | Search the current location |
| `talk` | Speak with nearby characters |
| `fight` | Start a turn-based encounter |
| `quests` | View quest progress |
| `map` | Display the 2D map |
| `status` | View character attributes |
| `inventory` | View carried items |
| `rest` | Recover health in Willowmere |
| `save` / `load` | Save or restore a session |
| `help` | Show available commands |
| `quit` | End the session |

Combat actions are entered as `attack`, `defend`, `potion` or `run`.

## Build

Use CMake 3.16+ and a C++17 compiler:

```sh
cmake -S . -B build
cmake --build build
```

On Windows, run the executable produced in the build directory. On Linux/macOS, run `./build/TheLostKingdom`.

## Next development milestones

1. More NPCs, branching dialogue and multi-stage quest chains.
2. Equipment, weapons, armour, item use and shops.
3. More locations, secrets, encounters and region-specific events.
4. Story flags and chapter progression.
5. Automated tests for world travel, combat rules, quest rewards and save compatibility.
