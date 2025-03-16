# Termiverve

## A Minecraft-based [florr.io](https://florr.io) Simulator

Termiverve is a 2D survival game that combines elements from Minecraft and florr.io, offering a unique gaming experience in a terminal-like environment.

## 🎮 Game Overview

Termiverve is a grid-based survival game where players can:
- Navigate a procedurally generated world
- Gather resources (wood, stone, leaves, sticks)
- Craft items at workbenches
- Defend against hostile mobs
- Build and modify the environment

## 🔧 Game Mechanics

- **Resource Gathering**: Break trees and obstacles to collect materials
- **Combat System**: Fight against various mobs with different weapons
- **Crafting**: Use workbenches to create tools and items
- **Survival Elements**: Manage health while exploring the world

## 🖥️ Technical Details

- Written in C++ using Windows API for graphics rendering
- Grid-based movement and interaction system
- Simple collision detection
- Entity-based game objects

## 🚀 Getting Started

### Prerequisites
- Windows operating system
- C++ compiler (if building from source)

### Installation
1. Clone this repository
   ```
   git clone https://github.com/horsemeatlasagne/termiverve.git
   ```
2. Run the executable `main.exe` or compile the source code

### Controls
- **WASD**: Move the player
- **Mouse Click**: Interact with objects/attack
- **Number Keys**: Select items from inventory

## 🛠️ Building from Source

To compile the game from source:
1. Ensure you have a C++ compiler installed
2. Compile main.cpp with Windows libraries:
   ```
   g++ main.cpp -o termiverve.exe -luser32 -lgdi32 -mwindows.
   ```

## What does the project mean to me?
Showcasing the power and extreme speed of traditional c++ using antique win32 api, low-level programming, combined with simplistic minimalist algorithms.

## 🤝 Contributing

Contributions are welcome! Feel free to submit pull requests or open issues to improve the game.

## 📝 License

This project is open source and subject to GPL v3. See LICENSE for more details.

## 🙏 Acknowledgements

- Inspired by [florr.io](https://florr.io) and Minecraft
- Thanks to all contributors and testers

