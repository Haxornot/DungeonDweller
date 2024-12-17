#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

// Main 
int main() {
    Room rooms[ROOM_COUNT] = {0};
    Player player = {100, 10, 0, 0, 1}; 
    loadRooms(rooms, "rooms.txt");

    printf("\n=== Welcome to Dungeon Dweller ===\n");
    printf("\nYou're currently trapped in a dungeon that consists of 9 rooms. "
           "To escape, you must defeat all 5 villians. "
           "There will be different items and weapons throughout the dungeon that will help you on your journey.\n");
    printf("\nBelow are the available commands along with their explanation:\n\n");

    printf(" - stats       : View your current health, strength, and defeated villians.\n");
    printf(" - inventory   : Check the items in your inventory.\n");
    printf(" - pickup      : Pick up an item in the current room (if available).\n");
    printf(" - attack      : Attack the villian in the current room.\n");
    printf(" - move <dirextion>  : Move in a direction (e.g., 'move up', 'move down', 'move left', 'move right').\n");
    printf(" - look        : Inspect the current room for details, including villians and items.\n");
    printf(" - save        : Save your game progress to a file.\n");
    printf(" - load        : Load a previously saved game.\n");
    printf(" - exit        : Quit the game.\n");

    printf("\nYou start at Room 1. Good luck, adventurer!\n");

    
    printf("Commands: stats, inventory, pickup, attack, move <dir>, look, save, load, exit.\n");

    gameLoop(&player, rooms);
    return 0;
}

// reading room from file
void loadRooms(Room rooms[], const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) exit(printf("Error: Cannot open %s\n", filename));

    int id, up, down, left, right;
    while (fscanf(file, "%d\n", &id) != EOF) {
        rooms[id].id = id;
        fgets(rooms[id].description, sizeof(rooms[id].description), file);
        rooms[id].description[strcspn(rooms[id].description, "\n")] = 0;

        fscanf(file, "%49[^\n]\n", rooms[id].item);
        fscanf(file, "%49[^;];%d;%d\n", rooms[id].enemy, &rooms[id].enemyHealth, &rooms[id].enemyDamage);
        fscanf(file, "%d %d %d %d\n", &up, &down, &left, &right);

        rooms[id].up = (up != -1) ? &rooms[up] : NULL;
        rooms[id].down = (down != -1) ? &rooms[down] : NULL;
        rooms[id].left = (left != -1) ? &rooms[left] : NULL;
        rooms[id].right = (right != -1) ? &rooms[right] : NULL;
    }
    fclose(file);
}

