/*
 * @Author: Kripendra Singh
 * @Description: This program demonstrates a Smart Memory Management System.
 */


#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 1000000
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

int readInteger(char message[]);
int ensureCapacity(MemoryManager *manager);
int insertBlock(MemoryManager *manager, size_t index, Block block);
void removeBlock(MemoryManager *manager, size_t index);
void mergeBlocks(MemoryManager *manager);
int initialize(MemoryManager *manager, size_t totalSize, Strategy strategy);
int findAllocation(MemoryManager *manager, unsigned int id);
int findBlock(MemoryManager *manager, size_t size);
int allocateMemory(MemoryManager *manager, size_t size, unsigned int *id);
int releaseMemory(MemoryManager *manager, unsigned int id);
int resizeMemory(MemoryManager *manager, unsigned int id, size_t newSize);
void displayMemoryMap(MemoryManager *manager);
void displayStatistics(MemoryManager *manager);
void menu(void);
void allocateOperation(MemoryManager *manager);
int checkExternalFragmentation(MemoryManager *manager, size_t requestedSize);
void releaseOperation(MemoryManager *manager);
void resizeOperation(MemoryManager *manager);
void changeStrategy(MemoryManager *manager);
void destroy(MemoryManager *manager);


int readInteger(char message[]) {

    int number;
    char extra;

    printf("%s", message);

    if (scanf("%d%c", &number, &extra) != 2 || extra != '\n') {
        printf("Invalid input.\n");

        while (getchar() != '\n');

        return -1;
    }

    return number;
}


