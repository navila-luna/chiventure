#include <stdlib.h>

#include "common-item.h"
#include "common-room.h"
#include "common-path.h"

/* See room.h */
int room_init(room_t *new_room, char *room_id, char *short_desc,
              char *long_desc)
{
    assert(new_room != NULL);

    strncpy(new_room->room_id, room_id, strlen(room_id)+1);
    strncpy(new_room->short_desc, short_desc, strlen(short_desc)+1);
    strncpy(new_room->long_desc, long_desc, strlen(long_desc)+1);

    return SUCCESS;
}

room_t *room_new(char *room_id, char *short_desc, char *long_desc)
{

    room_t *room = malloc(sizeof(room_t));
    memset(room, 0, sizeof(room_t));
    room->room_id = malloc(MAX_ID_LEN);
    room->short_desc = malloc(MAX_SDESC_LEN);
    room->long_desc = malloc(MAX_LDESC_LEN);
    int check = room_init(room, room_id, short_desc, long_desc);

    if (room == NULL || room->room_id == NULL ||
            room->short_desc == NULL || room->long_desc == NULL)
    {
        return NULL;
    }

    if(check != SUCCESS)
    {
        return NULL;
    }

    return room;
}



/* See room.h */
int room_free(room_t *room)
{
    free(room->room_id);
    free(room->short_desc);
    free(room->long_desc);
    delete_all_paths(room->paths);
    delete_all_items(&room->items);
    free(room);
    return SUCCESS;
}


/* See room.h */
int add_item_to_room(room_t *room, item_t *item)
{
    item_t* check;
    HASH_FIND(hh, room->items, item->item_id, strlen(item->item_id), check);

    if (check != NULL)
    {
        return FAILURE; //this item id is already in use.
    }
    HASH_ADD_KEYPTR(hh, room->items, item->item_id, strlen(item->item_id),
                    item);
    return SUCCESS;

}

/* See room.h */
int add_path_to_room(room_t *room, path_t *path)
{
    path_t *s;

    if (room == NULL)
    {
        return FAILURE; //cannot add path to NULL room
    }

    if (path == NULL)
    {
        return FAILURE; //cannot add NULL path to room
    }

    HASH_FIND(hh, room->paths, path->direction, strlen(path->direction), s);

    if (s != NULL)
    {
        return FAILURE; //direction already used
    }

    HASH_ADD_KEYPTR(hh, room->paths, path->direction, strlen(path->direction),
                    path);
    return SUCCESS;
}

/* See common-room.h */
int delete_all_rooms(room_hash_t *rooms)
{
    room_t *current_room, *tmp;
    HASH_ITER(hh, rooms, current_room, tmp)
    {
        HASH_DEL(rooms, current_room);  /* deletes (rooms advances to next) */
        room_free(current_room);             /* free it */
    }
    return SUCCESS;
}

/* See room.h */
path_t *path_search(room_t *room, char* direction)
{
    path_t *path;
    if (room == NULL)
    {
        return NULL; //cannot search path in NULL room
    }

    HASH_FIND(hh, room->paths, direction, strlen(direction), path);
    return path;
}

/* See room.h */
char *get_sdesc(room_t *room)
{
    return room->short_desc;
}

/* See room.h */
char *get_ldesc(room_t *room)
{
    return room->long_desc;
}

/* Get list (implemented with hashtable) of items in room
 *
 * Parameters:
 *  pointer to room
 *
 * Returns:
 *  hashtable of items in room
 */
item_hash_t* list_items(room_t *room)
{
    return room->items;
}

/* Get list of paths from room
 *
 * Parameters:
 *  pointer to room
 *
 * Returns:
 *  pointer to hashtable of paths from room
 */
path_t *list_paths(room_t *room)
{
    return room->paths;
}

/* FOR CLI
* Implement a function that returns an item
* given an item_ID as string and a pointer to the current room.
*/

/* see room.h */
item_t* get_item_in_room(room_t* room, char* item_id)
{
    item_t* return_value;
    HASH_FIND(hh, room->items, item_id, strlen(item_id), return_value);
    return return_value;
    //if it is NULL, return_value will be equal to NULL by default
}


/* See room.h */
room_t *find_room_from_path(path_t *path)
{
    if(path != NULL)
    {
        return path->dest;
    }
    return NULL;
}

/* See room.h */
room_t *find_room_from_dir(room_t *curr, char* direction)
{
    path_t *path = path_search(curr, direction);
    room_t *room_adj = find_room_from_path(path);
    return room_adj;
}

/* See room.h */
item_list_t *get_all_items_in_room(room_t *room)
{
    item_list_t *head = NULL;
    item_t *ITTMP_ITEMRM, *curr_item;
    item_list_t *tmp;
    HASH_ITER(hh, room->items, curr_item, ITTMP_ITEMRM)
    {
        tmp = malloc(sizeof(item_list_t));
        tmp->item = curr_item;
        LL_APPEND(head, tmp);
    }
    return head;
}
