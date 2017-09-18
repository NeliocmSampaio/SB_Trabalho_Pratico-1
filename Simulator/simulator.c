#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TAM_INST_MEM 1024
#define TAM_DATA_MEM 256

#define getbit(by, pos) ((by>>pos)&0x01)

typedef struct {
    unsigned char first_byte[2], second_byte[2];
}instruction;

typedef struct {
    char first_byte[8], second_byte[8];
}inst;

instruction     program[TAM_INST_MEM];  //instruction memory
inst            prog[TAM_INST_MEM];
int             memory [TAM_DATA_MEM];  //data memory

int tam_program;
int load_position;
//Funções auxiliares
void byte2bits(char c, char *s);        //conversão de char para binario
void print_str_tam(char *str, int n);   //imprime tantos caracteres a partir do endereco recebido.
void print_inst(int i);                 //imprime a instrucao na posicao i
void cpy_tam(char *dest, char *src, int t);

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

int run_program2(int program_size)
{
    int pc = 0;
    int ac = 0;
    int op_code;
    int ref;
    char op[3], rf[3];
    //unsigned char reg[16], fb, sb;
    //char fb1[8], sb1[8];
    //char aux1[8], aux2[8];

    while(pc < (program_size))
    {
        //print_inst(pc);

        cpy_tam(op, program[pc].first_byte, 2);
        cpy_tam(rf, program[pc].second_byte, 2);

        op_code = atoi(op);
        ref = atoi(rf);

        printf("%d %d\n", op_code, ref);

        pc++;

        /*switch (fb >> 4)
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
            default: return -1;*/
        //}//switch

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

void printProgram()
{
    int i;
    for(i=0; i<tam_program; i++)
    {
        print_str_tam(&(program[i].first_byte), 2);
        print_str_tam(&(program[i].second_byte), 2);
        //printf("%s", &(program[i].first_byte)/*, &(program[i].second_byte)*/);
        printf("\n");
    }

}//printProgram()

void readProgram(FILE *f)
{
    char buffer[7], *pointer, inst[7];
    while(fgets(buffer, sizeof(buffer), f) != NULL)
    {
        tam_program++;
        //printf("%s", buffer);
        pointer = strtok(buffer, " ");
        strcpy(inst, pointer);
        strcpy(&(program[load_position].first_byte), inst);
        //printf("%s", &(program[load_position].first_byte));
        pointer = strtok(NULL, " ");
        if(pointer != NULL)
        {
            strcpy(inst, pointer);
            strcpy(&(program[load_position].second_byte), inst);
            //printf("%s", &(program[load_position].second_byte));
        }else
        {
            strcpy(program[load_position].second_byte, "00");
            program[load_position].first_byte[1] = '0';
        }
        load_position++;
    }//while
}//readProgram()

int main(int argc, char *argv[]) {

    load_position = 0;
    tam_program = 0;

    FILE *f = fopen(argv[1], "r");

    if((argc != 2) || (f == NULL))
    {
        return -1;
    }//if

    readProgram(f);
    //printProgram();

    int t = tam_program;/* = fread(program, 1, sizeof(program), f);
    //printf("%d\n", t);*/

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

void print_str_tam(char *str, int n)
{
    int i;
    char a;
    for(i=0; i<n; i++)
    {
        a = str[i];
        printf("%c", a);
    }//for
}//print_str_tam()

void print_inst(int i)
{
    print_str_tam(&(program[i].first_byte), 2);
    printf(" ");
    print_str_tam(&(program[i].second_byte), 2);
    printf("\n");
}//print_inst()

void cpy_tam(char *dest, char *src, int t)
{
    int i;
    for (i=0; i<t; i++)
    {
        dest[i] = src[i];
    }//for
    src[i] = '\0';
}//cpy_tam()