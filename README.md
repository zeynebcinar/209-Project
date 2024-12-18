# Dungeon Game

## Table of Contents
1. [Introduction](#introduction)
2. [Gameplay](#gameplay)
3. [Commands](#commands)
4. [How to Compile and Run](#how-to-compile-and-run)
5. [Code Structure](#code-structure)
6. [Game Logic](#game-logic)
7. [Features](#features)
8. [Credits](#credits)


## Introduction
**Dungeon Adventure Game** is a command-line-based text adventure game written in C. The game simulates an exciting dungeon exploration where players navigate rooms, collect items, and battle creatures. The project is designed to enhance understanding of input parsing, dynamic memory management, and file I/O operations.

## Gameplay
You begin your journey in a mysterious dungeon filled with rooms containing items, traps, and creatures. As a player, you must navigate through the dungeon using commands, collect valuable items, and fight creatures to survive. Your health and strength determine your chances of survival.

### Objective
- Explore the dungeon.
- Collect items to increase your strength.
- Defeat creatures to progress.
- Avoid traps to maintain your health.
- Survive as long as possible while navigating through the dungeon.

## Commands
Here are the available commands in the game:

### In-Game Commands
- **`map`**: Display the dungeon map.
- **`look`**: Display the current room's description, including any items or creatures.
- **`up/down/left/right`**: Move to a connected room.
- **`pickup <item_name>`**: Pick up an item in the current room.
- **`drop <item_name>`**: Drop an item from your inventory in the current room.
- **`attack`**: Attack a creature in the room.
- **`inventory`**: Display your current inventory and stats.

### Menu Commands
- **`save <filepath>`**: Save the current game state to a file.
- **`load <filepath>`**: Load a saved game state from a file.
- **`list`**: Display a list of saved games in the directory.
- **`help`**: Display the list of available commands.
- **`exit`**: Exit the game.

---
## How to Compile and Run Running the Game
Once compiled, you can run the game using:
### Compilation
1. This project includes a **Makefile** for easy compilation. To compile the game, simply run:
   ```bash
   make
   ```
2. Run the game:
   ```bash
   ./dungeon_game
   or
   make run
   ``` 
3. Cleaning Build Files:
   ```bash
   make clean
   ```

## Code Structure
- `dungeon_game.c`: The main source code file.
- `Makefile`: Simplifies the compilation process.
- `README.md`: This documentation file.

## Game Logic

### Player
**Stats**: Health, strength, and inventory capacity.
**Health**: Reduces during combat or when stepping into a trapped room.
**Strength**: Increases when items are picked up and decreases when dropped.

### Rooms
Each room is unique and connected to others via up, down, left, and right directions.
Rooms can contain:
- **Items**: Pickable objects that increase the player's stats.
- **Creatures**: Opponents that the player can fight.
- **Traps**: Reduce the player's health upon entry.

### Combat
- Combat follows a turn-based system where the player and creature exchange attacks.
- Player strength determines the damage dealt to the creature.
- Creature strength reduces the player's health during combat.

## Features
- Dynamic Room Connections: Rooms are dynamically allocated with unique descriptions and connections.
- Traps: Randomly placed traps in rooms reduce player health.
- Inventory Management: Limited inventory capacity for strategic gameplay.
- Save and Load: Save the game state to a file and resume later.
- Error Handling: Robust error handling for invalid commands and file I/O.

## Credits
This game was developed as part of a C programming assignment. Special thanks to the course instructors for providing guidance and support.

- Developer: Zeyneb Cinar
- Institution: Ankara Yildirim Beyazit University
- Submission Date: December 17, 2024# Dungeon-Game
# 209-Project
