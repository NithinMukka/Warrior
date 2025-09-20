Warrior: A C++ Text Adventure
A classic text-based adventure game written in modern C++. The player awakens in a dungeon cell and must explore, collect items, and solve puzzles to find their way to freedom.
You awake in a cold, dark cell. You must escape.
This project was built as a hands-on exercise to practice and demonstrate core Object-Oriented Programming (OOP) principles in C++.
Screenshot
![Game Screenshot](httpss://raw.githubusercontent.com/NithinMukka/Warrior/your-repo-name/main/gameplay.png)
Features
Object-Oriented Design: The world is built from a set of robust classes (Game, Room, Player, Item, Connection, Enemy, etc.) that manage their own state and behavior.
Modern C++ Memory Management: Uses std::unique_ptr for automatic memory management, preventing memory leaks and demonstrating the RAII (Resource Acquisition Is Initialization) principle.
Interactive Command Parser: A simple but effective parser that handles multi-word arguments and is case-insensitive.
Dynamic World: The game world is created at runtime by connecting Room objects, making it easily expandable.
Puzzle Mechanics: Features locked doors that require specific keys, encouraging exploration and item management.
Simple Combat: Includes enemies that the player must find the correct item to defeat.
NPC Interaction: Players can talk to non-player characters to get hints.
How to Compile and Run
This project is contained in a single main.cpp file and uses only standard C++ libraries.
Prerequisites
A C++ compiler that supports the C++17 standard (e.g., g++, Clang, or MSVC).
Compilation
Open a terminal or command prompt, navigate to the project directory, and run the following command:
code
Bash
g++ main.cpp -o dungeon_escape -std=c++17
g++ main.cpp: Compiles the source code.
-o dungeon_escape: Specifies the name of the output executable file.
-std=c++17: Ensures the code is compiled with the C++17 standard, which is required for features used in the project.
Running the Game
Once compiled, you can run the game from the terminal:

./dungeon_escape```

## How to Play

The game is controlled by typing commands. The parser understands a `[command] [argument]` format.

| Command | Example | Description |
| :--- | :--- | :--- |
| `go` | `go north` | Move the player in the specified direction. |
| `look` | `look` | Provides a detailed description of the current room. |
| `take` | `take Rusty Key` | Adds an item from the room to your inventory. |
| `use` | `use Rusty Key` | Attempts to use an item from your inventory, typically on a locked door. |
| `attack` | `attack Skeleton` | Attacks an enemy in the current room. |
| `inventory` | `inventory` | Lists all items currently in your inventory. |
| `talk` | `talk Guard` | Initiates a conversation with an NPC. |
| `help` | `help` | Displays the list of available commands. |
| `quit` | `quit` | Exits the game. |

## Code Overview: OOP Design

The project is a practical demonstration of several key OOP concepts:

*   **Encapsulation:** Each class manages its own data. For example, the `Room` class is solely responsible for the items and connections within it.
*   **Inheritance:** `Player`, `Enemy`, and `NonPlayerChar` all inherit from a common `Character` base class, sharing common attributes and interfaces.
*   **Polymorphism:** The use of `virtual` functions in the `Character` class allows for future expansion where different character types could have unique behaviors.
*   **Composition & Aggregation:** The `Game` class *owns* the `Room`s (`std::unique_ptr`), representing composition. A `Player` *observes* their `currentRoom` (a raw `Room*`), representing aggregation/association. This clear ownership model is central to the design.

## Future Enhancements

This project provides a solid foundation that can be expanded upon. Potential future features include:

*   **Saving and Loading:** Implement functionality to save the game state to a file and load it later.
*   **More Complex Puzzles:** Introduce tasks that require using items on other items or specific objects within a room.
*   **Advanced Parser:** Upgrade the parser to understand more complex sentences (e.g., `use key on iron door`).
*   **Player Stats:** Add health points, stats, and a more detailed combat system.
