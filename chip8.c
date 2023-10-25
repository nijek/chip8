#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "display.h"
#include <ncurses.h>
#include <unistd.h>
#include <string.h>

unsigned short program_counter = RESERVED_MEMORY;
unsigned short stack_index = 0;
unsigned short index_register = 0;
unsigned short processor_clock = 0;
unsigned short delay_timer = 0;
unsigned short sound_timer = 0;

unsigned char memory[MEMSIZE] = {0};
unsigned short program_stack[STACK_SIZE];
unsigned char display[WIDTH][HEIGHT] = {0};
unsigned char registers[REGISTER_NUMBER] = {0};

void exit_error(char *errorMessage)
{
    erase();
    endwin();
    puts(errorMessage);
    exit(EXIT_FAILURE);
}

void createMatrix()
{

    clearScreen();
}

int fetch(unsigned short instruction[])
{

    if (program_counter >= MEMSIZE)
    {
        return ERROR;
    }

    I = memory[program_counter] >> 4;
    X = memory[program_counter] & 0xf;
    Y = memory[program_counter + 1] >> 4;
    N = memory[program_counter + 1] & 0xf;

    NN = memory[program_counter + 1];
    NNN = (X << 8) ^ NN;
    program_counter += 2;
    return 0;
}

void create_sprite(unsigned char sprite[], int memoryIndex)
{
    int i;
    unsigned short sprite_data = memory[memoryIndex];
    printf("sprite_data = %x\n", sprite_data);
    for (i = 0; i < 8; i++)
        sprite[i] = 0;

    i = 7;
    while (sprite_data > 0)
    {
        sprite[i] = sprite_data % 2;
        sprite_data = sprite_data >> 1;
        i--;
    }
    for (i = 0; i < 8; i++)
        printf(" %d ", sprite[i]);
}

int execute(unsigned short instruction[])
{

    switch (I)
    {
    case 0:
        if (NNN == 0)
            program_counter = RESERVED_MEMORY;

        else
            clearScreen();
        break;
    case 1:
        updateDisplay(instruction);
        program_counter = NNN;
        break;
    case 2:
        stack_index++;
        if (stack_index == STACK_SIZE)
        {
            exit_error("Stack Overflow");
        }
        program_stack[stack_index] = program_counter;
        program_counter = NNN;
        break;
    case 6:
        registers[X] = NN;
        break;
    case 7:
        registers[X] += NN;
        break;
    case 0xA:
        index_register = NNN;
        break;

    /* It will draw an N pixels tall sprite from the memory location
    that the I index register is holding to the screen,
    at the horizontal X coordinate in VX and the Y coordinate in VY.*/
    case 0xD:
        /*x coordinate is the VX value modulo 64*/

        printf("index_register = %x memory = %x\n", index_register, memory[index_register]);

        unsigned short x = registers[X] & 63;
        unsigned short y = registers[Y] & 31;
        printf("x = %d y = %d\n", x, y);
        /*sets VF to 0*/
        registers[0xF] = 0;
        int i, j;
        unsigned char sprite[8];
        create_sprite(sprite, index_register);
        for (i = 0; i < 8; i++)
            printf(" %d ", sprite[i]);
        i = 0;

        while (i < N && y < HEIGHT)
        {

            j = 0;
            x = registers[X] & 63;
            while (x < WIDTH && j < 8)
            {
                if (display[y][x] == ON && sprite[j] == ON)
                {
                    display[y][x] = OFF;
                    registers[0xf] = 1;
                }
                else if (sprite[j] == ON && display[x][y] == OFF)
                {
                    display[y][x] = ON;
                }
                x++;
                j++;
                //  printf("x = %d, y = %d, i = %d, j = %d\n", x, y, i, j);
            }
            y++;
            i++;
        }

        updateDisplay(instruction);
        break;

    default:
        puts("\n\n\nDEFAULT \n\n\n");
        break;
    }
    return 0;
}

void printInstruction(unsigned short instruction[])
{
    erase();
    printf("\nI: %02x\nX: %02x\nY: %02x\nN: %02x\nNN: %02x\nNNN: %02x\n",
           I, X, Y, N, NN, NNN);
    printf("program coutner inside instruction display = %d\n", program_counter);
    refresh();
}

int main(int argc, char **argv)
{
    int delayTimeAccumulator = 0;
    int processorTimeAccumulator = 0;

    /* Start curses. Also sets LINES and COLS. */
    // createMatrix();
    // initscr();

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
        // printf("ROM size: %d\n", file_size);
        rewind(file);

        // return error if file is too large or cannot be found
        /*if (file_size > MAX_FILE_SIZE)
        {
            printf("File is too large to load.\n");
            return -1;
        }*/

        // read program into memory
        unsigned char program_code[MEMSIZE];
        fread(program_code, 1, file_size, file);
        /*first 512 bits are reserved*/
        for (int i = 0; i < file_size; i++)
            memory[i + RESERVED_MEMORY] = program_code[i];
        fclose(file);
        for (int i = RESERVED_MEMORY; i < file_size + RESERVED_MEMORY; i += 2)
        {
            printf("posição %x: ", i);
            printf("%02x %02x\n", memory[i], memory[i + 1]);
        }
        /*exit_error(".");
        return 0;*/
    }
    createMatrix();

    do
    {

        // divided in 4 nibbles
        // puts("start");
        unsigned short instruction[6];

        if (fetch(instruction) == ERROR)
            exit_error("Memory Error");

        printInstruction(instruction);
        execute(instruction);
        // printf("program counter = %d\n", program_counter);

        // sleep(1);
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
            // invertColors();sleep
            // updateDisplay(instruction);
            processorTimeAccumulator = 0;
        }
    } while (TRUE); // c != 'Q' && c != 'q');
    /* Done. Return the terminal back to its normal state. */
    endwin();
    return 0;
}