int* matchingStrings(int stringList_count, char** stringList,
                     int queries_count, char** queries,
                     int* result_count) {

    int *result = malloc(queries_count * sizeof(int));

    *result_count = queries_count;

    for (int i = 0; i < queries_count; i++) {

        int count = 0;

        for (int j = 0; j < stringList_count; j++) {

            if (strcmp(queries[i], stringList[j]) == 0) {
                count++;
            }
        }

        result[i] = count;
    }

    return result;
}
