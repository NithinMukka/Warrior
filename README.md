# Warrior: A C++ Text Adventure

A classic text-based adventure game written in modern C++. The player awakens in a dungeon cell and must explore, collect items, and solve puzzles to find their way to freedom.

> You awake in a cold, dark cell. You must escape.

This project was built as a hands-on exercise to practice and demonstrate core Object-Oriented Programming (OOP) principles in C++.

## Class Diagram

![Class Diagram Screenshot](https://raw.githubusercontent.com/NithinMukka/Warrior/main/warrior_class.png)

## Screenshot

![Game Screenshot](https://raw.githubusercontent.com/NithinMukka/Warrior/main/gameplay.png)

## Features

*   **Object-Oriented Design:** The world is built from a set of robust classes (`Game`, `Room`, `Player`, `Item`, `Connection`, `Enemy`, etc.) that manage their own state and behavior.
*   **Modern C++ Memory Management:** Uses `std::unique_ptr` for automatic memory management, preventing memory leaks and demonstrating the RAII (Resource Acquisition Is Initialization) principle.
*   **Interactive Command Parser:** A simple but effective parser that handles multi-word arguments and is case-insensitive.
*   **Dynamic World:** The game world is created at runtime by connecting `Room` objects, making it easily expandable.
*   **Puzzle Mechanics:** Features locked doors that require specific keys, encouraging exploration and item management.
*   **Simple Combat:** Includes enemies that the player must find the correct item to defeat.
*   **NPC Interaction:** Players can talk to non-player characters to get hints.

## How to Compile and Run

This project is contained in a single `main.cpp` file and uses only standard C++ libraries.

### Prerequisites

*   A C++ compiler that supports the C++17 standard (e.g., g++, Clang, or MSVC).

### Compilation

Open a terminal or command prompt, navigate to the project directory, and run the following command:

```bash
g++ main.cpp -o warrior -std=c++17
