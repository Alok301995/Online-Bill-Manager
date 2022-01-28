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

// *********Global **********

typedef struct _connected_client
{
    int client_fd[5];
    int con_client;
    int MAX_CLIENT;

} Client_info;

// *********************************

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

int NLINEX(FILE *file)
{
    int count = 0;
    char buffer[100];
    while (!feof(file) && fgets(buffer, 1000, file))
    {
        if (strlen(buffer) > 0)
        {
            count++;
        }
    }
    // printf("%d\n", count);
    return count;
}

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

int validate_date(char *str)
{
    int day;
    int month;
    int year;
    sscanf(str, "%d.%d.%d", &day, &month, &year);

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

int check_dec_digits(char *str)
{
    char d[30];
    char e[30];
    sscanf(str, "%[^.] %s", d, e);
    if (strlen(e) > 3)
    {
        return 0;
    }
    return 1;
}
int check_validity(FILE *fp)
{
    char buffer[100];
    while (!feof(fp))
    {
        char str_1[100];
        char str_2[100];
        char str_3[100];
        fgets(buffer, 100, fp);

        int count = sscanf(buffer, "%[^\t] %[^\t] %[^\n]", str_1, str_2, str_3);
        // printf("count %d\n", count);
        if (count != 3)
        {
            return 0;
        }
        else if (!check_dec_digits(str_3))
        {
            return 0;
        }
        else if (validate_date(str_1) == 0)
        {
            return 0;
        }
    }
    return 1;
}

// for printing msg

void _print_msg(char *msg)
{
    printf("%s\n", msg);
}

Record **_init_record(int size)
{
    Record **arr = (Record **)malloc(size * sizeof(Record *));
    printf("%d\n", size);
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
    if (strcmp(type, "P") == 0)
    {
        return _check_sorted_helper(arr, size, _value_comparator);
    }
    else if (strcmp(type, "N") == 0)
    {
        return _check_sorted_helper(arr, size, _string_comparator);
    }
    else if (strcmp(type, "D") == 0)
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
    if (strcmp(feild_type, "P") == 0)
    {
        qsHelper(arr, 0, size - 1, _value_comparator);
    }
    else if (strcmp(feild_type, "N") == 0)
    {
        qsHelper(arr, 0, size - 1, _string_comparator);
    }
    else if (strcmp(feild_type, "D") == 0)
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
        k++;
    }
    while (j < size_2)
    {
        new_record[k] = arr_2[j];
        j++;
        k++;
    }
    return new_record;
}

