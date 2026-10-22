#include <stdio.h>
#include <stdlib.h>
struct employee{
    int Id;
    char Name[50];
    char Designation[20];
    struct employee *Reports;
    struct employee *ReportingManager;
    struct employee *next;

};

int valid_name(char name[])
{
    int i = 0;
 
    while (name[i] != '\0')
    {
        if (!((name[i] >= 'A' && name[i] <= 'Z') ||
              (name[i] >= 'a' && name[i] <= 'z')))
        {
            return 0;
        }

        i++;
    }

    return 1;
}
int get_name(char name[])
{
    while (1)
    {
        if (scanf("%49s", name) != 1)
            return 0;

        if (valid_name(name))
            return 1;

        printf("Invalid name. Enter again: ");

        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF)
            ;

        if (ch == EOF)
            return 0;
    }
}
int isUniqueID(struct employee *root, int id)
{
    if (root == NULL)
    {
        return 1;
    }

    if (root->Id == id)
    {
        return 0;
    }

    if (isUniqueID(root->Reports, id) == 0)
    {
        return 0;
    }

    return isUniqueID(root->next, id);
}
int get_employeeId(struct employee *root)
{
    int id;
    int ch;

    while (1)
    {
        printf("Enter employee ID: ");

        if (scanf("%d", &id) != 1)
        {
            printf("Please enter a valid integer.\n");

            while ((ch = getchar()) != '\n' && ch != EOF)
                ;

            if (ch == EOF)
                return -1;

            continue;
        }

        while ((ch = getchar()) != '\n' && ch != EOF)
            ;

        if (id <= 0)
        {
            printf("ID must be positive.\n");
            continue;
        }

        if (!isUniqueID(root, id))
        {
            printf("Employee ID already exists. Enter another ID.\n");
            continue;
        }

        return id;
    }
}
struct employee *findEmployee(struct employee *root, int id)
{
    if (root == NULL)
    {
        return NULL;
    }

    if (root->Id == id)
    {
        return root;
    }

    struct employee *temp = root->Reports;

    while (temp != NULL)
    {
        struct employee *found = findEmployee(temp, id);

        if (found != NULL)
        {
            return found;
        }

        temp = temp->next;
    }

    return NULL;
}

struct employee *createEmployee(struct employee *root)
{
    struct employee *newEmployee = malloc(sizeof(struct employee));

    if (newEmployee == NULL)
    {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    newEmployee->Id = get_employeeId(root);

    if (newEmployee->Id == -1)
    {
        free(newEmployee);
        return NULL;
    }

    printf("Enter employee name: ");
    get_name(newEmployee->Name);

    printf("Enter designation: ");
    get_name(newEmployee->Designation);

    newEmployee->Reports = NULL;
    newEmployee->ReportingManager = NULL;
    newEmployee->next = NULL;

    if (root == NULL)
    {
        printf("Employee created as CEO.\n");
        return newEmployee;
    }

    int managerID;
    struct employee *manager = NULL;

    while (manager == NULL)
    {
        printf("Enter reporting manager ID: ");

        if (scanf("%d", &managerID) != 1)
        {
            printf("Please enter a valid integer.\n");

            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF)
                ;

            if (ch == EOF)
            {
                free(newEmployee);
                return NULL;
            }

            continue;
        }

        while (getchar() != '\n')
            ;

        manager = findEmployee(root, managerID);

        if (manager == NULL)
        {
            printf("Manager not found. Enter a valid ID.\n");
        }
    }

    newEmployee->ReportingManager = manager;

    return newEmployee;
}
void addEmployee(struct employee **root)
{
    struct employee *newEmployee = createEmployee(*root);

    if (newEmployee == NULL)
    {
        return;
    }

    if (*root == NULL)
    {
        *root = newEmployee;
        printf("CEO created successfully!\n");
        return;
    }

    struct employee *manager = newEmployee->ReportingManager;

    struct employee *temp = manager->Reports;

if (temp == NULL)
{
    manager->Reports = newEmployee;
}
else
{
    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    temp->next = newEmployee;
}
    printf("Employee added successfully!\n");
}

