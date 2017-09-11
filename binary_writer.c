//
// Created by nelio on 05/09/17.
//

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char string[20], g_name[20];
    FILE *f, *g;

    if(argc==1) {puts("Digite o nome do arquivo destino"); return 1;}

    sprintf(g_name, "%s.n_binario", argv[1]);

    f = fopen(argv[1], "wb");
    g = fopen(g_name, "w");
    if(f==NULL || g==NULL){puts("Erro ao abrir o arquivo!"); return 1;}

    while(scanf("%s", string)!=EOF)
    {
        fprintf(f, "%s", string);
        fprintf(g, "%s", string);
    }//while
}//main()