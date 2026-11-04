
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 4

typedef enum {
    FIRST_FIT = 1,
    BEST_FIT = 2,
    WORST_FIT = 3
} Strategy;

typedef enum {
    FREE = 0,
    ALLOCATED = 1
} Status;

typedef struct {
    size_t start;
    size_t size;
    unsigned int id;
    Status status;
} Block;

typedef struct {
    unsigned char *buffer;
    Block *blocks;
    size_t totalSize;
    size_t blockCount;
    size_t capacity;
    unsigned int nextId;
    Strategy strategy;
} MemoryManager;

int READ_INTEGER(char message[]);
int ENSURE_CAPACITY(MemoryManager *manager);
int INSERT_BLOCK(MemoryManager *manager, size_t index, Block block);
void REMOVE_BLOCK(MemoryManager *manager, size_t index);
void MERGE_BLOCKS(MemoryManager *manager);
int INITIALIZE(MemoryManager *manager, size_t totalSize, Strategy strategy);
int FIND_ALLOCATION(MemoryManager *manager, unsigned int id);
int FIND_BLOCK(MemoryManager *manager, size_t size);
int ALLOCATE_MEMORY(MemoryManager *manager, size_t size, unsigned int *id);
int RELEASE_MEMORY(MemoryManager *manager, unsigned int id);
int RESIZE_MEMORY(MemoryManager *manager, unsigned int id, size_t newSize);
void DISPLAY_MEMORY_MAP(MemoryManager *manager);
void DISPLAY_STATISTICS(MemoryManager *manager);
void MENU(void);
void ALLOCATE_OPERATION(MemoryManager *manager);
void RELEASE_OPERATION(MemoryManager *manager);
void RESIZE_OPERATION(MemoryManager *manager);
void CHANGE_STRATEGY(MemoryManager *manager);
void DESTROY(MemoryManager *manager);



int READ_INTEGER(char message[]) {
    int number;

    printf("%s", message);

    if (scanf("%d", &number) != 1) {
        printf("Invalid input.\n");

        while (getchar() != '\n');

        return -1;
    }

    return number;
}



int ENSURE_CAPACITY(MemoryManager *manager) {

    if (manager->blockCount < manager->capacity)
        return 1;

    size_t newCapacity;

    if (manager->capacity == 0)
        newCapacity = INITIAL_CAPACITY;
    else
        newCapacity = manager->capacity * 2;

    if (newCapacity < manager->capacity ||
        newCapacity > (size_t)-1 / sizeof(Block))
        return 0;

    Block *temp = realloc(
        manager->blocks,
        newCapacity * sizeof(Block)
    );

    if (temp == NULL)
        return 0;

    manager->blocks = temp;
    manager->capacity = newCapacity;

    return 1;
}

int INSERT_BLOCK(MemoryManager *manager, size_t index, Block block) {

    if (index > manager->blockCount || !ENSURE_CAPACITY(manager))
        return 0;

    memmove(
        &manager->blocks[index + 1],
        &manager->blocks[index],
        (manager->blockCount - index) * sizeof(Block)
    );

    manager->blocks[index] = block;
    manager->blockCount++;

    return 1;
}

void REMOVE_BLOCK(MemoryManager *manager, size_t index) {

    if (index >= manager->blockCount)
        return;

    memmove(
        &manager->blocks[index],
        &manager->blocks[index + 1],
        (manager->blockCount - index - 1) * sizeof(Block)
    );

    manager->blockCount--;
}

void MERGE_BLOCKS(MemoryManager *manager) {

    size_t i = 0;

    while (i + 1 < manager->blockCount) {

        Block *current = &manager->blocks[i];
        Block *next = &manager->blocks[i + 1];

        if (current->status == FREE && next->status == FREE) {

            current->size += next->size;
            REMOVE_BLOCK(manager, i + 1);

        } else {
            i++;
        }
    }
}


int INITIALIZE(
    MemoryManager *manager,
    size_t totalSize,
    Strategy strategy
) {

    manager->buffer = calloc(totalSize, sizeof(unsigned char));

    if (manager->buffer == NULL)
        return 0;

    manager->blocks = malloc(INITIAL_CAPACITY * sizeof(Block));

    if (manager->blocks == NULL) {

        free(manager->buffer);
        manager->buffer = NULL;

        return 0;
    }

    manager->totalSize = totalSize;
    manager->capacity = INITIAL_CAPACITY;
    manager->blockCount = 1;
    manager->nextId = 1;
    manager->strategy = strategy;

    manager->blocks[0] = (Block){0, totalSize, 0, FREE};

    return 1;
}



int FIND_ALLOCATION(MemoryManager *manager, unsigned int id) {

    for (size_t i = 0; i < manager->blockCount; i++) {

        if (manager->blocks[i].status == ALLOCATED &&
            manager->blocks[i].id == id)
            return (int)i;
    }

    return -1;
}

