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

## Choice-driven game flow

The title screen offers a new adventure, continue from save, or quit. A new character can begin in Free Adventure or Main Story mode. Free Adventure skips the prologue and leaves exploration, combat and side quests available immediately. The `story` command starts the prologue later without resetting the character or world. Story state is stored in save format v2; version 1 saves remain readable.

The Keeper of Forgotten Tales is the narrator voice used for the prologue and selected world transitions. The terminal title screen uses built-in ASCII artwork and does not require external image assets. Windows virtual-terminal processing is enabled when the output handle supports it.

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

### Books and magic
- Exploration can uncover the Field Guide to Moon Herbs and the Codex of the First Seal.
- Reading books teaches spells, including Minor Mend and Arcane Spark.
- Mana is tracked on the player and restored by resting in Willowmere or exploring Moonwell Lake.
- Minor Mend restores health outside combat; Arcane Spark is an available combat action.

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
| `story` | Begin the main story at any time |
| `read` | Read a carried book |
| `spells` | View learned spells and mana |
| `cast` | Cast Minor Mend |
| `use` | Use a healing potion |
| `map` | Display the 2D map |
| `status` | View character attributes |
| `inventory` | View carried items |
| `rest` | Recover health in Willowmere |
| `save` / `load` | Save or restore a session |
| `help` | Show available commands |
| `quit` | End the session |

Combat actions are entered as `attack`, `defend`, `potion`, `magic` or `run`.

## Build

Use CMake 3.16+ and a C++17 compiler:

```sh
cmake -S . -B build
cmake --build build
```

On Windows, run the executable produced in the build directory. On Linux/macOS, run `./build/TheLostKingdom`.

## Release-readiness status

This branch is an integrated development milestone, not a production release. Before release, add automated tests for game-flow choices, combat, quest repetition, item use, spell costs, and save compatibility; complete the main-story chapter objectives; expand the item/equipment and magic catalogues; and verify the Linux and Windows CI builds.

## Next development milestones

1. More NPCs, branching dialogue and multi-stage quest chains.
2. Equipment, weapons, armour, item use and shops.
3. More locations, secrets, encounters and region-specific events.
4. Story flags and chapter progression.
5. Automated tests for world travel, combat rules, quest rewards and save compatibility.
