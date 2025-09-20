#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <sstream>
#include <algorithm>
#include <cctype>

// Forward declarations
class Player;
class Room;
class NonPlayerChar; // Forward declare for GameView

// ... (Item, Task, Connection, Room, Character, Player classes are unchanged) ...
// NOTE: For brevity, unchanged classes are omitted here. Full code is below.

// ---------------- All classes from Item to Player are here ----------------
class Item {
    std::string name;
    std::string description;
public:
    Item(std::string n, std::string desc) : name(std::move(n)), description(std::move(desc)) {}
    const std::string& getName() const { return name; }
    const std::string& getDescription() const { return description; }
};

class Task {
    std::string taskName;
    std::string taskDescription;
    std::string requiredItemName;
    bool completed = false;

public:
    Task(std::string name, std::string description, std::string itemToComplete)
        : taskName(std::move(name)), taskDescription(std::move(description)),
          requiredItemName(std::move(itemToComplete)) {}

    const std::string& getTaskName() const { return taskName; }
    const std::string& getTaskDescription() const { return taskDescription; }
    const std::string& getRequiredItemName() const { return requiredItemName; }
    bool isCompleted() const { return completed; }

    void completeTask() { completed = true; }
};

class Connection {
    std::string direction;
    Room* destination = nullptr;
    bool isLocked = false;
    std::string keyName;

public:
    Connection(std::string direct, Room& room)
        : direction(std::move(direct)), destination(&room) {}

    Connection(std::string direct, Room& room, std::string key)
        : direction(std::move(direct)), destination(&room), isLocked(true), keyName(std::move(key)) {}

    const std::string& getDirection() const { return direction; }
    Room* getDestination() const { return destination; }
    bool getIsLocked() const { return isLocked; }

    bool unlock(const std::string& playerKey) {
        if (isLocked && playerKey == keyName) {
            isLocked = false;
            std::cout << "The " << keyName << " fits perfectly. The door unlocks with a click." << std::endl;
            return true;
        }
        return false;
    }
};

class Room {
    std::string name;
    std::string description;
    std::vector<std::unique_ptr<Item>> items;
    std::vector<std::unique_ptr<Task>> tasks;
    std::vector<std::unique_ptr<Connection>> connections;

public:
    Room(std::string n, std::string desc) : name(std::move(n)), description(std::move(desc)) {}

    const std::string& getName() const { return name; }
    const std::string& getDescription() const { return description; }

    std::vector<std::unique_ptr<Connection>>& getConnections() { return connections; }
    const std::vector<std::unique_ptr<Connection>>& getConnections() const { return connections; }
    
    const std::vector<std::unique_ptr<Item>>& getItems() const { return items; }
    const std::vector<std::unique_ptr<Task>>& getTasks() const { return tasks; }

    void addConnection(std::unique_ptr<Connection> conn) { connections.push_back(std::move(conn)); }
    void addTask(std::unique_ptr<Task> task) { tasks.push_back(std::move(task)); }
    void addItem(std::unique_ptr<Item> item) { items.push_back(std::move(item)); }

    std::unique_ptr<Item> takeItem(const std::string& itemName) {
        auto it = std::find_if(items.begin(), items.end(),
            [&](const std::unique_ptr<Item>& item) {
                std::string lowerItemName = item->getName();
                std::transform(lowerItemName.begin(), lowerItemName.end(), lowerItemName.begin(), ::tolower);
                return lowerItemName == itemName;
            });

        if (it != items.end()) {
            std::unique_ptr<Item> foundItem = std::move(*it);
            items.erase(it);
            return foundItem;
        }
        return nullptr;
    }
};

class Character {
    std::string name;
protected:
    Room* currentRoom = nullptr;
public:
    Character(std::string name, Room& room) : name(std::move(name)), currentRoom(&room) {}
    virtual ~Character() = default;

    const std::string& getName() const { return name; }
    Room* getCurrentRoom() const { return currentRoom; }
    void setCurrentRoom(Room* room) { currentRoom = room; }
};

class Player : public Character {
    std::vector<std::unique_ptr<Item>> inventory;
public:
    Player(std::string name, Room& room) : Character(std::move(name), room) {}

