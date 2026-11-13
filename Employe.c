
#include <stdio.h>

#define MAX_EMPLOYEES 100

struct employee {
    int Id;
    char Name[50];
    char Designation[20];
    struct employee *Reports[MAX_EMPLOYEES];
    int reportCount;
    struct employee *ReportingManager;
};

struct employee employees[MAX_EMPLOYEES];
int occupied[MAX_EMPLOYEES] = {0};

void clearInput(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
}

int valid_name(const char name[]) {
    if (name[0] == '\0')
        return 0;

    for (int i = 0; name[i] != '\0'; i++) {
        if (!((name[i] >= 'A' && name[i] <= 'Z') ||
              (name[i] >= 'a' && name[i] <= 'z'))) {
            return 0;
        }
    }

    return 1;
}

int get_name(char name[], int size) {
    while (1) {
        printf("Enter name \n");

        if (scanf("%49s", name) != 1)
            return 0;

        clearInput();

        if (valid_name(name))
            return 1;

        printf("Only alphabets are allowed. Try again.\n");
    }
}

struct employee *allocateEmployee(void) {
    for (int i = 0; i < MAX_EMPLOYEES; i++) {
        if (!occupied[i]) {
            occupied[i] = 1;
            employees[i].Id = 0;
            employees[i].Name[0] = '\0';
            employees[i].Designation[0] = '\0';
            employees[i].reportCount = 0;
            employees[i].ReportingManager = NULL;
            return &employees[i];
        }
    }

    printf("Maximum employee capacity reached.\n");
    return NULL;
}

struct employee *findEmployee(int id) {
    for (int i = 0; i < MAX_EMPLOYEES; i++) {
        if (occupied[i] && employees[i].Id == id)
            return &employees[i];
    }

    return NULL;
}

int getUniqueId(void) {
    int id;

    while (1) {
        printf("Enter employee ID: ");

        if (scanf("%d", &id) != 1) {
            printf("Enter a valid integer.\n");
            clearInput();
            continue;
        }

        clearInput();

        if (id <= 0) {
            printf("ID must be positive.\n");
        } else if (findEmployee(id) != NULL) {
            printf("Employee ID already exists.\n");
        } else {
            return id;
        }
    }
}

void addEmployee(struct employee **root) {
    struct employee *emp = allocateEmployee();

    if (emp == NULL)
        return;

    emp->Id = getUniqueId();

    if (!get_name(emp->Name, sizeof(emp->Name))) {
        occupied[emp - employees] = 0;
        return;
    }

    printf("Enter designation: ");

    if (scanf("%19s", emp->Designation) != 1) {
        clearInput();
        occupied[emp - employees] = 0;
        return;
    }

    clearInput();

    if (!valid_name(emp->Designation)) {
        printf("Designation must contain only alphabets.\n");
        occupied[emp - employees] = 0;
        return;
    }

    if (*root == NULL) {
        *root = emp;
        printf("CEO created successfully!\n");
        return;
    }

    int managerID;
    struct employee *manager = NULL;

    while (manager == NULL) {
        printf("Enter reporting manager ID: ");

        if (scanf("%d", &managerID) != 1) {
            printf("Enter a valid integer.\n");
            clearInput();
            continue;
        }

        clearInput();

        manager = findEmployee(managerID);

        if (manager == NULL) {
            printf("Manager not found. Try again.\n");
        } else if (manager->reportCount >= MAX_EMPLOYEES) {
            printf("Manager cannot have more reports.\n");
            occupied[emp - employees] = 0;
            return;
        }
    }

    emp->ReportingManager = manager;
    manager->Reports[manager->reportCount++] = emp;

    printf("Employee added successfully!\n");
}

void displayEmployeeHierarchy(struct employee *emp, int level) {
    if (emp == NULL)
        return;

    for (int i = 0; i < level; i++)
        printf("    ");

    printf("ID: %d | Name: %s | Designation: %s\n",
           emp->Id, emp->Name, emp->Designation);

    for (int i = 0; i < emp->reportCount; i++) {
        displayEmployeeHierarchy(emp->Reports[i], level + 1);
    }
}

