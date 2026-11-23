/*
 * @Author: Kripendra Singh
 * @Description: This program demonstrates a Smart Virtual File System.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 512
#define NUM_BLOCKS 1024
#define MAX_NAME 50
#define MAX_INPUT 4096

typedef enum {
    FILE_TYPE=0,
    DIRECTORY_TYPE=1
} NodeType;

typedef struct FreeBlock {
    int index;
    struct FreeBlock *next;
    struct FreeBlock *prev;
} FreeBlock;

typedef struct FileNode {
    char name[MAX_NAME + 1];
    NodeType type;

    struct FileNode *parent;
    struct FileNode *next;
    struct FileNode *prev;

    struct FileNode *children;

    int *blockPointers;
    int blockCount;
    size_t contentSize;

} FileNode;

typedef struct {
    unsigned char (*virtualDisk)[BLOCK_SIZE];

    FreeBlock *freeHead;
    FreeBlock *freeTail;

    FileNode *root;
    FileNode *cwd;

    int totalBlocks;
    int freeBlocks;

} VirtualFileSystem;

VirtualFileSystem vfs;

void initialize(VirtualFileSystem *vfs);
FreeBlock *createFreeBlock(int index);
FileNode *createNode(const char *name, NodeType type);
void insertFreeBlock(VirtualFileSystem *vfs, int index);
int removeFreeBlock(VirtualFileSystem *vfs);
void insertChild(FileNode *parent, FileNode *child);
void removeChild(FileNode *parent, FileNode *child);
FileNode *findChild(FileNode *parent, const char *name);
FileNode *findFile(const char *name);
void makeDirectory(const char *name);
void createFile(const char *name);
void changeDirectory(const char *name);
void listDirectory(void);
void printWorkingDirectory(void);
void writeFile(const char *name, const char *content);
void readFile(const char *name);
void deleteFile(const char *name);
void removeDirectory(const char *name);
void displayDiskUsage(void);
void releaseFile(FileNode *file);
void freeDirectory(FileNode *directory);
void cleanup(void);
void printPrompt(void);

FreeBlock *createFreeBlock(int index) {

    FreeBlock *newBlock = malloc(sizeof(FreeBlock));

    if (newBlock == NULL) {
        return NULL;
    }

    newBlock->index = index;
    newBlock->next = NULL;
    newBlock->prev = NULL;
    return newBlock;
}

FileNode *createNode(const char *name, NodeType type) {

    FileNode *newNode = calloc(1, sizeof(FileNode));

    if (newNode == NULL) {
        return NULL;
    }

    strncpy(newNode->name, name, MAX_NAME);
    newNode->name[MAX_NAME] = '\0';

    newNode->type = type;

    return newNode;
}

void insertFreeBlock(VirtualFileSystem *vfs, int index) {

    FreeBlock *newBlock = createFreeBlock(index);

    if (newBlock == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    if (vfs->freeHead == NULL) {

        vfs->freeHead = newBlock;
        vfs->freeTail = newBlock;

    } else {

        newBlock->prev = vfs->freeTail;
        vfs->freeTail->next = newBlock;
        vfs->freeTail = newBlock;
    }

    vfs->freeBlocks++;
}

int removeFreeBlock(VirtualFileSystem *vfs) {

    if (vfs->freeHead == NULL) {
        return -1;
    }

    FreeBlock *temp = vfs->freeHead;
    int index = temp->index;

    vfs->freeHead = temp->next;

    if (vfs->freeHead == NULL) {

        vfs->freeTail = NULL;

    } else {

        vfs->freeHead->prev = NULL;
    }

    free(temp);
    vfs->freeBlocks--;

    return index;
}

void insertChild(FileNode *parent, FileNode *child) {

    if (parent->children == NULL) {

        parent->children = child;
        child->next = child;
        child->prev = child;

    } else {

        FileNode *tail = parent->children->prev;

        child->next = parent->children;
        child->prev = tail;

        tail->next = child;
        parent->children->prev = child;
    }

    child->parent = parent;
}

void removeChild(FileNode *parent, FileNode *child) {

    if (parent->children == NULL) {
        return;
    }

    if (child->next == child) {

        parent->children = NULL;

    } else {

        child->prev->next = child->next;
        child->next->prev = child->prev;

        if (parent->children == child) {
            parent->children = child->next;
        }
    }

    child->next = NULL;
    child->prev = NULL;
    child->parent = NULL;
}

FileNode *findChild(FileNode *parent, const char *name) {

    if (parent->children == NULL) {
        return NULL;
    }

    FileNode *current = parent->children;

    do {

        if (strcmp(current->name, name) == 0) {
            return current;
        }

        current = current->next;

    } while (current != parent->children);

    return NULL;
}

FileNode *findFile(const char *name) {

    FileNode *file = findChild(vfs.cwd, name);

    if (file == NULL) {

        printf("Error: Entry '%s' not found.\n", name);
        return NULL;
    }

    if (file->type != FILE_TYPE) {

        printf("Error: '%s' is a directory.\n", name);
        return NULL;
    }

    return file;
}

void initialize(VirtualFileSystem *vfs) {

    vfs->totalBlocks = NUM_BLOCKS;
    vfs->freeBlocks = 0;

    vfs->freeHead = NULL;
    vfs->freeTail = NULL;

    vfs->virtualDisk = malloc(
        NUM_BLOCKS * sizeof(*vfs->virtualDisk)
    );

    if (vfs->virtualDisk == NULL) {

        printf("Disk initialization failed.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < NUM_BLOCKS; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            vfs->virtualDisk[i][j] = 0;
        }
    }

    for (int i = 0; i < NUM_BLOCKS; i++) {
        insertFreeBlock(vfs, i);
    }

    vfs->root = createNode("/", DIRECTORY_TYPE);

    if (vfs->root == NULL) {

        printf("Root initialization failed.\n");
        exit(EXIT_FAILURE);
    }

    vfs->cwd = vfs->root;
}

void makeDirectory(const char *name) {

    if (strlen(name) == 0 || strlen(name) > MAX_NAME ||
        strcmp(name, ".") == 0 ||
        strcmp(name, "..") == 0 ||
        strcmp(name, "/") == 0) {

        printf("Invalid directory name.\n");
        return;
    }

    if (findChild(vfs.cwd, name) != NULL) {

        printf("Error: Entry already exists.\n");
        return;
    }

    FileNode *newDirectory = createNode(name, DIRECTORY_TYPE);

    if (newDirectory == NULL) {

        printf("Memory allocation failed.\n");
        return;
    }

    insertChild(vfs.cwd, newDirectory);

    printf("Directory '%s' created successfully.\n", name);
}

void createFile(const char *name) {

    if (strlen(name) == 0 || strlen(name) > MAX_NAME ||
        strcmp(name, ".") == 0 ||
        strcmp(name, "..") == 0 ||
        strcmp(name, "/") == 0) {

        printf("Invalid file name.\n");
        return;
    }

    if (findChild(vfs.cwd, name) != NULL) {

        printf("Error: Entry already exists.\n");
        return;
    }

    FileNode *newFile = createNode(name, FILE_TYPE);

    if (newFile == NULL) {

        printf("Memory allocation failed.\n");
        return;
    }

    insertChild(vfs.cwd, newFile);

    printf("File '%s' created successfully.\n", name);
}

void changeDirectory(const char *name) {

    if (strcmp(name, "/") == 0) {

        vfs.cwd = vfs.root;
        printf("Moved to /\n");
        return;
    }

    if (strcmp(name, "..") == 0) {

        if (vfs.cwd->parent != NULL) {
            vfs.cwd = vfs.cwd->parent;
        }

        printf("Moved to ");
        printWorkingDirectory();
        printf("\n");

        return;
    }

    if (strcmp(name, ".") == 0) {

        printWorkingDirectory();
        printf("\n");
        return;
    }

    FileNode *directory = findChild(vfs.cwd, name);

    if (directory == NULL) {

        printf("Error: Directory not found.\n");
        return;
    }

    if (directory->type != DIRECTORY_TYPE) {

        printf("Error: Not a directory.\n");
        return;
    }

    vfs.cwd = directory;

    printf("Moved to ");
    printWorkingDirectory();
    printf("\n");
}

void listDirectory(void) {

    if (vfs.cwd->children == NULL) {

        printf("(empty)\n");
        return;
    }

    FileNode *current = vfs.cwd->children;

    do {

        if (current->type == DIRECTORY_TYPE) {
            printf("%s/\n", current->name);
        } else {
            printf("%s\n", current->name);
        }

        current = current->next;

    } while (current != vfs.cwd->children);
}

void printWorkingDirectory(void) {

    FileNode *current = vfs.cwd;

    char path[2048] = "";
    char temp[2048];

    while (current != NULL && current != vfs.root) {

        int length = snprintf(
            temp,
            sizeof(temp),
            "/%s%s",
            current->name,
            path
        );

        if (length >= sizeof(temp)) {

            printf("Error: Path is too long.\n");
            return;
        }

        strcpy(path, temp);
        current = current->parent;
    }

    if (strlen(path) == 0) {
        printf("/");
    } else {
        printf("%s", path);
    }
}

void writeFile(const char *name, const char *content) {

    FileNode *file = findFile(name);

    if (file == NULL) {
        return;
    }

    size_t dataSize = strlen(content);

    int requiredBlocks =
        (int)((dataSize + BLOCK_SIZE - 1) / BLOCK_SIZE);

    if (requiredBlocks > vfs.freeBlocks + file->blockCount) {

        printf("Error: Not enough disk space.\n");
        return;
    }

    int *newPointers = NULL;

    if (requiredBlocks > 0) {

        newPointers = malloc(
            requiredBlocks * sizeof(int)
        );

        if (newPointers == NULL) {

            printf("Memory allocation failed.\n");
            return;
        }
    }

    for (int i = 0; i < file->blockCount; i++) {
        insertFreeBlock(&vfs, file->blockPointers[i]);
    }

    free(file->blockPointers);

    file->blockPointers = newPointers;
    file->blockCount = 0;
    file->contentSize = 0;

    for (int i = 0; i < requiredBlocks; i++) {

        int index = removeFreeBlock(&vfs);

        if (index == -1) {

            printf("Disk allocation failed.\n");

            for (int j = 0; j < file->blockCount; j++) {
                insertFreeBlock(&vfs, file->blockPointers[j]);
            }

            free(file->blockPointers);

            file->blockPointers = NULL;
            file->blockCount = 0;
            file->contentSize = 0;

            return;
        }

        file->blockPointers[i] = index;
        file->blockCount++;
    }

    size_t copied = 0;

    for (int i = 0; i < file->blockCount; i++) {

        size_t remaining = dataSize - copied;

        size_t chunkSize =
            remaining > BLOCK_SIZE ? BLOCK_SIZE : remaining;

        for (size_t j = 0; j < chunkSize; j++) {

            vfs.virtualDisk[file->blockPointers[i]][j] =
                content[copied + j];
        }

        copied += chunkSize;
    }

    file->contentSize = dataSize;

    printf("Data written successfully (size=%zu bytes).\n",
           dataSize);
}

void readFile(const char *name) {

    FileNode *file = findFile(name);

    if (file == NULL) {
        return;
    }

    if (file->contentSize == 0) {

        printf("(empty file)\n");
        return;
    }

    size_t remaining = file->contentSize;

    for (int i = 0; i < file->blockCount; i++) {

        size_t chunkSize =
            remaining > BLOCK_SIZE ? BLOCK_SIZE : remaining;

        fwrite(
            vfs.virtualDisk[file->blockPointers[i]],
            1,
            chunkSize,
            stdout
        );

        remaining -= chunkSize;
    }

    printf("\n");
}

void releaseFile(FileNode *file) {

    for (int i = 0; i < file->blockCount; i++) {

        insertFreeBlock(
            &vfs,
            file->blockPointers[i]
        );
    }

    free(file->blockPointers);

    file->blockPointers = NULL;
    file->blockCount = 0;
    file->contentSize = 0;
}

void deleteFile(const char *name) {

    FileNode *file = findFile(name);

    if (file == NULL) {
        return;
    }

    releaseFile(file);
    removeChild(vfs.cwd, file);
    free(file);

    printf("File deleted successfully.\n");
}

void removeDirectory(const char *name) {

    FileNode *directory = findChild(vfs.cwd, name);

    if (directory == NULL) {

        printf("Error: Directory not found.\n");
        return;
    }

    if (directory->type != DIRECTORY_TYPE) {

        printf("Error: Not a directory.\n");
        return;
    }

    if (directory == vfs.cwd) {

        printf("Error: Cannot remove the current working directory.\n");
        return;
    }

    if (directory->children != NULL) {

        printf("Error: Directory is not empty.\n");
        return;
    }

    removeChild(vfs.cwd, directory);
    free(directory);

    printf("Directory removed successfully.\n");
}

void displayDiskUsage(void) {

    int usedBlocks =
        vfs.totalBlocks - vfs.freeBlocks;

    double usage =
        ((double)usedBlocks / vfs.totalBlocks) * 100.0;

    printf("Total Blocks: %d\n", vfs.totalBlocks);
    printf("Used Blocks: %d\n", usedBlocks);
    printf("Free Blocks: %d\n", vfs.freeBlocks);
    printf("Disk Usage: %.2f%%\n", usage);
}

void freeDirectory(FileNode *directory) {

    while (directory->children != NULL) {

        FileNode *child = directory->children;

        removeChild(directory, child);

        if (child->type == DIRECTORY_TYPE) {
            freeDirectory(child);
        } else {
            releaseFile(child);
        }

        free(child);
    }
}

void cleanup(void) {

    if (vfs.root != NULL) {

        freeDirectory(vfs.root);
        free(vfs.root);

        vfs.root = NULL;
        vfs.cwd = NULL;
    }

    FreeBlock *current = vfs.freeHead;

    while (current != NULL) {

        FreeBlock *next = current->next;
        free(current);
        current = next;
    }

    vfs.freeHead = NULL;
    vfs.freeTail = NULL;

    free(vfs.virtualDisk);
    vfs.virtualDisk = NULL;
}

void printPrompt(void) {

    if (vfs.cwd == vfs.root) {
        printf("/ > ");
    } else {
        printf("%s > ", vfs.cwd->name);
    }

    fflush(stdout);
}

int main(void) {

    char input[MAX_INPUT];
    char command[20];
    char name[MAX_NAME + 1];

    initialize(&vfs);

    printf("Compact VFS - ready. Type 'exit' to quit.\n");

    while (1) {

        printPrompt();

        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        command[0] = '\0';
        name[0] = '\0';

        if (sscanf(input, "%19s", command) != 1) {
            continue;
        }

        char *arguments = input + strlen(command);

        while (*arguments == ' ' || *arguments == '\t') {
            arguments++;
        }

        if (strcmp(command, "mkdir") == 0) {

            if (sscanf(arguments, "%50s", name) == 1) {
                makeDirectory(name);
            } else {
                printf("Usage: mkdir directory_name\n");
            }

        } else if (strcmp(command, "create") == 0) {

            if (sscanf(arguments, "%50s", name) == 1) {
                createFile(name);
            } else {
                printf("Usage: create file_name\n");
            }

        } else if (strcmp(command, "cd") == 0) {

            if (sscanf(arguments, "%50s", name) == 1) {
                changeDirectory(name);
            } else {
                printf("Usage: cd directory_name\n");
            }

        } else if (strcmp(command, "ls") == 0) {

            listDirectory();

        } else if (strcmp(command, "pwd") == 0) {

            printWorkingDirectory();
            printf("\n");

        } else if (strcmp(command, "write") == 0) {

            char *contentStart = arguments;

            if (sscanf(arguments, "%50s", name) != 1) {

                printf("Usage: write filename \"content\"\n");
                continue;
            }

            contentStart += strlen(name);

            while (*contentStart == ' ' ||
                   *contentStart == '\t') {
                contentStart++;
            }

            size_t length = strlen(contentStart);

            if (length >= 2 &&
                contentStart[0] == '"' &&
                contentStart[length - 1] == '"') {

                contentStart[length - 1] = '\0';
                contentStart++;
            }

            writeFile(name, contentStart);

        } else if (strcmp(command, "read") == 0) {

            if (sscanf(arguments, "%50s", name) == 1) {
                readFile(name);
            } else {
                printf("Usage: read filename\n");
            }

        } else if (strcmp(command, "delete") == 0) {

            if (sscanf(arguments, "%50s", name) == 1) {
                deleteFile(name);
            } else {
                printf("Usage: delete filename\n");
            }

        } else if (strcmp(command, "rmdir") == 0) {

            if (sscanf(arguments, "%50s", name) == 1) {
                removeDirectory(name);
            } else {
                printf("Usage: rmdir directory_name\n");
            }

        } else if (strcmp(command, "df") == 0) {

            displayDiskUsage();

        } else if (strcmp(command, "exit") == 0) {

            cleanup();

            printf("Memory released. Exiting program...\n");
            break;

        } else {

            printf("Invalid command.\n");
        }
    }

    if (vfs.root != NULL) {
        cleanup();
    }

    return 0;
}
