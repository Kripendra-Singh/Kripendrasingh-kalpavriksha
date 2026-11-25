void countSort(int arr_rows, int arr_columns, char*** arr)
{
    int count[100] = {0};
    int position[100] = {0};

    char **sorted = malloc(arr_rows * sizeof(char *));

    int i;
    int key;

    for (i = 0; i < arr_rows; i++)
    {
        key = atoi(arr[i][0]);
        count[key]++;
    }

    position[0] = 0;

    for (i = 1; i < 100; i++)
    {
        position[i] = position[i - 1] + count[i - 1];
    }

    for (i = 0; i < arr_rows; i++)
    {
        key = atoi(arr[i][0]);

        if (i < arr_rows / 2)
        {
            sorted[position[key]++] = "-";
        }
        else
        {
            sorted[position[key]++] = arr[i][1];
        }
    }

    for (i = 0; i < arr_rows; i++)
    {
        printf("%s", sorted[i]);

        if (i < arr_rows - 1)
        {
            printf(" ");
        }
    }

    printf("\n");

    free(sorted);
}