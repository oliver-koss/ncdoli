#include <stdio.h>
//#include <sys/ioctl.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define RESET "\x1b[0m"
#define BLUE "\x1b[94m"
#define MAGENTA "\x1b[35m"

typedef struct dir_array_
{
    struct dirent* dir;
    struct list_index_* next_file;
    struct dir_array_* next_entrie;
} dir_array;

typedef struct list_index_
{
    char** path;
    dir_array* next_entrie;
} list_index;


dir_array* add_to_list(dir_array* prev, struct dirent* dir)
{
    dir_array* output = malloc(sizeof(dir_array));
//    char* name = malloc(sizeof(strlen(dir->d_name)));

//    strcpy(name, dir->d_name);

//    struct dir_array* output;

    output->dir = dir;
    output->next_file = NULL;
    output->next_entrie = prev;

    return output;
}


void print_list(list_index* input, char* prefix)
{
    dir_array* current = input->next_entrie;

    while(current != NULL)
    {
        printf("%s%s", prefix, current->dir->d_name);
        if (current->dir->d_type == DT_DIR)
        {
            printf("/\n");

            list_index* next = current->next_file;
            print_list(next, "");


        } else {
            printf("\n");
        }

        current = current->next_entrie;
    }
}

list_index* read_dir(char* path)
{
    DIR* dir_handle = opendir(path);
    if (dir_handle == NULL)
    {
        printf("Cannot open directory");
        return NULL;
    }

    dir_array* sub_list = NULL;

    struct dirent* dir;
    while((dir = readdir(dir_handle)) != NULL)
    {
        if (dir->d_name[1] == '\0' && dir->d_name[0] == '.' || dir->d_name[1] == '.')
        {
            continue;
        }

        sub_list = add_to_list(sub_list, dir);


        if (sub_list->dir->d_type == DT_DIR)
        {
            char* subdir = malloc(sizeof(path) + sizeof(sub_list->dir->d_name) + 2);
            if (subdir == NULL)
                {   
                    printf("ERROR: Cannot allocate memory\n");
                    return NULL;
                }
            strcpy(subdir, path);
            if (subdir[strlen(path) - 1] != '/')
            {
                subdir[strlen(path)] = '/';
            }
            strcat(subdir, sub_list->dir->d_name);

            sub_list->next_file = read_dir(subdir);
        }


    }

    list_index* output = malloc(sizeof(list_index));
    output->path = &path;
    output->next_entrie = sub_list;

    return output;
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

    dir_array* dir_content = NULL;

    struct dirent* dir;

    /*
    while((dir = readdir(d)) != NULL)
    {

    }
    */

    list_index* dir_list = read_dir(path);

 //   dir_array* current = dir_list;

    /*
    while(current != NULL)
    {
        

        current = current->next_entrie;
    }
*/

    print_list(dir_list, "");

    return 0;
}