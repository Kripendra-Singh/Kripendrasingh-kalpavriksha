#include <stdio.h>
#define TOTALTICKETS 20
enum TicketCategory
{
    STANDARD = 1,
    PREMIUM,
    RECLINER
};

int ticket_count = 0;
enum TicketCategory category;
int subtotal = 0;
int total = 0;
int discount = 0;
char name[50];
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

    return i > 0;
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
int get_count()
{
    while (1)
    {
        ticket_count = 0;
        printf("enter no. of tickets\n");
        if (scanf("%d", &ticket_count) == 1 && (ticket_count >= 1 && ticket_count <= TOTALTICKETS))
            return ticket_count;
        else
        {
            printf("enter valid no between 1 to 20");
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF)
                ;
        }
    }
}
enum TicketCategory get_category()
{
    int choice, ch;

    while (1)
    {
        printf("\nChoose ticket category:\n");
        printf("1. Standard   - Rs. 150\n");
        printf("2. Premiuim     - Rs. 250\n");
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
    }
}
int calculate_subTotal(int n)
{
    switch (n)
    {
    case 1:
        return ticket_count * 150;
        break;
    case 2:
        return ticket_count * 250;
        break;
    case 3:
        return ticket_count * 400;
        break;

    default:
        return 0;
        break;
    }
}
int calculate_total()
{
    subtotal = calculate_subTotal(category);
    if (ticket_count >= 5)
    {
        discount = 0.1 * subtotal;
        return subtotal - discount;
    }
    return subtotal;
}
void Display()
{
    int subtotal = calculate_subTotal(category);

    printf("\n");
    printf("********** TICKET BILL **********\n");
    printf("Customer Name : %s\n", name);
    printf("Ticket Count  : %d\n", ticket_count);
    printf("Ticket Price  : Rs. %d\n", subtotal / ticket_count);
    printf("---------------------------------\n");
    printf("Subtotal      : Rs. %d\n", subtotal);
    printf("Discount      : Rs. %d\n", discount);
    printf("Final Amount  : Rs. %d\n", total);
    printf("---------------------------------\n");
    printf("          THANK YOU!\n");
    printf("*********************************\n");
}
int main()
{
    printf("Ticket Booking Programe\n");
    printf("Enter your name\n");
    get_name(name);
    get_count();
    category = get_category();
    calculate_subTotal(category);
    total = calculate_total();
    Display();
    return 0;
}
