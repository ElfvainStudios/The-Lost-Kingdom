# The Lost Kingdom — Game Architecture

## Game format

The Lost Kingdom is a 2D world, text-based role-playing game written in standard C++17. The world is represented by connected locations and map coordinates; the player explores by choosing commands and destinations. There is no 3D renderer, real-time movement, or graphics dependency.

## Target source layout

- `Game/app.cpp` — application entry point.
- `Game/Engine/` — game loop, shared game types, input and session state.
- `Game/World/` — 2D location graph, map coordinates, exits and discovery.
- `Game/Entities/` — player, NPC, enemy and interactable definitions.
- `Game/Systems/` — combat, quests, inventory, progression and save/load systems.
- `Game/UI/` — console presentation, menus, dialogue and command parsing.
- `Game/Data/` — story, item, enemy and location definitions kept separate from logic.
- `Game/Components/Chapters/` — chapter content and story scenes.
- `docs/` — design and technical documentation.

## Core gameplay loop

1. Present the current location and its description.
2. Accept a text command.
3. Validate the command against the current game state.
4. Resolve travel, exploration, dialogue, quest, inventory or combat actions.
5. Update player/world state and return to the location view.

## World model

Every location has a stable ID, a display name, descriptive text, 2D map coordinates and a list of connected exits. The player can only travel along defined exits. New locations can be added to the world data without changing the game loop.

## Planned systems

- Character creation and class-based starting attributes.
- Location exploration and discovery.
- NPC dialogue and branching choices.
- Quest journal and objective tracking.
- Turn-based combat and enemy encounters.
- Items, equipment, shops and crafting.
- Experience, levels and character attributes.
- Save/load with versioned save data.
- Chapter progression and story flags.

## Build

Use CMake 3.16+ and a C++17 compiler:

```sh
cmake -S . -B build
cmake --build build
./build/TheLostKingdom
```

On Windows, run the executable produced in the build directory.
