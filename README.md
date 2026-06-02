# PokerGame

A desktop Texas Hold'em poker game simulator developed in C++23 using SFML and TGUI.

The application supports both human-vs-bot gameplay and fully automated bot-vs-bot simulations. The project was developed as a diploma thesis and demonstrates the implementation of poker game logic, poker combination evaluation algorithms, AI-controlled players, and a graphical user interface, including Monte Carlo-based probability estimation with single-threaded and multi-threaded execution modes.

## Download

Download the latest release here:  
https://github.com/BanditFromSiege/PokerGame/releases/latest

## Features

* Texas Hold'em poker implementation
* Human player vs AI bots
* AI bots playing against each other
* Automatic hand evaluation
* Winner determination and pot distribution
* Monte Carlo probability simulation
* Single-threaded (sequenced) and multi-threaded (parallel) computation modes
* Graphical user interface built with TGUI
* Rendering powered by SFML
* Player action logging (in-game console and file output)
* UI theme customization with 4 background color options (red, green, blue, purple)
* Designed for 1920x1080 (Full HD) resolution
* Modern C++23 codebase

## Technologies

* C++23
* SFML 3.0
* TGUI 1.11
* Microsoft Visual Studio 2022
* MSVC Compiler

## Screenshots

### Main Menu

<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/bf3f87b4-857c-44f8-9d40-6a2c78af03c2" />

### Settings Menu

<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/376e238b-31a6-4e56-a9a9-e8b2bafdc64e" />

### Game Setup Menu

<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/dcc25a7a-5aac-4529-bf2e-b0ebba61eec8" />

### Gameplay

#### Player Mode

<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/d3f35c19-9632-40a3-9d4e-75745922144d" />
<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/f6beacf2-a097-401a-9052-2c09b92a4342" />

#### Spectator Mode

<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/62ab3a25-cd12-4db5-a2b4-be868d87a522" />
<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/ac42cf54-4c0f-4d33-9b0d-7ede4604c7a5" />

## Project Structure

```text
PokerGame/
├── PokerGame/                    # Main project directory
│   ├── Card_Logic/               # Cards, deck, poker combinations, probability evaluation
│   ├── Game_Logic/               # Game flow, rules, betting, AI logic
│   ├── Render_Logic/             # Rendering and UI layer (SFML + TGUI)
│   ├── Images/                   # Game assets (textures and icon)
│   ├── SFML-3.0.0/               # SFML library
│   ├── TGUI-1.11/                # TGUI library
│   ├── Main.cpp                  # Entry point
│   ├── PokerGame.vcxproj         # Visual Studio project file
│   └── PokerGame.vcxproj.filters
├── .gitignore
├── .gitattributes
└── PokerGame.sln                 # Visual Studio solution
```

## Build Requirements

* Windows 10 / Windows 11
* 1920x1080 resolution (Full HD) display required
* Microsoft Visual Studio 2022
* MSVC with C++23 support

## Build Instructions

1. Clone the repository:

```bash
git clone https://github.com/BanditFromSiege/PokerGame.git
```

2. Open `PokerGame.sln` in Visual Studio 2022.

3. Select the desired configuration:

   * Debug
   * Release

4. Build the solution:

```text
Ctrl + Shift + B
```

5. After building, copy the following DLL files into the output directory (where `PokerGame.exe` is located).

Release build:

From PokerGame/SFML-3.0.0/dll_files
- sfml-graphics-3.dll
- sfml-window-3.dll
- sfml-system-3.dll

From PokerGame/TGUI-1.11/dll_files
- tgui.dll

From PokerGame
- Images folder

Debug build:

From PokerGame/SFML-3.0.0/dll_files
- sfml-graphics-d-3.dll
- sfml-window-d-3.dll
- sfml-system-d-3.dll

From PokerGame/TGUI-1.11/dll_files
- tgui-d.dll

From PokerGame
- Images folder

6. Run the project:

```text
F5
```

## Poker Logic

The project implements the complete Texas Hold'em game flow:

1. Dealing hole cards
2. Pre-flop betting round
3. Flop
4. Turn
5. River
6. Showdown
7. Winner determination

Hand strength is evaluated automatically according to standard poker rules.

## Bot Logic

Computer-controlled players make decisions based on the current game state and the strength of their hand.

Available actions:

* Fold
* Check
* Call
* Raise

Bots can also be used in fully automated simulations where no human player participates.

## Purpose

This project was developed as part of a diploma thesis focused on the design and implementation of a poker simulation system using modern C++ technologies.

## Author

- Ivan Markov
- Vladyslav Bublii

2026
