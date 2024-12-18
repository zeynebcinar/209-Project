#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NUM_ROOMS 8
#define MAX_INVENTORY 5
#define FILENAME_LENGTH 100

typedef struct {
    char name[50];
    int value;
} Item;

typedef struct {
    char name[50];
    int health;
    int strength;
} Creature;

typedef struct Room {
    int id;
    char *description;
    Item *item;
    Creature *creature;
    bool trap;
    int trapDamage;
    struct Room *up;
    struct Room *down;
    struct Room *right;
    struct Room *left;
} Room;

typedef struct {
    int health;
    int strength;
    Room *currentRoom;
    Item inventory[MAX_INVENTORY];
    int inventoryCount;
} Player;

void initializeRooms(Room rooms[]);
void loadRoomData(Room rooms[], const char *filepath);
void displayMap(Room *currentRoom);
void movePlayer(Player *player, const char *direction);
void lookRoom(Player *player);
void attack(Player *player);
void displayInventory(Player *player);
void pickupItem(Player *player, const char *itemName);
void saveGame(Player *player, const char *filepath);
void loadGame(Player *player, Room rooms[], const char *filepath);
void listSavedGames();
void displayHelp();
void displayRoomContent(Room *room);
void displayGameStart(Room rooms[]);
void freeRooms(Room rooms[]);


const char *ITEM_NAMES[] = {
    "Scroll", "Sword", "Key", "Orb",
    "Potion", "Shield", "Gem", "Feather"
};

const char *CREATURE_NAMES[] = {
    "Dark Elf", "Stone Golem", "Fire Spirit", "Shadow Phantom",
    "Ice Wraith", "Venomous Serpent", "Undead Knight", "Abyssal Dragon"
};

void initializeRooms(Room rooms[]) {
    const char *roomDescriptions[] = {
        "A dusty library filled with cobwebs.",
        "A dimly lit armory with ancient weapons.",
        "A cold, damp dungeon cell with chains on the walls.",
        "A grand hall with flickering torchlights.",
        "A mysterious cave echoing strange sounds.",
        "A treasure room gleaming with gold and jewels.",
        "A dark forest clearing surrounded by tall trees.",
        "A wizard's chamber filled with magical artifacts."
    };

    for (int i = 0; i < NUM_ROOMS; i++) {
        rooms[i].id = i + 1;
        rooms[i].description = malloc(100 * sizeof(char));
        snprintf(rooms[i].description, 100, "%s", roomDescriptions[i]);

        rooms[i].item = malloc(sizeof(Item));
        snprintf(rooms[i].item->name, 50, "%s", ITEM_NAMES[i]);
        rooms[i].item->value = 10 + i * 5;

        rooms[i].creature = malloc(sizeof(Creature));
        snprintf(rooms[i].creature->name, 50, "%s", CREATURE_NAMES[i]);
        rooms[i].creature->health = 20 + i * 10;
        rooms[i].creature->strength = 5 + i * 2;

        rooms[i].trap = (rand() % 2 == 0);  
        rooms[i].trapDamage = rooms[i].trap ? (5 + rand() % 6) : 0; 

        rooms[i].up = rooms[i].down = rooms[i].right = rooms[i].left = NULL;
    }

    // Connect rooms
    rooms[0].right = &rooms[1];
    rooms[1].left = &rooms[0]; rooms[1].right = &rooms[2]; rooms[1].down = &rooms[3];
    rooms[2].left = &rooms[1]; rooms[2].down = &rooms[4];
    rooms[3].right = &rooms[4]; rooms[3].up = &rooms[1];
    rooms[4].up = &rooms[2]; rooms[4].left = &rooms[3]; rooms[4].right = &rooms[5]; rooms[4].down = &rooms[6];
    rooms[5].left = &rooms[4]; rooms[5].down = &rooms[7];
    rooms[6].up = &rooms[4]; rooms[6].right = &rooms[7];
    rooms[7].left = &rooms[6]; rooms[7].up = &rooms[5];
}

void displayRoomContent(Room *room) {
    printf("Current Room: %s\n", room->description);
    if (room->item) {
        printf("Item: %s (Value: %d)\n", room->item->name, room->item->value);
    } else {
        printf("No item in this room.\n");
    }
    if (room->creature) {
        printf("Creature: %s (Health: %d, Strength: %d)\n", room->creature->name, room->creature->health, room->creature->strength);
    } else {
        printf("No creature in this room.\n");
    }
}