Record **_merge(Record **arr_1, Record **arr_2, int size_1, int size_2, char *type)
{
    // _print_record(arr_1, size_1);
    // _print_record(arr_2, size_2);
    if (_check_sorted(arr_1, size_1, type) && _check_sorted(arr_2, size_2, type))
    {

        if (strcmp(type, "P") == 0)
        {
            return _merge_helper(arr_1, arr_2, size_1, size_2, _value_comparator);
        }
        else if (strcmp(type, "N") == 0)
        {
            return _merge_helper(arr_1, arr_2, size_1, size_2, _string_comparator);
        }
        else if (strcmp(type, "D") == 0)
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

// ***********************************************************
// file to Record

Record **_load_file(FILE *file, int size)
{
    char buffer[1000];
    Record **record = _init_record(size);
    int index = 0;
    while (fscanf(file, "%[^\n]\n", buffer) != EOF && index < size)
    {
        sscanf(buffer, "%d.%d.%d %[^\t] %f", &record[index]->day, &record[index]->month, &record[index]->year, record[index]->item_name, &record[index]->value);
        bzero(buffer, 1000);
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

                snprintf(buff, 300, "0%d.%d.%d\t%s\t%.2f", arr[i]->day, arr[i]->month, arr[i]->year, arr[i]->item_name, arr[i]->value);
            }
            else if (arr[i]->day > 9 && arr[i]->month < 10)
            {
                snprintf(buff, 300, "%d.0%d.%d\t%s\t%.2f", arr[i]->day, arr[i]->month, arr[i]->year, arr[i]->item_name, arr[i]->value);
            }
            else
            {
                snprintf(buff, 300, "%d.%d.%d\t%s\t%.2f", arr[i]->day, arr[i]->month, arr[i]->year, arr[i]->item_name, arr[i]->value);
            }
            fputs(buff, out_file);
            if (i == size - 1)
            {
            }
            else
            {
                fputc('\n', out_file);
            }
        }
        fclose(out_file);
        for (int i = 0; i < size; i++)
        {
            free(arr[i]);
        }
        free(arr);
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

int main(int argc, char *argv[])
{
    int master_sock;
    int cd;
    int max_cd;
    int port = atoi(argv[1]);
    int addrlen;
    char buffer[1024];

    struct sockaddr_in addr;
    struct sockaddr_in cli_addr;
    // Initializing Client_fds structure

    Client_info *connected_client = (Client_info *)malloc(sizeof(Client_info));

    connected_client->MAX_CLIENT = 5;
    connected_client->con_client = 0;
    for (int i = 0; i < connected_client->MAX_CLIENT; i++)
    {
        connected_client->client_fd[i] = 0;
    }
    // Intialize fdsets
    fd_set readfds;
    FD_ZERO(&readfds);
    master_sock = socket(AF_INET, SOCK_STREAM, 0);
    // check whether socket is created or not
    if (master_sock < 0)
    {
        printf("socket creation failed\n");
        exit(1);
    }
    else
        printf("master socket created successfuly\n");

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind the sock with the port;
    if (bind(master_sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        printf("Bind failed\n");
        exit(1);
    }
    else
        printf("Bind success\n");

    if (listen(master_sock, 3) < 0)
    {
        printf("error listening to sockets\n");
        exit(1);
    }
    else
        printf("listening\n");

    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(master_sock, &readfds);
        max_cd = master_sock;

        for (int i = 0; i < connected_client->MAX_CLIENT; i++)
        {
            if (connected_client->client_fd[i] > 0)
            {
                FD_SET(connected_client->client_fd[i], &readfds);
                if (connected_client->client_fd[i] > max_cd)
                {
                    max_cd = connected_client->client_fd[i];
                }
            }
        }

        // select system call
        int s = select(max_cd + 1, &readfds, NULL, NULL, NULL);

        if (FD_ISSET(master_sock, &readfds))
        {
            // check the no. of client connected if no. of client connected is greater than MAX client
            // reject it with the msg
            // else conneect it using accept system call and register the client to client_fd
            addrlen = sizeof(cli_addr);
            int confd = accept(master_sock, (struct sockaddr *)&cli_addr, &addrlen);
            if (confd < 0)
            {
                printf("error \n");
                exit(EXIT_FAILURE);
            }
            if (connected_client->con_client >= connected_client->MAX_CLIENT)
            {
                // open connection with new client
                // and send a msg that connection rejected
                bzero(buffer, 1024);
                sprintf(buffer, "%d", 0);
                write(confd, buffer, sizeof(buffer));
                close(confd);
            }
            else
            {
                bzero(buffer, 1024);
                sprintf(buffer, "%s", "Welcome to Online Bill Manager");
                write(confd, buffer, sizeof(buffer));
                bzero(buffer, 1024);
                for (int i = 0; i < 2; i++)
                {
                    if (connected_client->client_fd[i] == 0)
                    {
                        connected_client->client_fd[i] = confd;
                        break;
                    }
                }
                connected_client->con_client += 1;
            }
        }
        for (int i = 0; i < connected_client->MAX_CLIENT; i++)
        {
            if (FD_ISSET(connected_client->client_fd[i], &readfds))
            {
                int max_size = 1024;
                char buff[max_size];
                char com[20];
                char field[20];
                int _n_files;
                int newfd = connected_client->client_fd[i];

                bzero(buff, sizeof(buff));
                read(newfd, buff, sizeof(buff));
                sscanf(buff, "%[^' '] %[^\n]", com, field);
                bzero(buff, max_size);
                printf("%s %s\n", com, field);
                if (strcmp(com, "/exit") == 0)
                {
                    sprintf(buff, "%d", 2);
                    write(newfd, buff, sizeof(buff));
                    close(connected_client->client_fd[i]);
                    connected_client->client_fd[i] = 0;
                    connected_client->con_client -= 1;
                    continue;
                }

                else
                {
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
                            FILE *fp = fopen(file_rec->file[i], "w");
                            bzero(buff, 1024);
                            while (1)
                            {
                                read(newfd, buff, sizeof(buff));
                                if (strcmp(buff, "1") == 0)
                                {
                                    rewind(fp);
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
                        int set = 1;

                        for (int i = 0; i < _n_files; i++)
                        {
                            FILE *fp = fopen(file_rec->file[i], "r");
                            int check = check_validity(fp);
                            if (check == 0)
                            {
                                fclose(fp);
                                set = 0;
                                break;
                            }
                        }

                        if (set == 0)
                        {
                            sprintf(buff, "%d", 0);
                            write(newfd, buff, sizeof(buff));
                        }
                        else
                        {

                            // loading files to structure
                            int _n[2];
                            Record **record[2];
                            for (int i = 0; i < 2; i++)
                            {
                                FILE *fp = fopen(file_rec->file[i], "r");
                                _n[i] = NLINEX(fp);
                                rewind(fp);
                                record[i] = _load_file(fp, _n[i]);
                                fclose(fp);
                            }
                            if (strcmp(com, "/similarity") == 0)
                            {
                                sprintf(buff, "%d", 2);
                                write(newfd, buff, sizeof(buff));
                                bzero(buff, 1024);
                                read(newfd, buff, 1024);
                                bzero(buff, 1024);
                                for (int i = 0; i < _n[0]; i++)
                                {

                                    for (int j = 0; j < _n[1]; j++)
                                    {
                                        bzero(buff, 1024);
                                        if (record[0][i]->day == record[1][j]->day && record[0][i]->month == record[1][j]->month && record[0][i]->year == record[1][j]->year)
                                        {
                                            sprintf(buff, "%d.%d.%d %s %.2f | %d.%d.%d %s %.2f", record[0][i]->day, record[0][i]->month, record[0][i]->year, record[0][i]->item_name, record[0][i]->value, record[1][j]->day, record[1][j]->month, record[1][j]->year, record[1][j]->item_name, record[1][j]->value);
                                        }
                                        else if (strcmp(record[0][i]->item_name, record[1][j]->item_name) == 0)
                                        {
                                            sprintf(buff, "%d.%d.%d %s %.2f | %d.%d.%d %s %.2f", record[0][i]->day, record[0][i]->month, record[0][i]->year, record[0][i]->item_name, record[0][i]->value, record[1][j]->day, record[1][j]->month, record[1][j]->year, record[1][j]->item_name, record[1][j]->value);
                                        }
                                        else if (record[0][i]->value == record[1][j]->value)
                                        {
                                            sprintf(buff, "%d.%d.%d %s %.2f | %d.%d.%d %s %.2f", record[0][i]->day, record[0][i]->month, record[0][i]->year, record[0][i]->item_name, record[0][i]->value, record[1][j]->day, record[1][j]->month, record[1][j]->year, record[1][j]->item_name, record[1][j]->value);
                                        }
                                        write(newfd, buff, sizeof(buff));
                                        bzero(buff, 1024);
                                        read(newfd, buff, sizeof(buff));
                                    }
                                }
                                bzero(buff, 1024);
                                sprintf(buff, "%d", 1);
                                write(newfd, buff, sizeof(buff));
                            }
                            else
                            {
                                if (strcmp(com, "/sort") == 0)
                                {

                                    sort(record[0], _n[0], field);
                                    _record_to_file(record[0], _n[0]);
                                }
                                else if (strcmp(com, "/merge") == 0)
                                {
                                    _merge_to_file(record[0], record[1], _n[0], _n[1], field);
                                }

                                // performing operation
                                bzero(buff, 1024);
                                sprintf(buff, "%d", 1);
                                write(newfd, buff, sizeof(buff));
                                bzero(buff, 1024);
                                FILE *fp = fopen("output.txt", "r");
                                int n = NLINEX(fp);
                                int count = 0;
                                rewind(fp);
                                read(newfd, buff, sizeof(buff));
                                bzero(buff, 1024);
                                while (count < n)
                                {
                                    fgets(buff, 1024, fp);
                                    write(newfd, buff, sizeof(buff));
                                    bzero(buff, 1024);
                                    read(newfd, buff, sizeof(buff));
                                    if (strcmp(buff, "1") == 0)
                                    {
                                        bzero(buff, sizeof(buff));
                                        count++;
                                        continue;
                                    }
                                }
                                sprintf(buff, "%d", 1);
                                write(newfd, buff, sizeof(buff));
                            }
                        }
                    }
                }
            }
        }
    }
}