int ensureCapacity(MemoryManager *manager) {

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


int insertBlock(MemoryManager *manager, size_t index, Block block) {

    if (index > manager->blockCount || !ensureCapacity(manager))
        return 0;

    for (size_t i = manager->blockCount; i > index; i--) {
        manager->blocks[i] = manager->blocks[i - 1];
    }

    manager->blocks[index] = block;
    manager->blockCount++;

    return 1;
}


void removeBlock(MemoryManager *manager, size_t index) {

    if (index >= manager->blockCount)
        return;

    for (size_t i = index; i < manager->blockCount - 1; i++) {
        manager->blocks[i] = manager->blocks[i + 1];
    }

    manager->blockCount--;
}


void mergeBlocks(MemoryManager *manager) {

    size_t i = 0;

    while (i + 1 < manager->blockCount) {

        Block *current = &manager->blocks[i];
        Block *next = &manager->blocks[i + 1];

        if (current->status == FREE && next->status == FREE) {

            current->size += next->size;
            removeBlock(manager, i + 1);

        } else {
            i++;
        }
    }
}


int initialize(
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


int findAllocation(MemoryManager *manager, unsigned int id) {

    for (size_t i = 0; i < manager->blockCount; i++) {

        if (manager->blocks[i].status == ALLOCATED &&
            manager->blocks[i].id == id)
            return (int)i;
    }

    return -1;
}


int findBlock(MemoryManager *manager, size_t size) {

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


int checkExternalFragmentation(
    MemoryManager *manager,
    size_t requestedSize
) {

    size_t totalFree = 0;
    size_t largestFree = 0;

    for (size_t i = 0; i < manager->blockCount; i++) {

        if (manager->blocks[i].status == FREE) {

            totalFree += manager->blocks[i].size;

            if (manager->blocks[i].size > largestFree) {
                largestFree = manager->blocks[i].size;
            }
        }
    }

    if (totalFree >= requestedSize && largestFree < requestedSize)
        return 1;

    return 0;
}


int allocateMemory(
    MemoryManager *manager,
    size_t size,
    unsigned int *id
) {

    if (size == 0 || size > manager->totalSize ||
        manager->nextId == 0)
        return 0;

    int index = findBlock(manager, size);

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

        if (!insertBlock(manager, index, newBlock))
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


int releaseMemory(MemoryManager *manager, unsigned int id) {

    int index = findAllocation(manager, id);

    if (index == -1)
        return 0;

    manager->blocks[index].status = FREE;
    manager->blocks[index].id = 0;

    mergeBlocks(manager);

    return 1;
}


int resizeMemory(
    MemoryManager *manager,
    unsigned int id,
    size_t newSize
) {

    int index = findAllocation(manager, id);

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

        if (!insertBlock(manager, index + 1, freeBlock))
            return 0;

        manager->blocks[index].size = newSize;

        mergeBlocks(manager);

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
                removeBlock(manager, index + 1);

            return 1;
        }
    }


    unsigned int newId;

    if (!allocateMemory(manager, newSize, &newId))
        return 0;

    int newIndex = findAllocation(manager, newId);

    if (newIndex == -1)
        return 0;

    for (size_t i = 0; i < oldSize; i++) {
        manager->buffer[manager->blocks[newIndex].start + i] =
            manager->buffer[oldStart + i];
    }

    if (!releaseMemory(manager, id))
        return 0;

    newIndex = findAllocation(manager, newId);

    if (newIndex == -1)
        return 0;

    manager->blocks[newIndex].id = id;

    return 1;
}


void displayMemoryMap(MemoryManager *manager) {

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


void displayStatistics(MemoryManager *manager) {

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


void menu(void) {

    printf("\nMenu:\n");
    printf("1. Allocate memory\n");
    printf("2. Release memory\n");
    printf("3. Resize allocation\n");
    printf("4. Display memory map\n");
    printf("5. Display memory statistics\n");
    printf("6. Change allocation strategy\n");
    printf("7. Exit\n");
}


void allocateOperation(MemoryManager *manager) {

    int size = readInteger("Requested size: ");

    unsigned int id;

    if (size > 0 &&
        allocateMemory(manager, (size_t)size, &id)) {

        int index = findAllocation(manager, id);

        printf(
            "Allocation successful. ID: %u, Start: %zu, Size: %zu\n",
            id,
            manager->blocks[index].start,
            manager->blocks[index].size
        );

    } else {

        if (size > 0 &&
            checkExternalFragmentation(manager, (size_t)size)) {

            printf("Allocation failed due to external fragmentation.\n");

        } else {

            printf("Allocation failed due to insufficient memory.\n");
        }
    }
}


void releaseOperation(MemoryManager *manager) {

    int id = readInteger("Allocation ID: ");

    if (id > 0 && releaseMemory(manager, (unsigned int)id)) {

        printf("Allocation released.\n");

    } else {

        printf("Invalid allocation ID.\n");
    }
}


void resizeOperation(MemoryManager *manager) {

    int id = readInteger("Allocation ID: ");
    int size = readInteger("Requested new size: ");

    if (id > 0 && size > 0 &&
        resizeMemory(manager, (unsigned int)id, (size_t)size)) {

        printf("Allocation resized successfully.\n");

    } else {

        printf("Resize failed.\n");
    }
}


void changeStrategy(MemoryManager *manager) {

    printf("\n1. First Fit\n");
    printf("2. Best Fit\n");
    printf("3. Worst Fit\n");

    int choice = readInteger("Select allocation strategy: ");

    if (choice >= FIRST_FIT && choice <= WORST_FIT) {

        manager->strategy = (Strategy)choice;

        printf("Strategy changed successfully.\n");

    } else {

        printf("Invalid strategy.\n");
    }
}


void destroy(MemoryManager *manager) {

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

    int totalSize = readInteger("Enter memory pool size: ");

    if (totalSize <= 0 || totalSize > MAX_SIZE) {

        printf("Invalid memory pool size.\n");
        return 1;
    }

    printf(
        "\nSelect allocation strategy "
        "(1. First Fit, 2. Best Fit, 3. Worst Fit): "
    );

    int choice = readInteger("");

    if (choice < FIRST_FIT || choice > WORST_FIT) {

        printf("Invalid allocation strategy.\n");
        return 1;
    }

    if (!initialize(
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

        menu();

        menuChoice = readInteger("\nChoice: ");

        switch (menuChoice) {

            case 1:
                allocateOperation(&manager);
                break;

            case 2:
                releaseOperation(&manager);
                break;

            case 3:
                resizeOperation(&manager);
                break;

            case 4:
                displayMemoryMap(&manager);
                break;

            case 5:
                displayStatistics(&manager);
                break;

            case 6:
                changeStrategy(&manager);
                break;

            case 7:
                printf("Closing Memory Manager.\n");
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (menuChoice != 7);

    destroy(&manager);

    return 0;
}