int FIND_BLOCK(MemoryManager *manager, size_t size) {

    int selected = -1;

    for (size_t i = 0; i < manager->blockCount; i++) {

        Block *block = &manager->blocks[i];

        if (block->status != FREE || block->size < size)
            continue;

        if (manager->strategy == FIRST_FIT)
            return (int)i;

        if (selected == -1) {
            selected = (int)i;
            continue;
        }

        if (manager->strategy == BEST_FIT &&
            block->size < manager->blocks[selected].size)
            selected = (int)i;

        if (manager->strategy == WORST_FIT &&
            block->size > manager->blocks[selected].size)
            selected = (int)i;
    }

    return selected;
}



int ALLOCATE_MEMORY(
    MemoryManager *manager,
    size_t size,
    unsigned int *id
) {

    if (size == 0 || size > manager->totalSize ||
        manager->nextId == 0)
        return 0;

    int index = FIND_BLOCK(manager, size);

    if (index == -1)
        return 0;

    unsigned int newId = manager->nextId;

    Block *block = &manager->blocks[index];

    if (block->size == size) {

        block->status = ALLOCATED;
        block->id = newId;

    } else {

        Block newBlock = {
            block->start,
            size,
            newId,
            ALLOCATED
        };

        if (!INSERT_BLOCK(manager, index, newBlock))
            return 0;

        manager->blocks[index + 1].start += size;
        manager->blocks[index + 1].size -= size;
    }

    if (manager->nextId == (unsigned int)-1)
        manager->nextId = 0;
    else
        manager->nextId++;

    *id = newId;

    return 1;
}

int RELEASE_MEMORY(MemoryManager *manager, unsigned int id) {

    int index = FIND_ALLOCATION(manager, id);

    if (index == -1)
        return 0;

    manager->blocks[index].status = FREE;
    manager->blocks[index].id = 0;

    MERGE_BLOCKS(manager);

    return 1;
}



int RESIZE_MEMORY(
    MemoryManager *manager,
    unsigned int id,
    size_t newSize
) {

    int index = FIND_ALLOCATION(manager, id);

    if (index == -1 || newSize == 0 ||
        newSize > manager->totalSize)
        return 0;

    size_t oldSize = manager->blocks[index].size;
    size_t oldStart = manager->blocks[index].start;

    if (newSize == oldSize)
        return 1;

    

    if (newSize < oldSize) {

        Block freeBlock = {
            oldStart + newSize,
            oldSize - newSize,
            0,
            FREE
        };

        if (!INSERT_BLOCK(manager, index + 1, freeBlock))
            return 0;

        manager->blocks[index].size = newSize;

        MERGE_BLOCKS(manager);

        return 1;
    }

    

    size_t extraSize = newSize - oldSize;

    if ((size_t)index + 1 < manager->blockCount) {

        Block *next = &manager->blocks[index + 1];

        if (next->status == FREE && next->size >= extraSize) {

            manager->blocks[index].size = newSize;

            next->start += extraSize;
            next->size -= extraSize;

            if (next->size == 0)
                REMOVE_BLOCK(manager, index + 1);

            return 1;
        }
    }

    

    unsigned int newId;

    if (!ALLOCATE_MEMORY(manager, newSize, &newId))
        return 0;

    int newIndex = FIND_ALLOCATION(manager, newId);

    if (newIndex == -1)
        return 0;

    memcpy(
        manager->buffer + manager->blocks[newIndex].start,
        manager->buffer + oldStart,
        oldSize
    );

    if (!RELEASE_MEMORY(manager, id))
        return 0;

    newIndex = FIND_ALLOCATION(manager, newId);

    if (newIndex == -1)
        return 0;

    manager->blocks[newIndex].id = id;

    return 1;
}



void DISPLAY_MEMORY_MAP(MemoryManager *manager) {

    printf("\n--------- MEMORY MAP ---------\n");

    for (size_t i = 0; i < manager->blockCount; i++) {

        Block block = manager->blocks[i];

        printf("\nBlock        : %zu\n", i + 1);
        printf("Start        : %zu\n", block.start);
        printf("Size         : %zu\n", block.size);

        if (block.status == ALLOCATED) {

            printf("Status       : ALLOCATED\n");
            printf("Allocation ID: %u\n", block.id);

        } else {

            printf("Status       : FREE\n");
        }
    }

    
}


