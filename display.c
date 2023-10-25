#include "hardware.h"
#include <ncurses.h>
#include <time.h>
void updateDisplay(unsigned short instruction[])
{
    /*  clear();
      initscr();
      erase();*/
    for (int i = 0; i < HEIGHT; i++)
    {
        puts("");
        for (int j = 0; j < WIDTH; j++)
            printf(" %d ", display[i][j]);
    }
    /* for (int i = 0; i < WIDTH; i++)
         for (int j = 0; j < HEIGHT; j++)
         {
             if (display[i][j] == OFF)
                 mvaddch(i, j, ' ');

             else
                 // black box
                 mvaddch(i, j, ' ' | A_REVERSE);
         }*/

    /* printw("\n%d\n %d \n %d %d %d %d %d\n   pc = %d \n", delay_timer,
            I, X, Y, N, NN, NNN, program_counter);*/
    /* Refresh the screen. */
    refresh();
    sleep(1);
    endwin();
}

void clearScreen()
{
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            display[i][j] = OFF;
}