    bool hasItem(const std::string& name) const {
        for (const auto& item : inventory) {
            std::string lowerItemName = item->getName();
            std::transform(lowerItemName.begin(), lowerItemName.end(), lowerItemName.begin(), ::tolower);
            if (lowerItemName == name) return true;
        }
        return false;
    }

    void takeItem(std::unique_ptr<Item> item) {
        inventory.push_back(std::move(item));
    }

    const std::vector<std::unique_ptr<Item>>& getInventory() const {
        return inventory;
    }
};

// --- START OF CHANGES ---

// ---------------- Enemy ----------------
class Enemy : public Character {
    std::string requiredItemToDefeat;
public:
    Enemy(std::string name, Room& room, std::string itemToDefeat)
        : Character(std::move(name), room), requiredItemToDefeat(std::move(itemToDefeat)) {}

    // Now simply checks if the player CAN win, doesn't automatically resolve combat.
    bool isDefeatedBy(Player& player) {
        std::string lowerRequired = requiredItemToDefeat;
        std::transform(lowerRequired.begin(), lowerRequired.end(), lowerRequired.begin(), ::tolower);
        return player.hasItem(lowerRequired);
    }
};

// ---------------- NonPlayerChar ----------------
class NonPlayerChar : public Character {
    std::string dialogue;
public:
    NonPlayerChar(std::string name, Room& room, std::string dialogue)
        : Character(std::move(name), room), dialogue(std::move(dialogue)) {}

    void talk() const { std::cout << "'" << dialogue << "'" << std::endl; }
};


// ---------------- GameView ----------------
class GameView {
public:
    void displayWelcome() const {
        std::cout << "========================================" << std::endl;
        std::cout << "        Dungeon Escape Adventure!       " << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "You awake in a cold, dark cell. You must escape." << std::endl;
        std::cout << "Commands: go [dir], look, take [item], use [item], attack [enemy], inventory, talk [person], help, quit" << std::endl;
    }

    // FIX 1: Upgraded function to display NPCs and Enemies present in the room.
    void displayRoom(const Room* room, const std::vector<std::unique_ptr<NonPlayerChar>>& npcs, const std::vector<std::unique_ptr<Enemy>>& enemies) const {
        if (!room) return;
        std::cout << "\n--- You are in: " << room->getName() << " ---" << std::endl;
        std::cout << room->getDescription() << std::endl;
        
        // Display NPCs
        for (const auto& npc : npcs) {
            if (npc->getCurrentRoom() == room) {
                std::cout << "You see a " << npc->getName() << " here." << std::endl;
            }
        }

        // Display Enemies
        for (const auto& enemy : enemies) {
            if (enemy->getCurrentRoom() == room) {
                std::cout << "A menacing " << enemy->getName() << " stands here!" << std::endl;
            }
        }

        std::cout << "Items: ";
        if (room->getItems().empty()) std::cout << "none.";
        else for (const auto& item : room->getItems()) std::cout << "[" << item->getName() << "] ";
        std::cout << std::endl;
        
        std::cout << "Exits: ";
        if (room->getConnections().empty()) std::cout << "none.";
        else for (const auto& conn : room->getConnections()) std::cout << "[" << conn->getDirection() << "] ";
        std::cout << std::endl;
    }

    void displayInventory(const Player& player) const {
        std::cout << "Inventory: ";
        if (player.getInventory().empty()) std::cout << "nothing.";
        else for (const auto& item : player.getInventory()) std::cout << "[" << item->getName() << "] ";
        std::cout << std::endl;
    }

    void displayHelp() const {
        std::cout << "\n--- Help Menu ---\n"
            << " - go [direction]    (e.g., 'go north')\n"
            << " - look              (Describes the current room)\n"
            << " - take [item name]  (e.g., 'take Rusty Key')\n"
            << " - use [item name]   (Attempts to use an item on a locked door)\n"
            << " - attack [enemy]    (e.g., 'attack Skeleton')\n"
            << " - inventory         (Shows your items)\n"
            << " - talk [person]     (e.g., 'talk Guard')\n"
            << " - quit              (Exits the game)\n";
    }
};

