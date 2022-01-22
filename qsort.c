#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ***********Global**************

typedef struct Records
{
    int day;
    int month;
    int year;
    char *item_name;
    float value;
} Record;

// *******************************

// *********** Untility Function **********
void swap(Record *a, Record *b)
{
    Record temp = *a;
    *a = *b;
    *b = temp;
}
// ****************************************
// ***************Value Comparator*********

int _value_comparator(Record *a, Record *b)
{
    if (a->value > b->value)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
// ****************String Comparator**********************

int _string_comparator(Record *a, Record *b)
{
    if (strcmp(a->item_name, b->item_name) > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
// ***************Date Comparator**************************

int _date_comparator(Record *a, Record *b)
{
    if (a->year > b->year)
    {
        return 1;
    }
    else if (a->year == b->year && a->month > b->month)
    {
        return 1;
    }
    else if (a->year == b->year && a->month == b->month && a->day > b->day)
    {
        return 1;
    }
    else
        return 0;
}

// ********************************************************

// **************Partition**************************
int partition(Record **arr, int low, int high, int (*comparator)(Record *, Record *))
{
    int pivot = low;
    while (low <= high)
    {
        if (comparator(arr[low], arr[pivot]) && comparator(arr[pivot], arr[high]))
        {
            swap(arr[low], arr[high]);
            low++;
            high--;
        }
        else
        {
            if (!comparator(arr[low], arr[pivot]))
            {
                low++;
            }
            if (!comparator(arr[pivot], arr[high]))
            {
                high--;
            }
        }
    }
    swap(arr[high], arr[pivot]);
    return high;
}

void qsHelper(Record **arr, int low, int high, int (*comparator)(Record *, Record *))
{
    if (low < high)
    {

        int index = partition(arr, low, high, comparator);
        qsHelper(arr, low, index - 1, comparator);
        qsHelper(arr, index + 1, high, comparator);
    }
}

void sort(Record **arr, int size, char *feild_type)
{
    if (strcmp(feild_type, "value") == 0)
    {
        qsHelper(arr, 0, size - 1, _value_comparator);
    }
    else if (strcmp(feild_type, "item_name") == 0)
    {
        qsHelper(arr, 0, size - 1, _string_comparator);
    }
    else if (strcmp(feild_type, "Date") == 0)
    {
        qsHelper(arr, 0, size - 1, _date_comparator);
    }
    else
    {
        return;
    }
}
// **************************************************

int main()
{
    Record **records = (Record **)malloc(5 * sizeof(Record *));
    for (int i = 0; i < 5; i++)
    {
        records[i] = (Record *)malloc(sizeof(Record));
    }

    records[0]->item_name = "b";
    records[0]->value = 5;
    records[1]->item_name = "aman";
    records[1]->value = 1;
    records[2]->item_name = "chp";
    records[2]->value = 3;
    records[3]->item_name = "rahul";
    records[3]->value = 1;
    records[4]->item_name = "shbfs";
    records[4]->value = 1;
    // for (int i = 0; i < 5; i++)
    // {
    //     printf("%d ", records[i]->number);
    // }
    sort(records, 5, "value");
    for (int i = 0; i < 5; i++)
    {
        printf("%s ", records[i]->item_name);
    }
    printf("\n");
    for (int i = 0; i < 5; i++)
    {
        printf("%f ", records[i]->value);
    }
    printf("\n");
}