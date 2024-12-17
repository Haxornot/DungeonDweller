#ifndef MAIN_H
#define MAIN_H

// Constants
#define INVENTORY_SIZE 10
#define ROOM_COUNT 10
#define TOTAL_villianS 5

// Structures
typedef struct Room {
    int id;
    char description[200], item[50], enemy[50];
    int enemyHealth, enemyDamage;
    struct Room *up, *down, *left, *right;
} Room;

typedef struct {
    int health, strength, inventoryCount, villiansDefeated, currentRoomID;
    char inventory[INVENTORY_SIZE][50];
} Player;

// Function Prototypes
void loadRooms(Room rooms[], const char *filename);
void gameLoop(Player *player, Room rooms[]);
void displayRoom(Room *room);
void movePlayer(const char *direction, Room **currentRoom);
void handleCommand(char *command, Player *player, Room **currentRoom, Room rooms[]);
void applyItemEffect(Player *player, const char *item);
void saveGame(Player *player, Room rooms[]);
void loadGame(Player *player, Room rooms[]);

#endif // MAIN_H
