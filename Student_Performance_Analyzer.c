#include <stdio.h>

struct Student
{
    int rollnumber;
    char name[50];
    float marks1;
    float marks2;
    float marks3;
};

int getRollNumber()
{
    int rollnumber;

    while (1)
    {
        if (scanf("%d",&rollnumber)==1&&rollnumber>0)
        {
            return rollnumber;
        }

        printf("Invalid Roll Number. Enter again: ");

        while (getchar() != '\n')
            ;
    }
}

int validName(char name[])
{
    int i = 0;

    while (name[i]!='\0')
    {
        if (!((name[i]>='A'&&name[i]<='Z') ||
              (name[i]>='a'&&name[i]<='z')))
        {
            return 0;
        }

        i++;
    }

    return 1;
}

void getname(char name[])
{
    while (1)
    {
        scanf("%49s",name);

        if (validName(name))
        {
            return;
        }

        printf("Invalid Name.Enter again\n");
        while (getchar() != '\n');
    }
}

/// input function
void getinput(struct Student s[], int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("\nEnter Student %d details ", i + 1);
        printf("(RollNumber Name Marks1 Marks2 Marks3)\n");

        s[i].rollnumber = getRollNumber();

        getname(s[i].name);

        int valid;

        do
        {
            printf("Enter 3 marks ");

            int result=scanf("%f %f %f",
                               &s[i].marks1,
                               &s[i].marks2,
                               &s[i].marks3);

            while (getchar()!='\n')
                ;

            if (result!=3)
            {
                printf("Invalid input! Please enter 3 numeric marks.\n");
                valid = 0;
            }
            else if (s[i].marks1<0||s[i].marks1>100||
                     s[i].marks2<0 || s[i].marks2 > 100 ||
                     s[i].marks3<0||s[i].marks3>100)
            {
                printf("Invalid marks! Marks must be between 0 and 100\n");
                valid=0;
            }
            else
            {
                valid=1;
            }

        } while (valid==0);
    }
}

float calculateTotal(struct Student s)
{
    return s.marks1+s.marks2+s.marks3;
}


float calculateAverage(struct Student s)
{
    return calculateTotal(s)/3;
}

// Calculate Grade 
char calculateGrade(float average)
{
    if (average>=90)
        return 'A';
    else if (average>=80)
        return 'B';
    else if (average>=70)
        return 'C';
    else if (average>=60)
        return 'D';
    else if (average>=50)
        return 'E';
    else
        return 'F';
}

// Print Performance 
void printPerformance(float average)
{
    int stars;

    if (average >= 90)
        stars=5;
    else if (average >= 80)
        stars=4;
    else if (average >= 70)
        stars=3;
    else if (average >= 60)
        stars=2;
    else if (average >= 50)
        stars=1;
    else
        stars=0;

    printf("Performance: ");

    for (int i=0;i<stars;i++)
    {
        printf("*");
    }

    printf("\n");
}

// Display Student Details 
void display(struct Student s[], int n)
{
    for (int i=0;i<n;i++)
    {
        float total = calculateTotal(s[i]);
        float average = calculateAverage(s[i]);
        char grade = calculateGrade(average);

        printf("\n-------------------------\n");

        printf("Roll Number : %d\n", s[i].rollnumber);
        printf("Name        : %s\n", s[i].name);
        printf("Marks 1     : %.2f\n", s[i].marks1);
        printf("Marks 2     : %.2f\n", s[i].marks2);
        printf("Marks 3     : %.2f\n", s[i].marks3);
        printf("Total       : %.2f\n", total);
        printf("Average     : %.2f\n", average);
        printf("Grade       : %c\n", grade);

        printPerformance(average);
    }
}

///Recursive function to print all roll numbers 
void printRollNumbers(struct Student s[], int n)
{
    if (n==0)
    {
        return;
    }

    printRollNumbers(s,n - 1);

    printf("%d ",s[n - 1].rollnumber);
}

int main()
{
    int n;

    printf("Enter number of students: ");
   if(scanf("%d",&n)!=1) {
    printf("enter valid number \n");
      while (getchar() != '\n');
            return 0;

   }
    while (getchar() != '\n');   // it will clear invalid inputs after number of students 

    if (n<=0||n>100)
    {
        printf("invalid number of students\n");
        return 0;
    }

    struct Student s[n];

    printf("\nEnter details in this format\n");
    printf("RollNumber Name Marks1 Marks2 Marks3\n");

    getinput(s,n);

    display(s,n);

    printf("\nRoll Numbers of all students:");
    printRollNumbers(s,n);
    printf("\n");

    return 0;
}