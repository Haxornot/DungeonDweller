GAMEPLAY
Dungeon Dweller is a standard text-based game that involves being stuck in a dungeon

There are 9 rooms that you have to navigate through. Each room will contain an item, monster, both or none
To win you have to defeat 5 monsters

Code
There are 3 main files in this
main.c is where the main code functions are located
main.h is where the code definitions and structs are located
room.txt This file contains the room descriptions,items and monsters from which the main file reads from

Game logic
This a turn-based game where a player can perform one command at a time
For example when you use the command attack on a monster it will be the monster turn next to attack before youre able to use another command
you will die if your health deplets to 0 or below, and likewise for monsters

