#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TAM_INST_MEM 1024
#define TAM_DATA_MEM 256

#define getbit(by, pos) ((by>>pos)&0x01)

typedef struct {
    unsigned char first_byte, second_byte;
}instruction;

typedef struct {
    char first_byte[8], second_byte[8];
}inst;

instruction     program[TAM_INST_MEM];  //instruction memory
inst            prog[TAM_INST_MEM];
int   memory [TAM_DATA_MEM]; //data memory

//Funções auxiliares
void byte2bits(char c, char *s);        //conversão de char para binario

//funções do processador
void print_memory_contents()
{
    int i;
    for(i=0; i<TAM_DATA_MEM; i++)
    {
        printf("%d: '%u'\n", i, memory[i]);
    }//for
}//print_memory_contents()

int run_program(int num_bytes)
{
    int pc = 0;
    int ac = 0;
    int in, out;
    unsigned char reg[16], fb, sb;

    while(pc <= (num_bytes / 2))
    {
        fb = program[pc].first_byte;
        sb = program[pc].second_byte;
        pc++;

        switch (fb >> 4)
        {
            case 1: ac = memory[sb]; break;
            case 2: memory[sb] = ac; break;
            case 3: ac = ac + memory[sb]; break;
            case 4: ac = ac - memory[sb]; break;
            case 5: scanf("%d", &memory[sb]); break;
            case 6: printf("%d", memory[sb]); break;
            case 7: pc = memory[sb]; break;
            case 8: if(ac>0) pc = memory[sb]; break;
            case 9: if(ac<0) pc = memory[sb]; break;
            case 10: if(ac==0) pc = memory[sb]; break;
            case 11: return 0; break;
            default: return -1;
        }//switch

        /*switch (fb >> 4)
        {
            case 0: reg[fb & 0x0f] = memory[sb]; break;
            case 1: memory[sb] = reg[fb & 0x0f]; break;
            case 2: memory[reg[fb & 0x0f]] = reg[sb >> 4]; break;
            case 3: reg[fb & 0x0f] = sb; break;
            case 4: reg[fb & 0x0f] += reg[sb >> 4]; break;
            case 5: reg[fb & 0x0f] -= reg[sb >> 4]; break;
            case 6: pc += sb; break;
            default: return -1;
        }//switch*/
    }//while

    return 0;
}//run_program()

int run_program2(int num_bytes)
{
    int pc = 0;
    int ac = 0;
    unsigned char reg[16], fb, sb;
    char fb1[8], sb1[8];
    char aux1[8], aux2[8];

    while(pc < (num_bytes / 2))
    {
        fb = program[pc].first_byte;
        sb = program[pc].second_byte;

        byte2bits(program[pc].first_byte, fb1);
        byte2bits(program[pc].second_byte, sb1);

        byte2bits(fb & 0x0f, aux1);
        pc++;

        switch (fb >> 4)
        {
            case 1: ac = memory[sb]; break;
            case 2: memory[sb] = ac; break;
            case 3: ac = ac + memory[sb]; break;
            case 4: ac = ac - memory[sb]; break;
            case 5: scanf("%d", &memory[sb]); setbuf(stdin, NULL); break;
            case 6: printf("%d\n", memory[sb]); break;
            case 7: pc = memory[sb]; break;
            case 8: if(ac>0) pc = memory[sb]; break;
            case 9: if(ac<0) pc = memory[sb]; break;
            case 10: if(ac==0) pc = memory[sb]; break;
            case 11: return 0; break;
            case 12: memory[ac] = sb; break;    //store IMM
            case 13: ac = sb; break;            //load IMM
            default: return -1;
        }//switch

        /*switch (fb >> 4)
        {
            case 0: reg[fb & 0x0f] = memory[sb]; break;             //registrador no segundo nible do primeiro byte recebe conteúdo da mem.
            case 1: memory[sb] = reg[fb & 0x0f]; break;             //memoria recebe conteúdo do registrador.
            case 2: memory[reg[fb & 0x0f]] = reg[sb >> 4]; break;   //endereçamento indireto. Carrega conteúdo do reg na mem.
            case 3: reg[fb & 0x0f] = sb; break;                     //carrega imediato.
            case 4: reg[fb & 0x0f] += reg[sb >> 4]; break;          //soma conteúdo do reg em sb.
            case 5: reg[fb & 0x0f] -= reg[sb >> 4]; break;          //subtrai o valor do rem em sb do reg em fb.
            case 6: pc += sb; break;                                //jmp.
            default: return -1;
        }//switch*/
    }//while

    return 0;
}//run_program()

int main(int argc, char *argv[]) {

    FILE *f = fopen(argv[1], "rb");

    if((argc != 2) || (f == NULL) )
    {
        return -1;
    }//if

    //int a = sizeof(prog);

    int t = fread(program, 1, sizeof(program), f);
    //printf("%d\n", t);

    //Lê o arquivo inteiro no vetor de instruções. Não passa no if se a
    //leitura tiver tamanho 0.
    if( run_program2(t)==0 )
    {
        //print_memory_contents();
        return 0;
    }else return -1;

    return 0;
}//main()

void byte2bits(char c, char *s)
{
    int i=7;
    while(i>=0)
    {
        *s++ = '0' + getbit(c, i--);
    }//while
    *s = 0;
}//byte2bits()