#include <stdio.h>
#include <dirent.h>

int main() 
{

    DIR* directory = opendir(".");
    if (directory == NULL)
    {
        return 1;
    }

    struct dirent* dir;

    /*
    while((dir = readdir(directory)) != NULL)
    {

    }
    */

    dir = readdir(directory);

    closedir(directory);

    printf("name: %s\n", dir->d_name);


    return 0;
}