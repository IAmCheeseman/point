#include "manager.h"

void init_item_manager(ItemManager* manager, size_t size) {
    manager->capacity = 8;
    manager->count = 0;
    manager->items = malloc(size * manager->capacity);
    for (int i = manager->count; i < manager->capacity; i++)
        manager->items[i] = NULL;
}

void item_manager_resize(ItemManager* manager, size_t size) {
    if (manager->count + 1 > manager->capacity) {
        manager->capacity *= 1.5;
        manager->items = realloc(manager->items, size * manager->capacity);

        for (int i = manager->count; i < manager->capacity; i++)
            manager->items[i] = NULL;
    }
}

int item_manager_set(ItemManager* manager, void* item) {
    for (int i = 0; i < manager->count + 1; i++) {
        if (manager->items[i] == NULL) {
            manager->items[i] = item;
            return i;
        }
    }

    return -1;
}

void* item_manager_get(ItemManager* manager, int index, const char* item_name) {
    if (index < 0 || index > manager->count || manager->items[index] == NULL) {
        fprintf(stderr, "Invalid %s index '%d'.\n", item_name, index);
        exit(1);
    }
    return manager->items[index];
}

bool item_manager_index_is_valid(ItemManager* manager, int index) {
    return index >= 0 && index <= manager->count && manager->items[index] != NULL;
}