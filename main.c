#include <stdio.h>

#define TAM_INST_MEM 1024
#define TAM_DATA_MEM 256

typedef struct {
    unsigned char first_byte, second_byte;
}instruction;

instruction program[TAM_INST_MEM];  //instruction memory
unsigned char memory [TAM_DATA_MEM]; //data memory

int run_program(int num_bytes)
{
    int pc = 0;
    unsigned char reg[16], fb, sb;

    while(pc <= (num_bytes / 2))
    {
        fb = program[pc].first_byte;
        sb = program[pc].second_byte;
        pc++;
        switch (fb >> 4)
        {
            case 0: reg[fb & 0x0f] = memory[sb]; break;
            case 1: memory[sb] = reg[fb & 0x0f]; break;
            case 2: memory[reg[fb & 0x0f]] = reg[sb >> 4]; break;
            case 3: reg[fb & 0x0f] = sb; break;
            case 4: reg[fb & 0x0f] += reg[sb >> 4]; break;
            case 5: reg[fb & 0x0f] -= reg[sb >> 4]; break;
            case 6: pc += sb; break;
            default: return -1;
        }//switch
    }//while

    return 0;
}//run_program()

int main(int argc, char *argv[]) {

    FILE *f;

    if((argc != 2) || (f = fopen(argv[1], "rb") == NULL) )
    {
        return -1;
    }//if

    if( run_program(fread(program, 1, sizeof(program), f) == 0))
    {
        //print_memory_contents();
        return 0;
    }else return -1;

    return 0;
}