
#include <dirent.h> 
#include <stdio.h> 
#include <ncurses.h>
#include <sys/ioctl.h>
#include <string.h>

#define WIDTH 30
#define HEIGHT 10 


int startx = 0;
int starty = 0;

char** dir_to_array(char* directory)
{
    char* output[100][100];
    DIR *d;
    struct dirent *dir;
    d = opendir(directory);
    if (!d) {
        printf("ERROR: Can not open directory");
        return 0;
    }
    int output_len = 0;
    char* ch;
    while((dir = readdir(d)) != NULL)
    {

        /*
        while(true)
        {
            ch = dir->d_name[stringlength];
            printf("Debugg: %c\n", ch);

            
            output[output_len][stringlength] = ch;
            if (ch == '\0') {
                break;
            }
            stringlength++;
        }
        output_len++;
        */

        ch = dir->d_name;
        printf("Debugg: %s: %i\n", ch, output_len);

        strcpy(output[output_len], ch);
        output_len++;
    }
        
    return output;
}

/*
int print_dir(void) {
  DIR *d;
  struct dirent *dir;
  d = opendir(".");
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      printf("%s\n", dir->d_name);
    }
    closedir(d);
  }
  return(0);
}
*/


char *choices[] = { 
			"Choice 1",
			"Choice 2",
			"Choice 3",
			"Choice 4",
			"Exit",
			"Exit2"
		  };
int n_choices = sizeof(choices) / sizeof(char *);
void print_menu(WINDOW *menu_win, int highlight);

int choice()
{
	WINDOW *menu_win;
	int highlight = 1;
	int choice = 0;
	int c;

	initscr();
	clear();
	noecho();
	cbreak();	/* Line buffering disabled. pass on everything */
	startx = (80 - WIDTH) / 2;
	starty = (24 - HEIGHT) / 2;
		
	menu_win = newwin(HEIGHT, WIDTH, starty, startx);
	keypad(menu_win, TRUE);
	mvprintw(0, 0, "Use arrow keys to go up and down, Press enter to select a choice");
	refresh();
	print_menu(menu_win, highlight);
	while(1)
	{	c = wgetch(menu_win);
		switch(c)
		{	case KEY_UP:
				if(highlight == 1)
					highlight = n_choices;
				else
					--highlight;
				break;
			case KEY_DOWN:
				if(highlight == n_choices)
					highlight = 1;
				else 
					++highlight;
				break;
			case 10:
				choice = highlight;
				break;
			default:
				mvprintw(24, 0, "Charcter pressed is = %3d Hopefully it can be printed as '%c'", c, c);
				refresh();
				break;
		}
		print_menu(menu_win, highlight);
		if(choice != 0)	/* User did a choice come out of the infinite loop */
			break;
	}	
	mvprintw(23, 0, "You chose choice %d with choice string %s\n", choice, choices[choice - 1]);
	clrtoeol();
	refresh();
	endwin();
	return 0;
}


void print_menu(WINDOW *menu_win, int highlight)
{
	int x, y, i;	

	x = 2;
	y = 2;
	box(menu_win, 0, 0);
	for(i = 0; i < n_choices; ++i)
	{	if(highlight == i + 1) /* High light the present choice */
		{	wattron(menu_win, A_REVERSE); 
			mvwprintw(menu_win, y, x, "%s", choices[i]);
			wattroff(menu_win, A_REVERSE);
		}
		else
			mvwprintw(menu_win, y, x, "%s", choices[i]);
		++y;
	}
	wrefresh(menu_win);
}

void main()
{
    char* array_test = dir_to_array(".");
    for (int i = 0; i<3; i++)
    {
        printf("lol: %s\n", array_test[i]);
    }
	choice();
}
