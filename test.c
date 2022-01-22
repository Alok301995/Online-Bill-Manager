#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// ***********Global**************

typedef struct Records
{
    int day;
    int month;
    int year;
    char item_name[240];
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

// for printing msg

void _print_msg(char *msg)
{
    printf("%s\n", msg);
}

void _print_record(Record **arr, int size)
{
    printf("\n");
    for (int i = 0; i < size; i++)
    {
        printf("%d %d %d %s %f \n", arr[i]->day, arr[i]->month, arr[i]->year, arr[i]->item_name, arr[i]->value);
    }
}

Record **_init_record(int size)
{
    Record **arr = (Record **)malloc(5 * sizeof(Record *));
    for (int i = 0; i < size; i++)
    {
        arr[i] = (Record *)malloc(sizeof(Record));
    }
    return arr;
}

// ******************************************
// ***************Comparators*********

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

// ************ Function to check the file is sorted or not WRT to feild **************

int _check_sorted_helper(Record **arr, int size, int (*comparator)(Record *, Record *))
{
    for (int i = 0; i < size - 1; i++)
    {
        if (comparator(arr[i], arr[i + 1]))
        {
            return 0;
        }
    }
    return 1;
}

int _check_sorted(Record **arr, int size, char *type)
{
    if (strcmp(type, "price") == 0)
    {
        return _check_sorted_helper(arr, size, _value_comparator);
    }
    else if (strcmp(type, "name") == 0)
    {
        return _check_sorted_helper(arr, size, _string_comparator);
    }
    else if (strcmp(type, "date") == 0)
    {
        return _check_sorted_helper(arr, size, _date_comparator);
    }
    else
    {
        return 0;
    }
}

// ********************************************

// **************SORT**************************
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
    if (strcmp(feild_type, "price") == 0)
    {
        qsHelper(arr, 0, size - 1, _value_comparator);
    }
    else if (strcmp(feild_type, "name") == 0)
    {
        qsHelper(arr, 0, size - 1, _string_comparator);
    }
    else if (strcmp(feild_type, "date") == 0)
    {
        qsHelper(arr, 0, size - 1, _date_comparator);
    }
    else
    {
        return;
    }
}
// ********************************************************
// *************Merge function*****************************
// Assuming that both the records are sorted order

Record **_merge_helper(Record **arr_1, Record **arr_2, int size_1, int size_2, int (*comparator)(Record *, Record *))
{
    Record **new_record = _init_record(size_1 + size_2);
    int i = 0;
    int j = 0;
    int k = 0;
    while (i < size_1 && j < size_2)
    {
        if (!comparator(arr_1[i], arr_2[j]))
        {
            new_record[k] = arr_1[i];
            i++;
            k++;
        }
        else
        {
            new_record[k] = arr_2[j];
            j++;
            k++;
        }
    }
    while (i < size_1)
    {
        new_record[k] = arr_1[i];
        i++;
    }
    while (j < size_2)
    {
        new_record[k] = arr_2[j];
        j++;
    }
    return new_record;
}

Record **_merge(Record **arr_1, Record **arr_2, int size_1, int size_2, char *type)
{
    if (_check_sorted(arr_1, size_1, type) && _check_sorted(arr_2, size_2, type))
    {

        if (strcmp(type, "price") == 0)
        {
            return _merge_helper(arr_1, arr_2, size_1, size_2, _value_comparator);
        }
        else if (strcmp(type, "name") == 0)
        {
            return _merge_helper(arr_1, arr_2, size_1, size_2, _string_comparator);
        }
        else if (strcmp(type, "date") == 0)
        {
            return _merge_helper(arr_1, arr_2, size_1, size_2, _date_comparator);
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
}

// *********************************************************

int main()
{
    // // testing strcmp() function sort strings in lexicographic order
    // Record *records[15];
    // for (int i = 0; i < 15; i++)
    // {
    //     records[i] = (Record *)malloc(sizeof(Record));
    // }

    FILE *file = fopen("bill_1.txt", "r");
    // // /feof() is used to get position of file pointer
    // // struct stat sb;
    // // stat("bill_1.txt", &sb);

    char buffer[300];
    // int count = 0;

    // while (fscanf(file, "%[^\n]\n", buffer) != EOF && count < 15)
    // {
    //     sscanf(buffer, "%d.%d.%d %[^\t] %f", &records[count]->day, &records[count]->month, &records[count]->year, records[count]->item_name, &records[count]->value);
    //     count++;
    // }
    // count = 0;
    // // sort(records, 5, "Date");
    // while (count < 15)
    // {

    //     // printf("%d ", records[count]->day);
    //     // printf("%d ", records[count]->month);
    //     // printf("%d ", records[count]->year);
    //     printf("%s ", records[count]->item_name);
    //     printf("%f \n", records[count]->value);
    //     count++;
    // }
    // int check = _check_sorted(records, 15, "item_name");
    // printf("%d\n", check);
    // sort(records, 15, "value");
    // count = 0;
    // printf("\n");
    // while (count < 15)
    // {

    //     printf("%d ", records[count]->day);
    //     printf("%d ", records[count]->month);
    //     printf("%d ", records[count]->year);
    //     printf("%s ", records[count]->item_name);
    //     printf("%f \n", records[count]->value);
    //     count++;
    // }

    Record **r_1 = _init_record(5);
    Record **r_2 = _init_record(5);

    int count = 0;
    while (fscanf(file, "%[^\n]\n", buffer) != EOF && count < 5)
    {
        sscanf(buffer, "%d.%d.%d %[^\t] %f", &r_1[count]->day, &r_1[count]->month, &r_1[count]->year, r_1[count]->item_name, &r_1[count]->value);
        count++;
    }
    // _print_record(r_1, 5);
    while (fscanf(file, "%[^\n]\n", buffer) != EOF && count < 10)
    {
        sscanf(buffer, "%d.%d.%d %[^\t] %f", &r_2[count - 5]->day, &r_2[count - 5]->month, &r_2[count - 5]->year, r_2[count - 5]->item_name, &r_2[count - 5]->value);
        count++;
    }
    // _print_record(r_2, 5);
    sort(r_1, 5, "name");
    sort(r_2, 5, "name");
    // _print_record(r_1, 5);
    // _print_record(r_2, 5);
    Record **merged_record = _merge(r_1, r_2, 5, 5, "name");
    if (merged_record == NULL)
    {
        _print_msg("can not merged");
    }
    else
    {
        _print_record(merged_record, 10);
    }
}