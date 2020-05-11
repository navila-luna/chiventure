#include <stdbool.h>
#include <string.h>
#include "structs.h"
#ifndef PLAYERSTATS_H
#define PLAYERSTATS_H

/* Creates a new item and initializes its parameters */
item_t *create_item(int id, int quan, int durab, char* descrip, bool bat, int attack, int defense, int hp);

/* Adds an item to a player's inventory */
ilist_t *add_item_to_inventory(ilist_t *inv, item_t *item);

/* Adds an item to a player's armor list */
alist_t *add_armor(alist_t *arm, armor_t *armor);

/* Creates new armor and initializes its parameters */
armor_t *create_armor(char* descrip, int defense, int weight);

/** Make empty inventory list **/
ilist_t *create_test_inventory();

/** Make empty armor list **/
alist_t *create_test_armor();

#endif