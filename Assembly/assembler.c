//
// Created by nelio on 15/09/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct instruction{
    char label[2], op_code[4], end[5];
}instruction;

typedef struct ref{
    char label[2];
    int end;
}ref;

//Programa
instruction program[250];
int pc;
//tabela de referencia
ref ref_table[250];
int ref_pc;

void tok_instruction(char *buffer);
void save_inst(char **buf, int t);
void save_ref(char **buf, int t);
void str_clean(char *str);

char* code()
{
    return "a";
}//

void write_on_file()
{
    int i;
    int tam_prog = pc;
    pc = 0;

    for(int i=0; i<tam_prog-1; i++)
    {
        printf("%s %s %s\n", program[i].label, program[i].op_code, program[i].end);
    }//for
}//write_on_file()

int main(int argc, char *argv[])
{
    FILE *f, *g;
    char buffer[50];

    if(argc != 3)
    {
        puts("Informe o arquivo de entrada e o arquivo de saida!");
        return 1;
    }//if

    f = fopen(argv[1], "r");
    g = fopen(argv[2], "w");
    if(f == NULL)
    {
        puts("Erro ao abrir arquivo!");
        return 1;
    }else
    {
        //Primeira Passada
        while(fgets(buffer, sizeof(buffer), f))
        {
            tok_instruction(buffer);
        }//while

        //Segunda Passada
        write_on_file();
    }//else

}//main()

void save_inst(char **buf, int t)
{
    switch (t){
        case 0: return; break;
        case 1: strcpy(program[pc].op_code, buf[0]); break;
        case 2:
            if(strlen(buf[0])==3)
            {
                strcpy(program[pc].op_code, buf[0]);
                strcpy(program[pc].end, buf[1]);
            }else
            {
                strcpy(program[pc].label, buf[0]);
                strcpy(program[pc].op_code, buf[1]);
            }//else
            break;
        case 3:
            strcpy(program[pc].label, buf[0]);
            strcpy(program[pc].op_code, buf[1]);
            strcpy(program[pc].end, buf[2]);
            break;
        default: break;
    }//switch
}//save_inst()

void save_ref(char **buf, int t)
{
    switch (t)
    {
        case 0: return; break;
        case 1: return; break;
        case 2:
            if(strlen(buf[0])==1)
            {
                strcpy(ref_table[ref_pc].label, buf[0]);
                ref_table[ref_pc].end = pc;
                ref_pc++;
            }//if
            break;
        case 3:
            strcpy(ref_table[ref_pc].label, buf[0]);
            ref_table[ref_pc].end = pc;
            ref_pc++;
            break;
        default: break;
    }//switch
}//save_ref()

void tok_instruction(char *buffer)
{
    int i, cnt=0;
    char **inst = (char **) malloc(sizeof(char*)*3);
    char *label;
    char *op_code;
    char *operand;

    str_clean(buffer);

    inst[0] = strtok(buffer, " ");
    inst[1] = strtok(NULL, " ");
    inst[2] = strtok(NULL, " ");

    for(i=0; i<3; i++)
    {
        if(inst[i] == NULL) break;
        else cnt++;
    }//for

    //printf("%d\n", cnt);
    save_inst(inst, cnt);
    save_ref(inst, cnt);

    free(inst);
    pc++;
}//tol_instruction()

void str_clean(char *str)
{
    int i=0;
    while(str[i]!='\0')
    {
        if(str[i]=='\n') str[i] = '\0';
        i++;

    }//while
}//str_clean()