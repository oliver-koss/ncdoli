#include <stdio.h>
//#include <sys/ioctl.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <curses.h>
#include <menu.h>

#define RESET "\x1b[0m"
#define BLUE "\x1b[94m"
#define MAGENTA "\x1b[35m"
#define RED "\e[0;31m"

typedef struct dir_array_
{
    struct dirent* dir;
    struct list_index_* next_file;
    struct dir_array_* next_entrie;
} dir_array;

typedef struct list_index_
{
    char path[100];
    int entrie_count;
    dir_array* next_entrie;
} list_index;

void free_list(list_index* input)
{
    dir_array* current = input->next_entrie;
    free(input);

    while (current != NULL)
    {
        if (current->dir->d_type == DT_DIR)
        {
            free_list(current->next_file);
        }

        dir_array* next = current->next_entrie;
        free(current);
        current = next;
    }
}

dir_array* add_to_list(dir_array* prev, struct dirent* dir)
{
    dir_array* output = malloc(sizeof(dir_array));
    if (output == NULL)
    {
        return NULL;
    }


    output->dir = dir;
    output->next_file = NULL;
    output->next_entrie = prev;

    return output;
}


void print_list(list_index* input)
{
    dir_array* current = input->next_entrie;

//    printf("Debugg1: %p\n", input->path);

    printf(RED "DEBUGG: %s: %i entries\n" RESET, input->path, input->entrie_count);

    while(current != NULL)
    {
        printf("%s/%s", input->path, current->dir->d_name);
        if (current->dir->d_type == DT_DIR)
        {
            printf("/\n");

            list_index* next = current->next_file;
            print_list(next);


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

    int entrie_count = 0;
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
//            char* subdir = malloc(sizeof(path) + sizeof(sub_list->dir->d_name) + 2);

            char subdir[strlen(path) + strlen(sub_list->dir->d_name) + 2] = {};
/*
            if (subdir == NULL)
                {   
                    printf("ERROR: Cannot allocate memory\n");
                    return NULL;
                }
*/

            strcpy(subdir, path);
            if (subdir[strlen(path) - 1] != '/')
            {
                subdir[strlen(path)] = '/';
            }
            strcat(subdir, sub_list->dir->d_name);

            sub_list->next_file = read_dir(subdir);

            
        }

    entrie_count++;

    }


    list_index* output = malloc(sizeof(list_index));
    if (output == NULL)
    {
        return NULL;
    }

//    printf("Path: %s\n", path);
    strcpy(output->path, path);

    output->entrie_count = entrie_count;
    output->next_entrie = sub_list;

//    closedir(dir_handle);
    return output;
}

/*
int print_menu(list_index* dir_list)
{

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);


}
*/

int main(int argc, char *argv[])
{
    char* path;

    if (argc > 2) {
        printf("Wrong number of input parameters!\n");
        return 1;
    } else if (argc == 2) {
        path = argv[1];
    } else {
        path = ".";
    }



    list_index* dir_list = read_dir(path);
    print_list(dir_list);
    free_list(dir_list);



    return 0;
}