// ---------------- Game ----------------
class Game {
    std::vector<std::unique_ptr<Room>> rooms;
    std::vector<std::unique_ptr<NonPlayerChar>> npcs;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::unique_ptr<Player> player;
    GameView view;
    bool isRunning = true;

    void setupWorld() {
        auto prisonCell = std::make_unique<Room>("Prison Cell", "A damp, cold prison cell. A heavy iron door is to the north.");
        auto guardRoom = std::make_unique<Room>("Guard Room", "A messy room with a desk and a weapon rack. A guard is asleep in a chair.");
        auto armory = std::make_unique<Room>("Armory", "Racks of old, dusty weapons line the walls. A heavy wooden door is to the south.");
        auto tortureChamber = std::make_unique<Room>("Torture Chamber", "A grim room with unsettling devices. A reinforced door is to the east.");
        auto wardensOffice = std::make_unique<Room>("Warden's Office", "A surprisingly tidy office with a large oak desk. A single, sturdy door leads to the east.");
        auto courtyard = std::make_unique<Room>("Courtyard", "You step into the open air! The gate to freedom is just ahead.");

        Room* pPrisonCell = prisonCell.get();
        Room* pGuardRoom = guardRoom.get();
        Room* pArmory = armory.get();
        Room* pTortureChamber = tortureChamber.get();
        Room* pWardensOffice = wardensOffice.get();
        Room* pCourtyard = courtyard.get();

        pPrisonCell->addItem(std::make_unique<Item>("Rusty Key", "An old, rusty key."));
        pArmory->addItem(std::make_unique<Item>("Rusty Sword", "A short sword, pitted but sharp enough."));
        pTortureChamber->addItem(std::make_unique<Item>("Ornate Key", "A fancy key made of brass."));
        pWardensOffice->addItem(std::make_unique<Item>("Master Key", "A large, heavy iron key. It looks important."));

        pPrisonCell->addConnection(std::make_unique<Connection>("north", *pGuardRoom, "Rusty Key"));
        pGuardRoom->addConnection(std::make_unique<Connection>("south", *pPrisonCell));
        pGuardRoom->addConnection(std::make_unique<Connection>("east", *pArmory));
        pArmory->addConnection(std::make_unique<Connection>("west", *pGuardRoom));
        pArmory->addConnection(std::make_unique<Connection>("south", *pTortureChamber));
        pTortureChamber->addConnection(std::make_unique<Connection>("north", *pArmory));
        pTortureChamber->addConnection(std::make_unique<Connection>("east", *pWardensOffice, "Ornate Key"));
        pWardensOffice->addConnection(std::make_unique<Connection>("west", *pTortureChamber));
        pWardensOffice->addConnection(std::make_unique<Connection>("east", *pCourtyard, "Master Key"));

        npcs.push_back(std::make_unique<NonPlayerChar>("Guard", *pGuardRoom, "Zzzzz... The warden keeps the fancy keys in the scary room downstairs... Zzzzz..."));
        enemies.push_back(std::make_unique<Enemy>("Skeleton", *pTortureChamber, "Rusty Sword"));
        
        player = std::make_unique<Player>("Hero", *pPrisonCell);
        
        rooms.push_back(std::move(prisonCell));
        rooms.push_back(std::move(guardRoom));
        rooms.push_back(std::move(armory));
        rooms.push_back(std::move(tortureChamber));
        rooms.push_back(std::move(wardensOffice));
        rooms.push_back(std::move(courtyard));
    }

    bool checkWin() const {
        return player->getCurrentRoom()->getName() == "Courtyard";
    }

