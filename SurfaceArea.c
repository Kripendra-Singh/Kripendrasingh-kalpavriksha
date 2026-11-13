int surfaceArea(int A_rows, int A_columns, int** A) {

    int area = 0;

    for (int i = 0; i < A_rows; i++) {
        for (int j = 0; j < A_columns; j++) {

            int h = A[i][j];

            if (h == 0)
                continue;

          
            area += 2;

            
            if (j == 0)
                area += h;
            else if (h > A[i][j - 1])
                area += h - A[i][j - 1];

           
            if (j == A_columns - 1)
                area += h;
            else if (h > A[i][j + 1])
                area += h - A[i][j + 1];

            // Front side
            if (i == 0)
                area += h;
            else if (h > A[i - 1][j])
                area += h - A[i - 1][j];

            // Back side
            if (i == A_rows - 1)
                area += h;
            else if (h > A[i + 1][j])
                area += h - A[i + 1][j];
        }
    }

    return area;
}