// Main game loop
void gameLoop(Player *player, Room rooms[]) {
    char command[50];
    Room *currentRoom = &rooms[player->currentRoomID];

    while (1) {
        displayRoom(currentRoom);
        printf("\n> ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;
        handleCommand(command, player, &currentRoom, rooms);
    }
}


void handleCommand(char *command, Player *player, Room **currentRoom, Room rooms[]) {
    char direction[10];
    if (strcmp(command, "stats") == 0)
        printf("\nHealth: %d | Strength: %d | villians Defeated: %d\n", player->health, player->strength, player->villiansDefeated);
    else if (strcmp(command, "inventory") == 0) {
        if (player->inventoryCount == 0)
            printf("\nYour inventory is empty.\n");
        else {
            printf("\nYour inventory contains:\n");
            for (int i = 0; i < player->inventoryCount; i++) {
                printf(" - %s\n", player->inventory[i]);
            }
        }
    } else if (strcmp(command, "pickup") == 0)
        applyItemEffect(player, (*currentRoom)->item), strcpy((*currentRoom)->item, "None");
    else if (strcmp(command, "attack") == 0) {
        if ((*currentRoom)->enemyHealth > 0) {
            (*currentRoom)->enemyHealth -= player->strength;
            printf("You attack the %s! Enemy health: %d\n", (*currentRoom)->enemy, (*currentRoom)->enemyHealth);
            if ((*currentRoom)->enemyHealth <= 0) {
                printf("You defeated the %s!\n", (*currentRoom)->enemy);
                (*currentRoom)->enemyHealth = 0;
                player->villiansDefeated++;
            } else player->health -= (*currentRoom)->enemyDamage;
            if (player->health <= 0) exit(printf("Game over! You were defeated.\n"));
            if (player->villiansDefeated == TOTAL_villianS) exit(printf("Congratulations You Escaped the dungeon\n"));
        } else printf("No enemy here.\n");
    } else if (strcmp(command, "look") == 0) {
        printf("\n%s\n", (*currentRoom)->description);
        if (strcmp((*currentRoom)->item, "None") != 0)
            printf("You see a %s here.\n", (*currentRoom)->item);
        if ((*currentRoom)->enemyHealth > 0)
            printf("An enemy is here: %s (Health: %d)\n", (*currentRoom)->enemy, (*currentRoom)->enemyHealth);
        else if (strcmp((*currentRoom)->enemy, "None") != 0)
            printf("The defeated %s lies here.\n", (*currentRoom)->enemy);
    } else if (strcmp(command, "save") == 0) saveGame(player, rooms);
    else if (strcmp(command, "load") == 0) loadGame(player, rooms), *currentRoom = &rooms[player->currentRoomID];
    else if (sscanf(command, "move %s", direction) == 1) movePlayer(direction, currentRoom), player->currentRoomID = (*currentRoom)->id;
    else if (strcmp(command, "exit") == 0) exit(printf("Goodbye!\n"));
    else printf("Invalid command.\n");
}

// itemeffexts
void applyItemEffect(Player *player, const char *item) {
    if (strcmp(item, "None") == 0 || strlen(item) == 0) {
        printf("No items to u up.\n");
    } else {
        if (player->inventoryCount < INVENTORY_SIZE) {
            strcpy(player->inventory[player->inventoryCount], item);
            player->inventoryCount++;
            printf("You picked up the %s!\n", item);

            // Apply effects
            if (strcmp(item, "Wooden Sword") == 0) player->strength += 5;
            else if (strcmp(item, "Inverted Spear of Heaven") == 0) player->strength += 15;
            else if (strcmp(item, "Shield") == 0) player->health += 10;
            else if (strcmp(item, "Health Potion") == 0) player->health += 20;
        } else {
            printf("Your inventory is full!\n");
        }
    }
}

// movement
void movePlayer(const char *direction, Room **currentRoom) {
    Room *nextRoom = NULL;
    if (strcmp(direction, "up") == 0) nextRoom = (*currentRoom)->up;
    else if (strcmp(direction, "down") == 0) nextRoom = (*currentRoom)->down;
    else if (strcmp(direction, "left") == 0) nextRoom = (*currentRoom)->left;
    else if (strcmp(direction, "right") == 0) nextRoom = (*currentRoom)->right;

    if (nextRoom) *currentRoom = nextRoom, printf("You moved to Room %d.\n", (*currentRoom)->id);
    else printf("You can't go that way!\n");
}

void displayRoom(Room *room) {
    printf("\nRoom %d\n", room->id);
}

// Save game
void saveGame(Player *player, Room rooms[]) {
    FILE *file = fopen("savegame.txt", "w");
    if (!file) {
        printf("Error: Could not save the game.\n");
        return;
    }

    fprintf(file, "%d %d %d %d %d\n", 
            player->health, player->strength, player->inventoryCount, 
            player->villiansDefeated, player->currentRoomID);

    // Save inv
    for (int i = 0; i < player->inventoryCount; i++) {
        fprintf(file, "%s\n", player->inventory[i]);
    }

    fclose(file);
    printf("Game successfully saved.\n");
}

// load
void loadGame(Player *player, Room rooms[]) {
    FILE *file = fopen("savegame.txt", "r");
    if (!file) {
        printf("Error: No saved game found.\n");
        return;
    }

    fscanf(file, "%d %d %d %d %d", 
           &player->health, &player->strength, &player->inventoryCount, 
           &player->villiansDefeated, &player->currentRoomID);

    
    for (int i = 0; i < player->inventoryCount; i++) {
        fscanf(file, "%49[^\n]\n", player->inventory[i]);
    }

    fclose(file);
    printf("Game successfully loaded.\n");
}