void displayGameStart(Room rooms[]) {
    printf("Welcome to the Dungeon Game!\n");
    for (int i = 0; i < NUM_ROOMS; i++) {
        printf("Room %d contains: %s, Creature: %s (Health: %d)\n",
               rooms[i].id, rooms[i].item->name, rooms[i].creature->name, rooms[i].creature->health);
    }
    printf("\nUse 'help' to see available commands.\n\n");
}

#include <ctype.h>

void pickupItem(Player *player, const char *itemName) {
    if (player->currentRoom->item && strcasecmp(player->currentRoom->item->name, itemName) == 0) {
        if (player->inventoryCount < MAX_INVENTORY) {
            player->inventory[player->inventoryCount++] = *(player->currentRoom->item);
            player->strength += 5;
            printf("You picked up: %s (Strength +5)\n", player->currentRoom->item->name);
            printf("Your current strength: %d\n", player->strength);            
            free(player->currentRoom->item);
            player->currentRoom->item = NULL;
        } else {
            printf("Your inventory is full!\n");
        }
    } else {
        printf("No such item here.\n");
    }
}


void dropItem(Player *player, const char *itemName) {
    for (int i = 0; i < player->inventoryCount; i++) {
        if (strcmp(player->inventory[i].name, itemName) == 0) {
            player->currentRoom->item = malloc(sizeof(Item));
            *player->currentRoom->item = player->inventory[i];
            printf("You dropped: %s\n", itemName);

            player->strength -= 5;
            printf("Your current strength: %d\n", player->strength);

            for (int j = i; j < player->inventoryCount - 1; j++) {
                player->inventory[j] = player->inventory[j + 1];
            }
            player->inventoryCount--;
            return;
        }
    }
    printf("You don't have that item.\n");
}


void attack(Player *player) {
    if (player->currentRoom->creature) {
        Creature *creature = player->currentRoom->creature;
        printf("You attack the %s!\n", creature->name);
        creature->health -= player->strength;

        if (creature->health <= 0) {
            printf("You defeated the %s!\n", creature->name);
            free(player->currentRoom->creature);
            player->currentRoom->creature = NULL;
        } else {
            printf("The %s attacks you back!\n", creature->name);
            player->health -= creature->strength;

            if (player->health <= 0) {
                printf("You have been defeated! Game Over.\n");
                printf("Restarting the game...\n\n");
                exit(0);
            } else {
                printf("Your health: %d, Your strength: %d\n", player->health, player->strength);
                printf("%s's health: %d\n", creature->name, creature->health);
            }
        }
    } else {
        printf("There's no creature here to attack.\n");
    }
}

void displayInventory(Player *player) {
    printf("Inventory:\n");
    for (int i = 0; i < player->inventoryCount; i++) {
        printf("%d. %s (Value: %d)\n", i + 1, player->inventory[i].name, player->inventory[i].value);
    }
    if (player->inventoryCount == 0) {
        printf("Your inventory is empty.\n");
    }
}

void displayMap(Room *currentRoom) {
    printf("Map:\n");
    printf("Room 1 - Room 2 - Room 3\n");
    printf("            |       |      \n");
    printf("         Room 4 - Room 5 - Room 6\n");
    printf("                    |        |      \n");
    printf("                  Room 7 - Room 8\n");
}

void movePlayer(Player *player, const char *direction) {
    Room *nextRoom = NULL;
    if (strcmp(direction, "up") == 0) nextRoom = player->currentRoom->up;
    else if (strcmp(direction, "down") == 0) nextRoom = player->currentRoom->down;
    else if (strcmp(direction, "right") == 0) nextRoom = player->currentRoom->right;
    else if (strcmp(direction, "left") == 0) nextRoom = player->currentRoom->left;

    if (nextRoom) {
        player->currentRoom = nextRoom;
        printf("You moved to Room %d: %s\n", player->currentRoom->id, player->currentRoom->description);
        if (player->currentRoom->trap) {
            player->health -= player->currentRoom->trapDamage;
            printf("Oh no! There was a trap here. You lost %d health. New health: %d\n",
                   player->currentRoom->trapDamage, player->health);

            if (player->health <= 0) {
                printf("You fell victim to the trap. Game Over!\n");
                exit(0);
            }
        }
    } else {
        printf("You can't move in that direction.\n");
    }
}

void lookRoom(Player *player) {
    printf("You are in Room %d: %s\n", player->currentRoom->id, player->currentRoom->description);

    if (player->currentRoom->item) {
        printf("Item: %s (Value: %d)\n", player->currentRoom->item->name, player->currentRoom->item->value);
    } else {
        printf("No items in this room.\n");
    }
    if (player->currentRoom->creature) {
        printf("Creature: %s (Health: %d, Strength: %d)\n",
               player->currentRoom->creature->name, player->currentRoom->creature->health, player->currentRoom->creature->strength);
    } else {
        printf("No creatures in this room.\n");
    }
}

