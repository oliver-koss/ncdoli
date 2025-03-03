#include <sys/ioctl.h>
#include <stdio.h>

int main (void)
{
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

    printf ("lines %d\n", w.ws_row);
    printf ("columns %d\n", w.ws_col);

    printf("xpixel: %d\n", w.ws_xpixel);
    printf("ypixel: %d\n", w.ws_ypixel);

    return 0;
}