int GET_TWICE_MEDIAN(int count[], int d)
{
    int cumulative = 0;
    int median1 = -1;
    int median2 = -1;

    for (int i = 0; i <= 200; i++)
    {
        cumulative += count[i];

        if (median1 == -1 && cumulative > (d - 1) / 2)
        {
            median1 = i;
        }

        if (cumulative > d / 2)
        {
            median2 = i;
            break;
        }
    }

    if (d % 2 == 1)
    {
        return 2 * median1;
    }

    return median1 + median2;
}

int activityNotifications(int expenditure_count, int* expenditure, int d)
{
    int count[201] = {0};
    int notifications = 0;

    for (int i = 0; i < d; i++)
    {
        count[expenditure[i]]++;
    }

    for (int i = d; i < expenditure_count; i++)
    {
        int twiceMedian = GET_TWICE_MEDIAN(count, d);

        if (expenditure[i] >= twiceMedian)
        {
            notifications++;
        }

        count[expenditure[i - d]]--;
        count[expenditure[i]]++;
    }

    return notifications;
}