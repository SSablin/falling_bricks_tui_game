#include "random.h"

static void swap(int *p_i, int *p_j)
{
    int tmp = *p_i;
    *p_i = *p_j;
    *p_j = tmp;
}

void fisher_yates(int arr[], size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        int j = rand() % (i + 1);
        swap(&arr[i], &arr[j]);
    }
}
