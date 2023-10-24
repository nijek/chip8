#ifndef HARDWARE_DEFINED
#define HARDWARE_DEFINED

#define TRUE 1
#define FALSE 0
#define ERROR -1

/* display */
#define ON 1
#define OFF 0
#define HEIGHT 32
#define WIDTH 64
#define REGISTER_NUMBER 16

/*instructions:
I: the first nibble. Instruction type
X: The second nibble. Used to look up one of the 16 registers (VX) from V0 through VF.
Y: The third nibble. Also used to look up one of the 16 registers (VY) from V0 through VF.
N: The fourth nibble. A 4-bit number.
NN: The second byte (third and fourth nibbles). An 8-bit immediate number.
NNN: The second, third and fourth nibbles. A 12-bit immediate memory address
*/
#define I 0
#define X 1
#define Y 2
#define N 3
#define NN 4
#define NNN 5

/* Memory is 4096 bytes large */
#define MEMSIZE 4096
#define STACK_SIZE 16

/*frequencies in Hz*/
#define PROCESSOR_CLOCK 1
#define DELAY_TIMER 1

extern unsigned char memory[MEMSIZE];
extern unsigned short program_stack[STACK_SIZE];
extern unsigned char display[HEIGHT][WIDTH];
extern unsigned char registers[REGISTER_NUMBER];

/* index register and program counter can address all 4096 memory addresses
 (actually more since they are 16 bits in this implementation)*/
extern unsigned short program_counter;
extern unsigned short stack_index;
extern unsigned short index_register;
extern unsigned short processor_clock;
extern unsigned short delay_timer;
extern unsigned short sound_timer;
#endif