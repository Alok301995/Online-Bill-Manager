#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#define MAX_YEAR 9999
#define MIN_YEAR 1800

// ***********Global**************

typedef struct Records
{
    int day;
    int month;
    int year;
    char item_name[240];
    float value;
} Record;

typedef struct Client_Record
{
    char *file[2];
} Client_Record;

// *******************************

// *********** Untility Function **********
void swap(Record *a, Record *b)
{
    Record temp = *a;
    *a = *b;
    *b = temp;
}

int _NLINX(FILE *file)
{
    int _number_of_lines = 0;
    char ch;
    while ((ch = fgetc(file)) != EOF)
    {
        if (ch == '\n')
        {
            _number_of_lines++;
        }
    }
    if (ftell(file) > 0)
    {
        _number_of_lines = _number_of_lines + 1;
        return _number_of_lines;
    }
    return _number_of_lines;
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

void _print_precord(Record **arr_1, int i, Record **arr_2, int j)
{
    if (arr_1[i]->day < 10 && arr_1[i]->month > 9)
    {
        printf("0%d.%d.%d %s %f", arr_1[i]->day, arr_1[i]->month, arr_1[i]->year, arr_1[i]->item_name, arr_1[i]->value);
    }
    else if (arr_1[i]->day > 9 && arr_1[i]->month < 10)
    {
        printf("%d.0%d.%d %s %f", arr_1[i]->day, arr_1[i]->month, arr_1[i]->year, arr_1[i]->item_name, arr_1[i]->value);
    }
    else
    {
        printf("%d.%d.%d %s %f", arr_1[i]->day, arr_1[i]->month, arr_1[i]->year, arr_1[i]->item_name, arr_1[i]->value);
    }
    if (arr_2[i]->day < 10 && arr_2[i]->month > 9)
    {
        printf(" | 0%d.%d.%d %s %f\n", arr_2[j]->day, arr_2[j]->month, arr_2[j]->year, arr_2[j]->item_name, arr_2[j]->value);
    }
    else if (arr_2[i]->day > 9 && arr_2[i]->month < 10)
    {
        printf(" | %d.0%d.%d %s %f\n", arr_2[j]->day, arr_2[j]->month, arr_2[j]->year, arr_2[j]->item_name, arr_2[j]->value);
    }
    else
    {
        printf(" | %d.%d.%d %s %f\n", arr_2[j]->day, arr_2[j]->month, arr_2[j]->year, arr_2[j]->item_name, arr_2[j]->value);
    }
}

Record **_init_record(int size)
{
    Record **arr = (Record **)malloc(size * sizeof(Record *));
    for (int i = 0; i < size; i++)
    {
        arr[i] = (Record *)malloc(sizeof(Record));
    }
    return arr;
}

// ***********Date validation*******

int _is_leap(int year)
{
    if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int validate_date(int day, int month, int year)
{
    if (year < MIN_YEAR || year > MAX_YEAR)
        return 0;
    if (month < 1 || month > 12)
        return 0;
    if (day < 1 || day > 31)
        return 0;
    if (month == 2)
    {
        if (_is_leap(year))
        {
            if (day <= 29)
                return 1;
            else
                return 0;
        }
    }
    if (month == 4 || month == 6 || month == 9 || month == 11)
        if (day <= 30)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    return 1;
}

// *********************************

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

// ******************* Similarity  *************************

void _similarity(Record **r_1, Record **r_2, int size_1, int size_2)
{
    for (int i = 0; i < size_1; i++)
    {

        for (int j = 0; j < size_2; j++)
        {
            if (r_1[i]->day == r_2[j]->day && r_1[i]->month == r_2[j]->month && r_1[i]->year == r_2[j]->year)
            {
                _print_precord(r_1, i, r_2, j);
            }
            else if (strcmp(r_1[i]->item_name, r_2[j]->item_name) == 0)
            {
                _print_precord(r_1, i, r_2, j);
            }
            else if (r_1[i]->value == r_2[j]->value)
            {
                _print_precord(r_1, i, r_2, j);
            }
        }
    }
}
// ***********************************************************
// file to Record

Record **_load_file(FILE *file, int size)
{
    char buffer[300];
    Record **record = _init_record(size);
    int index = 0;
    while (fscanf(file, "%[^\n]\n", buffer) != EOF && index < size)
    {
        sscanf(buffer, "%d.%d.%d %[^\t] %f", &record[index]->day, &record[index]->month, &record[index]->year, record[index]->item_name, &record[index]->value);
        index++;
    }
    return record;
}

// Record to file

void _record_to_file(Record **arr, int size)
{
    // check if record is empty or not
    if (arr == NULL)
    {
        _print_msg("Unsuccessful\n");
    }
    else
    {

        FILE *out_file = fopen("output.txt", "w");
        char buff[300];
        for (int i = 0; i < size; i++)
        {
            if (arr[i]->day < 10 && arr[i]->month > 9)
            {

                snprintf(buff, 300, "0%d.%d.%d\t%s\t%f", arr[i]->day, arr[i]->month, arr[i]->year, arr[i]->item_name, arr[i]->value);
            }
            else if (arr[i]->day > 9 && arr[i]->month < 10)
            {
                snprintf(buff, 300, "%d.0%d.%d\t%s\t%f", arr[i]->day, arr[i]->month, arr[i]->year, arr[i]->item_name, arr[i]->value);
            }
            else
            {
                snprintf(buff, 300, "%d.%d.%d\t%s\t%f", arr[i]->day, arr[i]->month, arr[i]->year, arr[i]->item_name, arr[i]->value);
            }
            fputs(buff, out_file);
            fputc('\n', out_file);
        }
        fclose(out_file);
    }
}

// merge to file

void _merge_to_file(Record **r_1, Record **r_2, int size_1, int size_2, char *type)
{
    Record **merged_record = _merge(r_1, r_2, size_1, size_2, type);
    if (merged_record == NULL)
    {
        _print_msg(" merge unsuccessful");
    }
    else
    {
        _record_to_file(merged_record, size_1 + size_2);
    }
}
// *********************************************************************************
// ************ Recieve from client*********
void _process_client_request(char *buff, int _n_files, int fd)
{
}

// *****************************************

// *********************************************************

// *********************************************************

int main(int argc, char *argv[])
{
    int PORT = atoi(argv[1]);
    int sockfd, newfd, len;
    struct sockaddr_in addr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&addr, sizeof(addr));

    // assign IP, PORT
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT);
    if ((bind(sockfd, (struct sockaddr *)&addr, sizeof(addr))) != 0)
    {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    if ((listen(sockfd, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);
    newfd = accept(sockfd, (struct sockaddr *)&cli, &len);
    if (newfd < 0)
    {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accept the client...\n");
    int max_size = 1024;
    char buff[max_size];
    char com[20];
    char field[20];
    int _n_files;

    while (1)
    {
        bzero(buff, sizeof(buff));
        read(newfd, buff, sizeof(buff));
        sscanf(buff, "%[^' '] %[^\n]", com, field);
        bzero(buff, max_size);
        sprintf(buff, "%d", 1);
        write(newfd, buff, sizeof(buff));
        bzero(buff, max_size);
        read(newfd, buff, sizeof(buff));
        if (strcmp(buff, "-1") == 0)
        {
            continue;
        }
        else
        {
            sscanf(buff, "%d", &_n_files);
            bzero(buff, max_size);
            sprintf(buff, "%d", 1);
            write(newfd, buff, sizeof(buff));
            bzero(buff, max_size);
            sprintf(buff, "%d", 1);
            write(newfd, buff, 1024);
            Client_Record *file_rec = (Client_Record *)malloc(sizeof(Client_Record));
            file_rec->file[0] = "out_1.txt";
            file_rec->file[1] = "out_2.txt";
            for (int i = 0; i < _n_files; i++)
            {
                FILE *fp = fopen(file_rec->file[i], "w+");
                bzero(buff, 1024);
                while (1)
                {
                    read(newfd, buff, sizeof(buff));
                    if (strcmp(buff, "1") == 0)
                    {
                        fclose(fp);
                        break;
                    }
                    fputs(buff, fp);
                    bzero(buff, 1024);
                    sprintf(buff, "%d", 1);
                    write(newfd, buff, sizeof(buff));
                }
                bzero(buff, 1024);
            }
        }
    }
    close(newfd);
    close(sockfd);
}