#include <stdio.h>
#include <string.h>
#include <ctype.h>

// separate function to check operator
int isoperator(char ch)
{
    return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}

// function to check number
int isnumber(char exp[], int *i)
{
    int digitcount = 0;
    int decimalcount = 0;

    while (isdigit(exp[*i]) || exp[*i] == '.')
    {
        if (isdigit(exp[*i]))
            digitcount++;

        if (exp[*i] == '.')
            decimalcount++;

        if (decimalcount > 1)
            return 0;

        (*i)++;
    }

    if (digitcount == 0)
        return 0;

    return 1;
}

// function to check validity of expression
int isvalid(char exp[])
{
    int i = 0;
    int num = 1;

    while (exp[i] != '\0')
    {
        if (isspace(exp[i]))
        {
            i++;
            continue;
        }

        if (num)
        {
            if (exp[i] == '-')
            {
                i++;

                while (isspace(exp[i]))
                    i++;
            }

            if (!isnumber(exp, &i))
                return 0;

            num = 0;
        }
        else
        {
            if (isoperator(exp[i]))
            {
                num = 1;
                i++;
            }
            else
            {
                return 0;
            }
        }
    }

    if (num)
        return 0;

    return 1;
}

// function to calculate value of expression using two accumulator approach
int calculate(char exp[], int *error)
{
    *error = 0;

    int result = 0;
    int term = 0;
    int number = 0;
    char op = '+';
    int i = 0;

    while (exp[i] != '\0')
    {
        if (isdigit(exp[i]))
        {
            number = 0;

            while (isdigit(exp[i]))
            {
                number = number * 10 + (exp[i] - '0');
                i++;
            }

            if (op == '+')
            {
                result += term;
                term = number;
            }
            else if (op == '-')
            {
                result += term;
                term = -number;
            }
            else if (op == '*')
            {
                term = term * number;
            }
            else if (op == '/')
            {
                if (number == 0)
                {
                    *error = 1;
                    return 0;
                }

                term = term / number;
            }
        }
        else
        {
            op = exp[i];
            i++;
        }
    }

    result += term;

    return result;
}
void getexpression()
{
    char exp[100];
    int error = 0;

    printf("Enter expression: ");
    scanf(" %99[^\n]", exp);

    // Check expression validity
    if (isvalid(exp) != 1)
    {
        printf("Expression is not valid.\n");
        return;
    }

    // Calculate expression
    int result = calculate(exp, &error);

    if (error)
    {
        printf("Error: Division by zero.\n"); // handling divide by zero
        return;
    }

    printf("Result = %d\n", result);
}
int menu()
{
    int choice;

    printf("enter 1 to evaluate a expression\n");
    printf("enter 0 to exit from program\n");
    printf("enter your Choice..\n");

    int res = scanf("%d", &choice);

    if (res == EOF)  ///considering EOF as exit
    {
        return 0;
    }

    if (res != 1)
    {
        while (getchar() != '\n')
            ;

        return -1;
    }

    while (getchar() != '\n')
        ;

    return choice;
}

int main()
{

    printf("Calculator Program\n");

    char exp[100];
    int exit = 1;
    int error = 0;

    do
    {
        int choice = menu();
        switch (choice)
        {
        case 1:
            getexpression();
            break;
        case 0:
            printf("exiting");
            exit = 0;
            break;

        default:
            printf("\n please select a valid choice \n ");
            break;
        }

    } while (exit);

    printf("Calculator closed.\n"); // when while loop ends than claculator is closed

    return 0;
}