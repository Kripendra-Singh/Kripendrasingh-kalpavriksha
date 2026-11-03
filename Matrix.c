
#include <stdio.h>
#include <stdlib.h>

#define MAX 10
#define MIN 2

int get_input(void);
void generate_matrix(int n, int *matrix);
void display_matrix(int n, int *matrix);
void transpose_matrix(int n, int *matrix);
void rotate_matrix_90(int n, int *matrix);
void smooth_matrix(int n, int *matrix);

int get_input(void) {
    char str[100];
    int n;
    char extra;

    printf("Enter size of Matrix:\n");

    while (1) {
        if (fgets(str, sizeof(str), stdin) == NULL)
            return -1;

        if (sscanf(str, " %d %c", &n, &extra) == 1 &&
            n >= MIN && n <= MAX) {
            return n;
        }

        printf("Enter a valid number between 2 to 10:\n");
    }
}

void generate_matrix(int n, int *matrix) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            *(matrix + i * n + j) = rand() % 256;
        }
    }
}

void display_matrix(int n, int *matrix) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%4d", *(matrix + i * n + j));
        }
        printf("\n");
    }
}

void transpose_matrix(int n, int *matrix) {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int *p = matrix + i * n + j;
            int *q = matrix + j * n + i;

            int temp = *p;
            *p = *q;
            *q = temp;
        }
    }
}

void rotate_matrix_90(int n, int *matrix) {
    transpose_matrix(n, matrix);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n / 2; j++) {
            int *p = matrix + i * n + j;
            int *q = matrix + i * n + (n - 1 - j);

            int temp = *p;
            *p = *q;
            *q = temp;
        }
    }
}

void smooth_matrix(int n, int *matrix) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int sum = 0;
            int count = 0;

            for (int x = -1; x <= 1; x++) {
                for (int y = -1; y <= 1; y++) {
                    int r = i + x;
                    int c = j + y;

                    if (r >= 0 && r < n && c >= 0 && c < n) {
                        int *p = matrix + r * n + c;

                        sum += (*p & 255);//this will extract lower 8 bits that contains original values 
                        count++;
                    }
                }
            }

            int *p = matrix + i * n + j;

            *p = (*p & 255) | ((sum / count) << 8);// this line will store the calculated average in upper 8 bits 
        }
    }

    for (int i = 0; i < n * n; i++) {
        *(matrix + i) = *(matrix + i) >> 8;//this will obtain new average values by shifting the upper bits 
    }
}

int main(void) {
    int n = get_input();

    if (n == -1)
        return 0;

    int *matrix = malloc(n * n * sizeof(int));

    if (matrix == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    generate_matrix(n, matrix);

    printf("\nOriginal Matrix:\n");
    display_matrix(n, matrix);

    printf("\nRotated Matrix:\n");
    rotate_matrix_90(n, matrix);
    display_matrix(n, matrix);

    printf("\nMatrix after smoothing:\n");
    smooth_matrix(n, matrix);
    display_matrix(n, matrix);

    free(matrix);
    matrix = NULL;

    return 0;
}
