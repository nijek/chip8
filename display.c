#include "hardware.h"
#include <ncurses.h>
void updateDisplay(unsigned short instruction[])
{
    /* Clear the screen. */
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

    printw("\n%d\n %d \n %d %d %d %d %d\n   pc = %d \n", delay_timer,
           instruction[0], instruction[1], instruction[2], instruction[3],
           instruction[4], instruction[5], program_counter);
    /* Refresh the screen. */
    refresh();
}

void clearScreen()
{
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            display[i][j] = OFF;
}