    // FIX 2: Added 'attack' command and modified 'talk' to be case-insensitive.
    void processCommand(std::string inputLine) {
        std::transform(inputLine.begin(), inputLine.end(), inputLine.begin(), ::tolower);

        std::stringstream ss(inputLine);
        std::string command, argument;
        ss >> command >> std::ws;
        std::getline(ss, argument);

        if (command == "quit") isRunning = false;
        else if (command == "look") view.displayRoom(player->getCurrentRoom(), npcs, enemies);
        else if (command == "inventory") view.displayInventory(*player);
        else if (command == "help") view.displayHelp();
        else if (command == "go") {
            Room* currentRoom = player->getCurrentRoom();
            bool connectionExists = false;
            for (auto& conn : currentRoom->getConnections()) {
                if (conn->getDirection() == argument) {
                    connectionExists = true;
                    if (conn->getIsLocked()) {
                        std::cout << "The door is locked." << std::endl;
                    } else {
                        player->setCurrentRoom(conn->getDestination());
                    }
                    break;
                }
            }
            if (!connectionExists && !argument.empty()) {
                std::cout << "You can't go that way." << std::endl;
            }
        } else if (command == "take") {
            auto item = player->getCurrentRoom()->takeItem(argument);
            if (item) {
                std::cout << "You took the " << item->getName() << "." << std::endl;
                player->takeItem(std::move(item));
            } else {
                std::cout << "There is no '" << argument << "' here." << std::endl;
            }
        } else if (command == "use") {
            if (player->hasItem(argument)) {
                bool unlocked = false;
                std::string originalCaseItemName;
                for(const auto& item : player->getInventory()){
                    std::string lowerItemName = item->getName();
                    std::transform(lowerItemName.begin(), lowerItemName.end(), lowerItemName.begin(), ::tolower);
                    if(lowerItemName == argument){
                        originalCaseItemName = item->getName();
                        break;
                    }
                }

                for(auto& conn : player->getCurrentRoom()->getConnections()) {
                    if (conn->unlock(originalCaseItemName)) {
                        unlocked = true;
                        break;
                    }
                }
                if (!unlocked) std::cout << "You can't use the " << argument << " here." << std::endl;
            } else {
                std::cout << "You don't have a '" << argument << "'." << std::endl;
            }
        } else if (command == "talk") {
            bool foundNpc = false;
            for(auto& npc : npcs) {
                std::string lowerNpcName = npc->getName();
                std::transform(lowerNpcName.begin(), lowerNpcName.end(), lowerNpcName.begin(), ::tolower);
                if(npc->getCurrentRoom() == player->getCurrentRoom() && lowerNpcName == argument) {
                    npc->talk();
                    foundNpc = true;
                    break;
                }
            }
            if(!foundNpc) std::cout << "There is no one named '" << argument << "' to talk to." << std::endl;
        } else if (command == "attack") {
            auto enemyIt = std::find_if(enemies.begin(), enemies.end(),
                [&](const auto& enemy) {
                    std::string lowerEnemyName = enemy->getName();
                    std::transform(lowerEnemyName.begin(), lowerEnemyName.end(), lowerEnemyName.begin(), ::tolower);
                    return enemy->getCurrentRoom() == player->getCurrentRoom() && lowerEnemyName == argument;
                });

            if (enemyIt != enemies.end()) {
                if ((*enemyIt)->isDefeatedBy(*player)) {
                    std::cout << "You attack the " << (*enemyIt)->getName() << " and defeat it!" << std::endl;
                    enemies.erase(enemyIt); // Remove the defeated enemy from the game
                } else {
                    std::cout << "You attack the " << (*enemyIt)->getName() << " but have no way to defeat it! You have been slain." << std::endl;
                    isRunning = false; // Player loses
                }
            } else {
                std::cout << "There is no '" << argument << "' to attack here." << std::endl;
            }
        }
        else {
            std::cout << "I don't understand that command. Type 'help'." << std::endl;
        }
    }

public:
    Game() { setupWorld(); }

    void run() {
        view.displayWelcome();

        while (isRunning) {
            // FIX 3: The game loop now calls the upgraded displayRoom function.
            view.displayRoom(player->getCurrentRoom(), npcs, enemies);
            
            // The automatic combat check is now gone.

            std::cout << "\n> ";
            std::string inputLine;
            std::getline(std::cin, inputLine);
            processCommand(inputLine);

            if (!isRunning) { // Check if the game ended due to combat loss
                if (!checkWin()) {
                     std::cout << "\n--- GAME OVER ---" << std::endl;
                }
            }

            if (checkWin()) {
                std::cout << "\n\n********************************************************" << std::endl;
                std::cout << "The final door swings open, and you breathe free air!" << std::endl;
                std::cout << "Congratulations! You have escaped the dungeon!" << std::endl;
                std::cout << "********************************************************" << std::endl;
                isRunning = false;
            }
        }
        std::cout << "Thanks for playing!" << std::endl;
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}