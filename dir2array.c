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
    struct dir_array_* next_file;
    struct dir_array_* next_entrie;
} dir_array;


dir_array* add_to_array(dir_array* prev, struct dirent* dir)
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


void print_list(dir_array* input)
{
    dir_array* current = input;

    while(current != NULL)
    {
        printf("%s", current->dir->d_name);
        if (current->dir->d_type == DT_DIR)
        {
            printf("/");
        }
        printf("\n");

        if (current->dir->d_type == DT_DIR)
        {
            printf("\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\n");
            print_list(current->next_file);
            printf("----------\n\n");
        }

        current = current->next_entrie;
    }
}

dir_array read_dir(char* path)
{
    DIR* dir_handle = opendir(path);
    if (dir_handle == NULL)
    {
        printf("Cannot open directory");
        return 1;
    }

    dir_array output = NULL;

    struct dirent* dir;
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
    while((dir = readdir(d)) != NULL)
    {
        if (dir->d_name[1] == '\0' && dir->d_name[0] == '.' || dir->d_name[1] == '.')
        {
            //printf("continue");
            continue;
        }
        
        dir_content = add_to_array(dir_content, dir);
//          dir_array* dir_content = {dir, NULL, dir_content};


        if (dir_content->dir->d_type == DT_DIR)
        {
            dir_array* buffer_list = NULL;


//            char* subdir_path = malloc(sizeof(path) + sizeof(dir_content->dir->d_name) + 2);
            char subdir_path[strlen(path) + strlen(dir_content->dir->d_name) + 2];
            strcat(subdir_path, path);

            /*
            printf("null '%s'\n", subdir_path);
            if (subdir_path[strlen(path)] != '/')
            {
                subdir_path[strlen(path)] = '/';
            }
*/
            strcat(subdir_path, dir_content->dir->d_name);




            //printf("subdir_path: %s\n", subdir_path);
            //printf("path: %s\n", path);


            DIR* subdir = opendir(subdir_path);
            if (subdir == NULL)
            {
                printf("ERROR: Cannot open directory '%s'\n", subdir_path);
                continue;
            }

            struct dirent* dir2;
            while ((dir2 = readdir(subdir)) != NULL)
            {
                buffer_list  = add_to_array(buffer_list, dir2);
                printf("name: %s\n", dir2->d_name);
            }

            dir_content->next_file = buffer_list;
        }


    }

    print_list(dir_content);

    return 0;
}