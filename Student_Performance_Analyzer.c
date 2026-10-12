
#include <stdio.h>

#define MAX_STUDENTS 100
#define NAME_SIZE 50
#define MARKS_COUNT 3
#define MIN_MARKS 0.0f
#define MAX_MARKS 100.0f

struct Student
{
    int roll_number;
    char name[NAME_SIZE];
    float marks_1;
    float marks_2;
    float marks_3;
};

int get_roll_number()
{
    int roll_number;

    while (1)
    {
        if (scanf("%d", &roll_number)==1 && roll_number> 0)
        {
            return roll_number;
        }

        printf("Invalid roll number. Enter again: ");

        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF)
            ;

        if (ch == EOF)
            return 0;
    }
}

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

int get_input(struct Student students[], int student_count)
{
    for (int i = 0; i < student_count; i++)
    {
        printf("\nEnter Student %d details\n", i + 1);
        printf("(Roll_Number Name Marks_1 Marks_2 Marks_3)\n");

        students[i].roll_number = get_roll_number();

        if (students[i].roll_number == 0)
            return 0;

        if (get_name(students[i].name) == 0)
            return 0;

        int valid;

        do
        {
            printf("Enter %d marks: ", MARKS_COUNT);

            int result = scanf("%f %f %f",
                               &students[i].marks_1,
                               &students[i].marks_2,
                               &students[i].marks_3);

            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF)
                ;

            if (result != MARKS_COUNT)
            {
                if (result == EOF || ch == EOF)
                    return 0;

                printf("Invalid input! Please enter 3 numeric marks.\n");
                valid = 0;
            }
            else if (students[i].marks_1 < MIN_MARKS ||
                     students[i].marks_1 > MAX_MARKS ||
                     students[i].marks_2 < MIN_MARKS ||
                     students[i].marks_2 > MAX_MARKS ||
                     students[i].marks_3 < MIN_MARKS ||
                     students[i].marks_3 > MAX_MARKS)
            {
                printf("Invalid marks! Marks must be between %.0f and %.0f.\n",
                       MIN_MARKS, MAX_MARKS);
                valid = 0;
            }
            else if (ch == EOF)
            {
                return 0;
            }
            else
            {
                valid = 1;
            }

        } while (valid == 0);
    }

    return 1;
}

float calculate_total(struct Student student)
{
    return student.marks_1 +
           student.marks_2 +
           student.marks_3;
}

float calculate_average(struct Student student)
{
    return calculate_total(student) / MARKS_COUNT;
}

char calculate_grade(float average)
{
    switch ((int)average / 10)
    {
        case 10:  //for both 9 and 10
        case 9:
            return 'A';

        case 8:
            if (average >= 85)
                return 'A';
            return 'B';
 
        case 7:    // for both 6 and 7
        case 6:
            return 'B';

        case 5:
            return 'C';

        case 4:
            return 'D';

        case 3:
            if (average >= 35)
                return 'D';
            return 'F';

        default:
            return 'F';
    }
}

int calculate_performance(float average)
{
    if (average >= 85)
        return 5;
    else if (average >= 70)
        return 4;
    else if (average >= 50)
        return 3;
    else if (average >= 35)
        return 2;
    else
        return 0;
}

int print_performance(float average)
{
    int stars = calculate_performance(average);

    printf("Performance: ");

    for (int i = 0; i < stars; i++)
    {
        printf("*");
    }

    printf("\n");

    return stars;
}

int display_students(struct Student students[], int student_count)
{
    for (int i = 0; i < student_count; i++)
    {
        float total = calculate_total(students[i]);
        float average = calculate_average(students[i]);
        char grade = calculate_grade(average);

        printf("\n-------------------------\n");

        printf("Roll Number : %d\n", students[i].roll_number);
        printf("Name        : %s\n", students[i].name);
        printf("Marks 1     : %.2f\n", students[i].marks_1);
        printf("Marks 2     : %.2f\n", students[i].marks_2);
        printf("Marks 3     : %.2f\n", students[i].marks_3);
        printf("Total       : %.2f\n", total);
        printf("Average     : %.2f\n", average);
        printf("Grade       : %c\n", grade);

        print_performance(average);
    }

    return student_count;
}

int print_roll_numbers(struct Student students[], int student_count)
{
    if (student_count == 0)
        return 0;

    int printed_count =
        print_roll_numbers(students, student_count - 1);

    printf("%d ", students[student_count - 1].roll_number);

    return printed_count + 1;
}

int main(void)
{
    int student_count;

    printf("Enter number of students: ");

    if (scanf("%d", &student_count) != 1)
    {
        printf("Enter a valid number.\n");
        return 1;
    }

    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;

    if (student_count <= 0 || student_count > MAX_STUDENTS)
    {
        printf("Invalid number of students. Enter 1 to %d.\n",
               MAX_STUDENTS);
        return 1;
    }

    struct Student students[student_count];

    printf("\nEnter details in this format:\n");
    printf("Roll_Number Name Marks_1 Marks_2 Marks_3\n");

    if (get_input(students, student_count) == 0)
    {
        printf("\nInput ended or failed. Program terminated.\n");
        return 1;
    }

    display_students(students, student_count);

    printf("\nRoll numbers of all students: ");
    print_roll_numbers(students, student_count);
    printf("\n");

    return 0;
}
