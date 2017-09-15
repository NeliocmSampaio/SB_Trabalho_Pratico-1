//
// Created by nelio on 15/09/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct instruction{
    char label[1], op_code[4], end[5];
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
void save_ref(char **buf, int t);
void str_clean(char *str);

int search_ref_table(char *c)
{
    int i = 0;
    while(i<ref_pc)
    {
        //strcmp(ref_table[i].label, c)
        if(ref_table[i].label[0] == c[0])
        {
            return ref_table[i].end;
        }//if
        i++;
    }//while

    //puts("Referencia nao encontrada!");
    return 0;
}//search_ref_table()

char* code(char *op)
{
    if(!strcmp(op, "LAD"))
    {
        return "01";
    }else if(!strcmp(op, "SAD"))
    {
        return "02";
    }else if(!strcmp(op, "ADD"))
    {
        return "03";
    }else if(!strcmp(op, "SUB"))
    {
        return "04";
    }else if(!strcmp(op, "INP"))
    {
        return "05";
    }else if(!strcmp(op, "OUT"))
    {
        return "06";
    }else if(!strcmp(op, "JMP"))
    {
        return "07";
    }else if(!strcmp(op, "JGZ"))
    {
        return "08";
    }else if(!strcmp(op, "JLZ"))
    {
        return "09";
    }else if(!strcmp(op, "JZE"))
    {
        return "10";
    }else if(!strcmp(op, "HLT"))
    {
        return "11";
    }else return "00";
}//code()

char* refr(char *r)
{
    char *str = (char*) malloc(sizeof(str)*5);
    sprintf(str, "%2d", search_ref_table(r));

    int i=0;
    while(i<5 && str[i]!='\0')
    {
        if(str[i]==' ') str[i] = '0';
        i++;
    }//while

    return str;
}//refr()

void write_on_file(FILE *f)
{
    int i;
    int tam_prog = pc;
    char *str;
    pc = 0;

    for(int i=0; i<tam_prog; i++)
    {
        fprintf(f, "%s ", code(program[i].op_code));
        str = refr(program[i].end);
        fprintf(f, "%s\n", str);
        free(str);
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
        write_on_file(g);
    }//else

    fclose(f);
    fclose(g);
}//main()

int save_inst(char **buf, int t)
{
    if(t==1 && !strcmp(buf[0], "END"))
    {
        return 0;
    }//if

    switch (t){
        case 0: return 0; break;
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

            if(strcmp(buf[1], "DC"))
            {
                strcpy(program[pc].label, buf[0]);
                strcpy(program[pc].op_code, buf[1]);
                strcpy(program[pc].end, buf[2]);
            }/*else
            {
                return 0;
            }//else*/
            break;
        default: break;
    }//switch
    return 1;
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
            if(!strcmp(buf[1], "DC"))
            {
                strcpy(ref_table[ref_pc].label, buf[0]);
                ref_table[ref_pc].end = pc;
                ref_pc++;
            }else
            {
                strcpy(ref_table[ref_pc].label, buf[0]);
                ref_table[ref_pc].end = pc;
                ref_pc++;
            }//else
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

    if(ref_pc==1)
        puts("");

    //printf("%d\n", cnt);
    if(save_inst(inst, cnt) == 1)
    {
        save_ref(inst, cnt);
        pc++;
    }else save_ref(inst, cnt);

    free(inst);
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