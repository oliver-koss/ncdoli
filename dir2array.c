#include <stdio.h>
//#include <sys/ioctl.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <curses.h>
#include <menu.h>

#include <sys/ioctl.h> // windows size

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
    struct list_index_* prev;
} list_index;

typedef struct menu_
{
    ITEM** item;
    list_index** files;
} menu_struct;

menu_struct* menu_from_index(list_index* index)
{
    int item_count = index->entrie_count;

    ITEM** item_list = malloc((item_count + 2) * sizeof(ITEM*));

    list_index** dir_list = malloc((item_count + 1) * sizeof(list_index*));

//    for (int i = 0; i<count; i++)

    dir_array* current = index->next_entrie;

    item_list[0] = new_item("..", "");
    dir_list[0] = index->prev;

    int count = 1;
    while (current != NULL)
    {
        char* name = current->dir->d_name;
    
        if (current->dir->d_type == DT_DIR)
        {

            item_list[count] = new_item(name, "DIR");
        } else {
            item_list[count] = new_item(name, "");
        }

        dir_list[count] = current->next_file;

        current = current->next_entrie;
        count++;
    }
    item_list[count] = '\0';

//    MENU* menu = new_menu(item_list);

    menu_struct* menu = malloc(sizeof(menu_struct));

    menu->item = item_list;
    menu->files = dir_list;

    return menu;

}


void quit(menu_struct* menu, MENU* me)
{

  unpost_menu(me);
  free_menu(me);

  int i = 0;
  ITEM* current;
  while((current = menu->item[i]) != '\0')
  {

    free_item(current);
    i++;
  }

  free(menu->item);

  free(menu->files);
  free(menu);
}

void screen_print()
{
    int row,col;

    getmaxyx(stdscr,row,col);

    mvprintw(row-3, 0, "ncdoli - lollololol");
    mvprintw(row-2,0,"This screen has %d rows and %d columns\n",row,col);
}

int menu(list_index* index)
{

  ITEM **it;
  MENU *me;

  menu_struct* menu;

  initscr();
//  atexit(quit(menu, me));

  clear();
  noecho();
  curs_set(0);
  cbreak();
  nl();
  keypad(stdscr, TRUE);


  menu = menu_from_index(index);

  me = new_menu(menu->item);

  post_menu(me);	

  screen_print();

  refresh();

  int ch;
  while((ch=getch()) != KEY_F(1))
  {
    switch(ch)
    {
      case KEY_DOWN:
        menu_driver(me, REQ_DOWN_ITEM);
        break;
      case KEY_UP:
        menu_driver(me, REQ_UP_ITEM);
        break;
      case 0xA: /* Return- bzw. Enter-Taste -> ASCII-Code */
        if (menu->files[item_index(current_item(me))] != NULL)
        {
            list_index* subdir = menu->files[item_index(current_item(me))];
            unpost_menu(me);
            quit(menu, me);

            menu = menu_from_index(subdir);
            me = new_menu(menu->item);

            post_menu(me);

            screen_print();

            refresh();
            
            break;
        }
    }
  } 
  return (0);  
}

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

    printf(RED "DEBUGG: %s: %i entries\n" RESET, input->path, input->entrie_count);
    printf(BLUE "Previous: %s\n" RESET, input->prev->path);

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



    list_index* output = malloc(sizeof(list_index));
    if (output == NULL)
    {
        return NULL;
    }


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
            sub_list->next_file->prev = output; // TODO
            
            
        }

    entrie_count++;

    }




//    printf("Path: %s\n", path);
    strcpy(output->path, path);

    output->entrie_count = entrie_count;
    output->next_entrie = sub_list;

//    closedir(dir_handle);
    return output;
}


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

//    struct winsize w;
//    ioctl(0, TIOCGWINSZ, &w);

//    printf("windows size: %ix%i\n", w.ws_xpixel, w.ws_ypixel);

    list_index* dir_list = read_dir(path);
//    print_list(dir_list);

    menu(dir_list);

    free_list(dir_list);



    return 0;
}