void displayEmployeeHierarchy(struct employee *emp, int level)
{
    if (emp == NULL)
    {
        return;
    }

    for (int i = 0; i < level; i++)
    {
        printf("    ");
    }

    printf("ID: %d | Name: %s | Designation: %s\n",
           emp->Id, emp->Name, emp->Designation);

    struct employee *temp = emp->Reports;

    while (temp != NULL)
    {
        displayEmployeeHierarchy(temp, level + 1);
        temp = temp->next;
    }
}
void displayCompleteHierarchy(struct employee *root)
{
    if (root == NULL)
    {
        printf("No employees in the hierarchy.\n");
        return;
    }

    displayEmployeeHierarchy(root, 0);
}
int countDirectReports(struct employee *emp)
{
    int count = 0;

    struct employee *temp = emp->Reports;

    while (temp != NULL)
    {
        count++;
        temp = temp->next;
    }

    return count;
}
struct employee *findBestSibling(struct employee *emp)
{
    struct employee *manager = emp->ReportingManager;
    struct employee *temp = manager->Reports;

    struct employee *best = NULL;
    int minCount = 0;

    while (temp != NULL)
    {
        if (temp != emp)
        {
            int count = countDirectReports(temp);

            if (best == NULL || count < minCount)
            {
                best = temp;
                minCount = count;
            }
        }

        temp = temp->next;
    }

    return best;
}
void transferReports(struct employee *emp,
                     struct employee *recipient)
{
    if (emp->Reports == NULL)
    {
        return;
    }

    struct employee *temp = emp->Reports;

    while (temp != NULL)
    {
        temp->ReportingManager = recipient;
        temp = temp->next;
    }

    temp = emp->Reports;

    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    temp->next = recipient->Reports;
    recipient->Reports = emp->Reports;

    emp->Reports = NULL;
}

void deleteEmployee(struct employee *root, int id)
{
    if (root == NULL)
    {
        printf("No employees exist.\n");
        return;
    }

    struct employee *emp = findEmployee(root, id);

    if (emp == NULL)
    {
        printf("Employee not found.\n");
        return;
    }

    if (emp == root)
    {
        printf("CEO cannot be removed.\n");
        return;
    }

    struct employee *manager = emp->ReportingManager;
    struct employee *recipient = findBestSibling(emp);

    
    struct employee **current = &manager->Reports;

    while (*current != NULL && *current != emp)
    {
        current = &(*current)->next;
    }

    if (*current != emp)
    {
        printf("Employee hierarchy is inconsistent.\n");
        return;
    }

    *current = emp->next;
    emp->next = NULL;

   
    if (recipient == NULL)
    {
        recipient = manager;
    }

    transferReports(emp, recipient);

    free(emp);

    printf("Employee deleted successfully.\n");
}


int main()
{
    struct employee *root = NULL;
    int choice, id;
    struct employee *emp;

    while (1)
    {
        printf("\n===== EMPLOYEE HIERARCHY MANAGEMENT =====\n");
        printf("1. Add Employee\n");
        printf("2. Display Complete Hierarchy\n");
        printf("3. Display Employee Hierarchy\n");
        printf("4. Delete Employee\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Exiting program.\n");
            break;
        }

        switch (choice)
        {
            case 1:
                addEmployee(&root);
                break;

            case 2:
                displayCompleteHierarchy(root);
                break;

            case 3:
                printf("Enter employee ID to search: ");

                if (scanf("%d", &id) != 1)
                {
                    printf("Invalid ID.\n");
                    return 1;
                }

                emp = findEmployee(root, id);

                if (emp != NULL)
                {
                    printf("\nEmployee Found:\n");
                    displayEmployeeHierarchy(emp, 0);
                }
                else
                {
                    printf("Employee not found.\n");
                }
                break;

            case 4:
                printf("Enter employee ID to delete: ");

                if (scanf("%d", &id) != 1)
                {
                    printf("Invalid ID.\n");
                    return 1;
                }

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