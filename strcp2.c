#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/*
bool strcpy2(char** input, char* source, int index)
{

    int count = 0;
    char ch;
    while((ch = source[count]) != '\0')
    {
        input[index + count] = ch;
        count++;
    }

    return 0;
}
*/

int main()
{

    char* path = "/lol/test";
    char* directory = "test2";

    char subdir_path[strlen(path) + strlen(directory) + 2];          
            
    strcat(subdir_path, path);
    subdir_path[strlen(path)] = '/';
    strcat(subdir_path, directory);

    printf("%s\n", subdir_path);
    return 0;
}