void DISPLAY_STATISTICS(MemoryManager *manager) {

    size_t used = 0;
    size_t freeMemory = 0;
    size_t freeBlocks = 0;
    size_t allocatedBlocks = 0;
    size_t largestFree = 0;

    for (size_t i = 0; i < manager->blockCount; i++) {

        Block block = manager->blocks[i];

        if (block.status == ALLOCATED) {

            used += block.size;
            allocatedBlocks++;

        } else {

            freeMemory += block.size;
            freeBlocks++;

            if (block.size > largestFree)
                largestFree = block.size;
        }
    }

    printf("\n------ MEMORY STATISTICS ------\n");

    printf("Total Memory      : %zu\n", manager->totalSize);
    printf("Used Memory       : %zu\n", used);
    printf("Free Memory       : %zu\n", freeMemory);
    printf("Allocated Blocks  : %zu\n", allocatedBlocks);
    printf("Free Blocks       : %zu\n", freeBlocks);
    printf("Largest Free Block: %zu\n", largestFree);
}


void MENU(void) {

    printf("\nMenu:\n");
    printf("1. Allocate memory\n");
    printf("2. Release memory\n");
    printf("3. Resize allocation\n");
    printf("4. Display memory map\n");
    printf("5. Display memory statistics\n");
    printf("6. Change allocation strategy\n");
    printf("7. Exit\n");
}


void ALLOCATE_OPERATION(MemoryManager *manager) {

    int size = READ_INTEGER("Requested size: ");

    unsigned int id;

    if (size > 0 &&
        ALLOCATE_MEMORY(manager, (size_t)size, &id)) {

        int index = FIND_ALLOCATION(manager, id);

        printf(
            "Allocation successful. ID: %u, Start: %zu, Size: %zu\n",
            id,
            manager->blocks[index].start,
            manager->blocks[index].size
        );

    } else {

        printf("Allocation failed.\n");
    }
}


void RELEASE_OPERATION(MemoryManager *manager) {

    int id = READ_INTEGER("Allocation ID: ");

    if (id > 0 && RELEASE_MEMORY(manager, (unsigned int)id)) {

        printf("Allocation released.\n");

    } else {

        printf("Invalid allocation ID.\n");
    }
}



void RESIZE_OPERATION(MemoryManager *manager) {

    int id = READ_INTEGER("Allocation ID: ");
    int size = READ_INTEGER("Requested new size: ");

    if (id > 0 && size > 0 &&
        RESIZE_MEMORY(manager, (unsigned int)id, (size_t)size)) {

        printf("Allocation resized successfully.\n");

    } else {

        printf("Resize failed.\n");
    }
}


void CHANGE_STRATEGY(MemoryManager *manager) {

    printf("\n1. First Fit\n");
    printf("2. Best Fit\n");
    printf("3. Worst Fit\n");

    int choice = READ_INTEGER("Select allocation strategy: ");

    if (choice >= FIRST_FIT && choice <= WORST_FIT) {

        manager->strategy = (Strategy)choice;

        printf("Strategy changed successfully.\n");

    } else {

        printf("Invalid strategy.\n");
    }
}


void DESTROY(MemoryManager *manager) {

    free(manager->buffer);
    free(manager->blocks);

    manager->buffer = NULL;
    manager->blocks = NULL;

    manager->blockCount = 0;
    manager->capacity = 0;
    manager->totalSize = 0;
}


int main(void) {

    MemoryManager manager = {0};

    int totalSize = READ_INTEGER("Enter memory pool size: ");

    if (totalSize <= 0) {

        printf("Invalid memory pool size.\n");
        return 1;
    }

    printf(
        "\nSelect allocation strategy "
        "(1. First Fit, 2. Best Fit, 3. Worst Fit): "
    );

    int choice = READ_INTEGER("");

    if (choice < FIRST_FIT || choice > WORST_FIT) {

        printf("Invalid allocation strategy.\n");
        return 1;
    }

    if (!INITIALIZE(
        &manager,
        (size_t)totalSize,
        (Strategy)choice
    )) {

        printf("Memory initialization failed.\n");
        return 1;
    }

    if (manager.strategy == FIRST_FIT)
        printf("Strategy set to First Fit.\n");
    else if (manager.strategy == BEST_FIT)
        printf("Strategy set to Best Fit.\n");
    else
        printf("Strategy set to Worst Fit.\n");

    int menuChoice;

    do {

        MENU();

        menuChoice = READ_INTEGER("\nChoice: ");

        switch (menuChoice) {

            case 1:
                ALLOCATE_OPERATION(&manager);
                break;

            case 2:
                RELEASE_OPERATION(&manager);
                break;

            case 3:
                RESIZE_OPERATION(&manager);
                break;

            case 4:
                DISPLAY_MEMORY_MAP(&manager);
                break;

            case 5:
                DISPLAY_STATISTICS(&manager);
                break;

            case 6:
                CHANGE_STRATEGY(&manager);
                break;

            case 7:
                printf("Closing Memory Manager.\n");
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (menuChoice != 7);

    DESTROY(&manager);

    return 0;
}
