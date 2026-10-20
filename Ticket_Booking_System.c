
#include <stdio.h>
#define TOTALTICKETS 20

enum TicketCategory
{
    STANDARD = 1,
    PREMIUM = 2,
    RECLINER = 3
};

struct Input
{
    char name[50];
    int ticket_count;
    enum TicketCategory category;
};

struct Calculation
{
    int subtotal;
    int total;
    int discount;
};

struct Input i = {0};
struct Calculation c = {0};

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

int get_count(void)
{
    int ch;

    while (1)
    {
        i.ticket_count = 0;
        printf("Enter number of tickets (1-20): ");

        if (scanf("%d", &i.ticket_count) == 1 &&
            i.ticket_count >= 1 && i.ticket_count <= TOTALTICKETS)
        {
            return i.ticket_count;
        }
        else
        {
            printf("Enter a valid number between 1 and 20.\n");

            while ((ch = getchar()) != '\n' && ch != EOF)
                ;

            if (ch == EOF)
                return 0;
        }
    }
}

enum TicketCategory get_category(void)
{
    int choice, ch;

    while (1)
    {
        printf("1. Standard - Rs. 150\n");
        printf("2. Premium  - Rs. 250\n");
        printf("3. Recliner - Rs. 400\n");
        printf("Enter choice (1-3): ");

        if (scanf("%d", &choice) == 1 &&
            choice >= STANDARD && choice <= RECLINER)
        {
            while ((ch = getchar()) != '\n' && ch != EOF)
                ;

            return (enum TicketCategory)choice;
        }

        printf("Invalid category. Try again.\n");

        while ((ch = getchar()) != '\n' && ch != EOF)
            ;

        if (ch == EOF)
            return 0;
    }
}

int calculate_subTotal(int n)
{
    switch (n)
    {
        case STANDARD:
            return i.ticket_count * 150;

        case PREMIUM:
            return i.ticket_count * 250;

        case RECLINER:
            return i.ticket_count * 400;

        default:
            return 0;
    }
}

int calculate_total(void)
{
    c.subtotal = calculate_subTotal(i.category);
    c.discount = 0;

    if (i.ticket_count >= 5)
    {
        c.discount = c.subtotal / 10;
    }

    return c.subtotal - c.discount;
}

void Display(void)
{
    printf("\n********** TICKET BILL **********\n");
    printf("Customer Name : %s\n", i.name);
    printf("Ticket Count  : %d\n", i.ticket_count);
    printf("Final Amount  : Rs. %d\n", c.total);
}

int main(void)
{
    printf("Ticket Booking Program\n");
    printf("Enter your name: ");

    if (!get_name(i.name))
        return 1;

    if (get_count() == 0)
        return 1;

    i.category = get_category();

    if (i.category == 0)
        return 1;

    c.total = calculate_total();

    Display();

    return 0;
}
