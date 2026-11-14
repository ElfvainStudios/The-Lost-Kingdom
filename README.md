```txt
  THE LOST KINGDOM
```

# The Lost Kingdom

A 2D-world, text-based fantasy role-playing game written in standard C++17.

Explore a connected world of villages, forests, ruins and forgotten places through a command-driven console interface. Build your character, discover locations, uncover the kingdom's history and grow stronger as the story unfolds.

## Project documentation

- [Game architecture](./docs/GAME_ARCHITECTURE.md)
- [Contributing guide](./CONTRIBUTING.md)

## Build requirements

- CMake 3.16 or newer
- A C++17 compatible compiler

```sh
cmake -S . -B build
cmake --build build
```

Run the generated `TheLostKingdom` executable from the build directory.

## Current foundation

- Character name and class selection
- Command-driven exploration loop
- Connected 2D location map
- Travel between discovered world locations
- Character status and inventory views
- Location-specific exploration and safe resting

## Planned RPG systems

NPC dialogue, quests, turn-based combat, equipment, shops, progression, save/load and chapter-based story content.

The game is in active development; the first beta release will follow completion and testing of the opening floors and storyline.
