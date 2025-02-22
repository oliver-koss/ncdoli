#include <stdio.h>
//#include <sys/ioctl.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define RESET "\x1b[0m"
#define BLUE "\x1b[94m"
#define MAGENTA "\x1b[35m"

struct dir_array
{
    struct dirent* dir;
    struct dir_array* next_file;
    struct dir_array* next_entrie;
};

bool dir_to_array()
{

}

struct dir_array* add_to_array(struct dir_array* prev, struct dirent* dir)
{
    struct dir_array* output = malloc(sizeof(struct dir_array));
//    char* name = malloc(sizeof(strlen(dir->d_name)));

//    strcpy(name, dir->d_name);

//    struct dir_array* output;

    output->dir = dir;
    output->next_file = NULL;
    output->next_entrie = prev;

    return output;
}

void print_list(struct dir_array* input)
{
    struct dir_array* current;

    current = input;

    while(current != NULL)
    {
        printf("%s\n", current->dir->d_name);
        current = current->next_entrie;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Wrong number of input parameters!\n");
        return 1;
    }

    char* path = argv[1];

    DIR* d = opendir(path);
    if (d == NULL)
    {
        printf("Cannot open directory");
        return 1;
    }

    struct dir_array* dir_content = NULL;

    struct dirent* dir;
    while((dir = readdir(d)) != NULL)
    {
        dir_content = add_to_array(dir_content, dir);
        /*
        if (dir->d_type == 8)
        {
            printf(BLUE "%s\n" RESET, dir->d_name);
        } else if (dir->d_type == 4) {
            printf(MAGENTA "%s\n" RESET, dir->d_name);
        } else {
            printf("%s\n", dir->d_name);
        }
        */
    }

    print_list(dir_content);

    return 0;
}