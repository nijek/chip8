#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "display.h"
#include <ncurses.h>
#include <unistd.h>
#include <string.h>

unsigned short program_counter = 0;
unsigned short stack_index = 0;
unsigned short index_register = 0;
unsigned short processor_clock = 0;
unsigned short delay_timer = 0;
unsigned short sound_timer = 0;

unsigned char memory[MEMSIZE] = {0};
unsigned short program_stack[STACK_SIZE];
unsigned char display[HEIGHT][WIDTH] = {0};
unsigned char registers[REGISTER_NUMBER] = {0};

void exit_error(char *errorMessage)
{
    erase();
    endwin();
    puts(errorMessage);
    exit(EXIT_FAILURE);
}

void *delayTimer(void *vargp)
{
    // sleep((float) 1.0 / (float)DELAY_TIMER);
    sleep(2);
    if (delay_timer == 90)
        delay_timer = 0;
    else
        delay_timer++;
    return 0;
}

void createMatrix()
{
    int c = 1;
    for (int i = 0; i < HEIGHT; i++, c++)
        for (int j = 0; j < WIDTH; j++, c++)
            if (c % 2 == 0)
                display[i][j] = ON;
}

void invertColors()
{
    sleep(1);
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
        {
            if (display[i][j] == ON)
                display[i][j] = OFF;
            else
                display[i][j] = ON;
        }
}

int fetch(unsigned short instruction[])
{
    program_counter += 1;
    if (program_counter >= MEMSIZE)
    {
        return ERROR;
    }
    /* memory[0] = 108; // 0110 1100
     memory[1] = 229; //1110 0101 */

    instruction[I] = memory[program_counter] >> 4;  // 6
    instruction[X] = memory[program_counter] & 0xf; // 12

    program_counter += 1;

    instruction[Y] = memory[program_counter] >> 4;  // 14
    instruction[N] = memory[program_counter] & 0xf; // 5

    instruction[NN] = memory[program_counter]; // 229
    instruction[NNN] = (instruction[X] << 8) ^ instruction[NN];

    return 0;
}

int execute(unsigned short instruction[])
{

    switch (instruction[I])
    {
    case 0:
        clearScreen();
        break;
    case 1:
        program_counter = instruction[NNN];
        break;
    case 2:
        stack_index++;
        if (stack_index == STACK_SIZE)
        {
            exit_error("Stack Overflow");
        }
        program_stack[stack_index] = program_counter;
        program_counter = instruction[NNN];
        break;
    case 6:
        registers[instruction[X]] = instruction[NN];
        break;
    case 7:
        registers[instruction[X]] += instruction[NN];
        break;
    case 0xA:
        index_register = instruction[NNN];
        break;
    case 0xD:
        updateDisplay(instruction);
        break;

    default:
        break;
    }
    return 0;
}

int main(int argc, char **argv)
{
    int delayTimeAccumulator = 0;
    int processorTimeAccumulator = 0;

    /* Start curses. Also sets LINES and COLS. */
    // createMatrix();
    initscr();

    /* Pass every character immediately, not buffered. */
    // cbreak();
    int file_size;
    FILE *file = fopen(argv[1], "rb");
    if (file == NULL)
        exit_error("File not found.");

    else
    {
        // find file size and load program
        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        printf("ROM size: %d\n", file_size);
        rewind(file);

        // return error if file is too large or cannot be found
        /*if (file_size > MAX_FILE_SIZE)
        {
            printf("File is too large to load.\n");
            return -1;
        }*/

        // read program into memory
        fread(memory, 1, file_size, file);
        fclose(file);
        /* for (int i = 0; i < MEMSIZE; i++)
             printf("%x ", memory[i]);
         exit_error(".");
         return 0;*/
    }

    do
    {

        // divided in 4 nibbles
        unsigned short instruction[6];

        if (fetch(instruction) == ERROR)
            exit_error("Memory Error");

        execute(instruction);

        usleep(100000);
        delayTimeAccumulator += 1;
        processorTimeAccumulator += 1;
        if (delayTimeAccumulator == 4)
        {
            delay_timer += 1;
            delayTimeAccumulator = 0;
        }
        if (delay_timer > 60)
            delay_timer = 0;
        if (processorTimeAccumulator == 2)
        {
            // invertColors();
            updateDisplay(instruction);
            processorTimeAccumulator = 0;
        }
    } while (TRUE); // c != 'Q' && c != 'q');
    /* Done. Return the terminal back to its normal state. */
    endwin();
    return 0;
}