#ifndef _point_manager_h
#define _point_manager_h

#include "common.h"

typedef struct {
    int capacity;
    int count;
    void** items;
} ItemManager;

#define ITEM_MANAGER_RESIZE(manager, type) (item_manager_resize(manager, sizeof(type*)))
#define ITEM_MANAGER_FREE(free_func, type) \
    do { \
        for (int i = 0; i < manager->capacity; i++) { \
            if (manager->items[i] == NULL) \
                continue; \
            free_func((type*)manager->items[i]); \
            manager->items[i] = NULL; \
        } \
        manager->capacity = 0; \
        manager->count = 0; \
        free(manager->items); \
        manager->items = NULL; \
    } while (false)

void item_manager_resize(ItemManager* manager, size_t size);
int item_manager_set(ItemManager* manager, void* item);
void* item_manager_get(ItemManager* manager, int index, const char* item_name);
bool item_manager_index_is_valid(ItemManager* manager, int index);

#endif