void displayHelp() {
    printf("Available Commands:\n");
    printf("map: Display the dungeon map.\n");
    printf("look: Display the current room description.\n");
    printf("up/down/left/right: Move to a connected room.\n");
    printf("attack: Attack a creature in the room.\n");
    printf("inventory: Show your inventory.\n");
    printf("pickup <item_name>: Pick up a specific item.\n");
    printf("drop <item_name>: Drop a specific item in the room.\n");
    printf("exit: Exit the game.\n");
    printf("save <filepath>: Save the current game state.\n");
    printf("load <filepath>: Load a saved game state.\n");
    printf("list: List all saved games.\n");
    printf("help: Available Commands:\n");

}

void freeRooms(Room rooms[]) {
    for (int i = 0; i < NUM_ROOMS; i++) {
        free(rooms[i].description);
        if (rooms[i].item) free(rooms[i].item);
        if (rooms[i].creature) free(rooms[i].creature);
    }
}
void saveGame(Player *player, const char *filepath) {
    FILE *file = fopen(filepath, "w");
    if (!file) {
        printf("Error: Unable to save the game.\n");
        return;
    }

    fprintf(file, "%d %d %d\n", player->health, player->strength, player->currentRoom->id);
    for (int i = 0; i < player->inventoryCount; i++) {
        fprintf(file, "%s %d\n", player->inventory[i].name, player->inventory[i].value);
    }
    fclose(file);
    printf("Game saved successfully to %s.\n", filepath);
}
void loadGame(Player *player, Room rooms[], const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (!file) {
        printf("Error: Unable to load the game.\n");
        return;
    }

    int roomId;
    fscanf(file, "%d %d %d", &player->health, &player->strength, &roomId);
    player->currentRoom = &rooms[roomId - 1];
    player->inventoryCount = 0;

    while (fscanf(file, "%s %d", player->inventory[player->inventoryCount].name, 
                  &player->inventory[player->inventoryCount].value) != EOF) {
        player->inventoryCount++;
    }
    fclose(file);
    printf("Game loaded successfully from %s.\n", filepath);
}
void listSavedGames() {
    printf("Saved games:\n");
    system("ls *.sav");
}

int main() {
    char playerName[50];
    printf("Welcome to the Dungeon Game!\n");

    printf("Enter your player name: ");
    scanf("%49s", playerName);

    printf("Welcome, %s! Let the adventure begin...\n", playerName);

    Room rooms[NUM_ROOMS];
    Player player = {100, 10, NULL, {{0}}, 0}; 

    initializeRooms(rooms);
    player.currentRoom = &rooms[0]; 

    displayMap(NULL);
    displayHelp(); 

    char command[50];
    while (1) {
        printf("> ");
        scanf("%s", command);

        if (strcmp(command, "map") == 0) displayMap(player.currentRoom);
        else if (strcmp(command, "look") == 0) lookRoom(&player);
        else if (strcmp(command, "up") == 0 || strcmp(command, "down") == 0 ||
                 strcmp(command, "right") == 0 || strcmp(command, "left") == 0)
            movePlayer(&player, command);
        else if (strcmp(command, "attack") == 0) attack(&player);
        else if (strcmp(command, "pickup") == 0) {
            char itemName[50];
            scanf("%49s", itemName);
            pickupItem(&player, itemName);
        } else if (strcmp(command, "drop") == 0) {
            char itemName[50];
            scanf("%49s", itemName);
            dropItem(&player, itemName);
        } else if (strcmp(command, "inventory") == 0) displayInventory(&player);
        else if (strcmp(command, "help") == 0) displayHelp();
        else if (strcmp(command, "save") == 0) {
            char filepath[FILENAME_LENGTH];
            scanf("%s", filepath);
            saveGame(&player, filepath);
        } else if (strcmp(command, "load") == 0) {
            char filepath[FILENAME_LENGTH];
            scanf("%s", filepath);
            loadGame(&player, rooms, filepath);
        } else if (strcmp(command, "list") == 0) {
            listSavedGames();
        } else if (strcmp(command, "exit") == 0) break;
        else printf("Unknown command. Type 'help' for a list of commands.\n");
    }
    freeRooms(rooms);
    printf("Thanks for playing!\n");
    return 0;
}
