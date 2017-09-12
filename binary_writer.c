//
// Created by nelio on 05/09/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define TAM_INST_MEM 250

char* byte2bit (unsigned char c);
unsigned char bit2byte (char* binstr);
unsigned char bit2byte_T (char* binstr, int t);

void rm_enter(char *str, int t);
void print_lim_str(char *str, int t);
void fprint_lim_str(char *str, int t, FILE *f);

typedef struct {
	unsigned char first_byte[8], second_byte[8];
}instruction;

instruction program[TAM_INST_MEM];

int main(int argc, char *argv[])
{
    int i;
    char string[8], g_name[20];
    char *leitura;
    FILE *f, *g;

    if(argc==1) {puts("Informe o nome do arquivo e entrada e de destino"); return 1;}

    //sprintf(g_name, "%s.n_binario", argv[1]);

    f = fopen(argv[1], "rb");
    g = fopen(argv[2], "wb");
    if(f==NULL || g==NULL){puts("Erro ao abrir os arquivos!"); return 1;}

    leitura = &program;
    int tam = fread(leitura, 1, sizeof(program), f);
    //int tam2 = (tam - tam%16);
    int tam2 = tam - (tam/17) ;

    rm_enter(leitura, tam);

    //printf("tam: %d; tam2: %d\n", tam, tam2);

    if(tam == 0) {
        //puts("")
        return 0;
    }else
    {
        for(i = 0; i<tam2/(16); i++)
        {
            //printf("i: %d; tam2: %d\n", i, tam2);
            //fprint_lim_str(program[i].first_byte, 8, g);
            //fprint_lim_str(program[i].second_byte, 8, g);
            fprintf(g, "%c%c", bit2byte_T(program[i].first_byte, 8),
                   bit2byte_T(program[i].second_byte, 8));
            print_lim_str(program[i].first_byte, 8);
            printf(" ");
            print_lim_str(program[i].second_byte, 8);
            printf("\n");
            //printf("%s %s\n", program[i].first_byte, program[i].second_byte);
        }//for

        //fprintf(stdout, "%s", string);
        //fprintf(g, "%u", bit2byte(string));
    }//while
}//main()

char* byte2bit( unsigned char c)
{
	static char bin[CHAR_BIT + 1] = {0};
	int i;

	for(i=CHAR_BIT - 1; i>=0; i--)
	{
		bin[i] = (c % 2) + '0';
		c /=2;
	}//for
	return bin;
}//byte2bit()

 unsigned char bit2byte (char* binstr)
 {
 	return (unsigned char) (strtoul(binstr, NULL, 2) );
 }//bit2byte()

unsigned char bit2byte_T (char* binstr, int t)
{
    char inst[t];
    int i;
    for(i=0; i<t; i++) inst[i] = binstr[i];
    return (unsigned char) (strtoul(inst, NULL, 2) );
}//bit2byte()

void rm_enter(char *str, int t)
{
    int i, j=0;
    for(i=0; i<t; i++)
    {
        if(str[i]!='\n')
        {
            str[j] = str[i];
            j++;
        }//if
    }//for
}//rm_enter()

void print_lim_str(char *str, int t)
{
    int i;
    for(i=0; i<t; i++)
    {
        printf("%c", str[i]);
    }//for
}//print_lim_str()

void fprint_lim_str(char *str, int t, FILE *f)
{
    int i;
    for(int i=0; i<t; i++)
    {
        fprintf(f, "%c", str[i]);
    }//for
}//fprint_lim_str()