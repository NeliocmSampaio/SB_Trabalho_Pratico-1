//
// Created by nelio on 05/09/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define TAM_INST_MEM 250

char* byte2bit (unsigned char c);
unsigned char bit2byte (char* binstr);

typedef struct {
	unsigned char first_byte[8], second_byte[8];
}instruction;

instruction program[TAM_INST_MEM];

int main(int argc, char *argv[])
{
    int i;
    char string[8], g_name[20];
    FILE *f, *g;

    if(argc==1) {puts("Informe o nome do arquivo e entrada e de destino"); return 1;}

    //sprintf(g_name, "%s.n_binario", argv[1]);

    f = fopen(argv[1], "r");
    g = fopen(argv[2], "wb");
    if(f==NULL || g==NULL){puts("Erro ao abrir os arquivos!"); return 1;}

    int tam = fread(program, 1, sizeof(program), f);

    if(tam == 0) {
        //puts("")
        return 0;
    }else
    {
        for(i = 0; i<tam/2; i++)
        {
            printf("%s %s\n", program[i].first_byte, program[i].second_byte);
        }

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

