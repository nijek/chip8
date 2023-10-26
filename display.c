#include "hardware.h"
#include <ncurses.h>
#include <unistd.h>
void updateDisplay()
{
    /*  clear();
      initscr();
      erase();*/
    printf("  0");
    for (int i = 0; i < 13; i++)
        printf("%6d ", i * 5 + 5);
    for (int i = 0; i < HEIGHT; i++)
    {
        printf("\n%2d |", i);
        for (int j = 0; j < WIDTH; j++)
        {
            if (j % 5 == 0 && j != 0)
                printf("| ");
            if (display[i][j] == 0)
                printf("*");
            else
                printf("X");
            if (j == WIDTH - 1)
                printf("|");
        }
    }
    // usleep(50000);
}

void updateDisplayNCurses()
{
    clear();
    initscr();
    erase();
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
        {
            if (display[i][j] == OFF)
                mvaddch(i, j, ' ');

            else
                // black box
                mvaddch(i, j, ' ' | A_REVERSE);
        }
    refresh();
    usleep(50000);
}
void clearScreen()
{
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            display[i][j] = OFF;
}