void displayCompleteHierarchy(struct employee *root) {
    if (root == NULL) {
        printf("No employees exist.\n");
        return;
    }

    displayEmployeeHierarchy(root, 0);
}

struct employee *findBestSibling(struct employee *emp) {
    struct employee *manager = emp->ReportingManager;
    struct employee *best = NULL;

    for (int i = 0; i < manager->reportCount; i++) {
        struct employee *sibling = manager->Reports[i];

        if (sibling == emp)
            continue;

        if (best == NULL ||
            sibling->reportCount < best->reportCount) {
            best = sibling;
        }
    }

    return best;
}

int removeEmployeeFromManager(struct employee *manager,
                              struct employee *emp) {
    int index = -1;

    for (int i = 0; i < manager->reportCount; i++) {
        if (manager->Reports[i] == emp) {
            index = i;
            break;
        }
    }

    if (index == -1)
        return 0;

    for (int i = index; i < manager->reportCount - 1; i++) {
        manager->Reports[i] = manager->Reports[i + 1];
    }

    manager->reportCount--;
    return 1;
}

int transferReports(struct employee *emp,
                    struct employee *recipient) {
    if (recipient->reportCount + emp->reportCount >
        MAX_EMPLOYEES) {
        return 0;
    }

    for (int i = 0; i < emp->reportCount; i++) {
        struct employee *report = emp->Reports[i];

        recipient->Reports[recipient->reportCount++] = report;
        report->ReportingManager = recipient;
    }

    emp->reportCount = 0;
    return 1;
}

void deleteEmployee(struct employee *root, int id) {
    struct employee *emp = findEmployee(id);

    if (emp == NULL) {
        printf("Employee not found.\n");
        return;
    }

    if (emp == root) {
        printf("CEO cannot be removed.\n");
        return;
    }

    struct employee *manager = emp->ReportingManager;
    struct employee *recipient = findBestSibling(emp);

    if (recipient == NULL)
        recipient = manager;

    if (recipient->reportCount + emp->reportCount >
        MAX_EMPLOYEES) {
        printf("Cannot transfer reports: recipient is full.\n");
        return;
    }

    if (!removeEmployeeFromManager(manager, emp)) {
        printf("Hierarchy is inconsistent.\n");
        return;
    }

    if (!transferReports(emp, recipient)) {
        printf("Could not transfer reports.\n");
        return;
    }

    occupied[emp - employees] = 0;

    printf("Employee deleted successfully.\n");
}

int main(void) {
    struct employee *root = NULL;
    int choice, id;

    while (1) {
        printf("\n===== EMPLOYEE HIERARCHY MANAGEMENT =====\n");
        printf("1. Add Employee\n");
        printf("2. Display Complete Hierarchy\n");
        printf("3. Display Employee Hierarchy\n");
        printf("4. Delete Employee\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting program.\n");
            break;
        }

        clearInput();

        switch (choice) {
            case 1:
                addEmployee(&root);
                break;

            case 2:
                displayCompleteHierarchy(root);
                break;

            case 3: {
                printf("Enter employee ID to search: ");

                if (scanf("%d", &id) != 1) {
                    printf("Invalid ID.\n");
                    clearInput();
                    break;
                }

                clearInput();

                struct employee *emp = findEmployee(id);

                if (emp != NULL) {
                    printf("\nEmployee Found:\n");
                    displayEmployeeHierarchy(emp, 0);
                } else {
                    printf("Employee not found.\n");
                }

                break;
            }

            case 4:
                printf("Enter employee ID to delete: ");

                if (scanf("%d", &id) != 1) {
                    printf("Invalid ID.\n");
                    clearInput();
                    break;
                }

                clearInput();
                deleteEmployee(root, id);
                break;

            case 5:
                printf("Exiting program.\n");
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
