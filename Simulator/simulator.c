#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>


#define TAM_INST_MEM 1024
#define TAM_DATA_MEM 256

#define getbit(by, pos) ((by>>pos)&0x01)

typedef struct {
    unsigned char byte[2]/*, second_byte[2]*/;
}instruction;

typedef struct {
    char first_byte[8], second_byte[8];
}inst;

instruction     program[TAM_INST_MEM * 2];  //instruction memory
inst            prog[TAM_INST_MEM];
int             memory [TAM_DATA_MEM];  //data memory

int tam_program;
int load_position;
//Funções auxiliares
void byte2bits(int c, char *s, int t);        //conversão de char para binario
void print_str_tam(char *str, int n);   //imprime tantos caracteres a partir do endereco recebido.
void print_inst(int i);                 //imprime a instrucao na posicao i
void cpy_tam(char *dest, char *src, int t);
void cleanstr(char *str, int tam);
void convbuffer(char *buf, int sizeb, int x);      //converte um inteiro no buffer para ler instruçoes.
void int2char(char *str, int x);        //converte um inteiro em uma palavra de 2 Bytes.
void char2int(int *x, char *str);       //converte uma palavra de 2 Bytes em inteiro.
int pot(int x, int n){int i; for(i=0; i<n; i++)x*=x;}
int bit2byte(char *binstr);

//funções do processador
void print_memory_contents()
{
    int i;
    for(i=0; i<TAM_DATA_MEM; i++)
    {
        printf("%d: '%u'\n", i, memory[i]);
    }//for
}//print_memory_contents()

int run_program2(int program_size)
{
    int pc = 0;
    int ac = 0;
    int op_code;
    int ref;
    int endereco, in, out;
    char op[3], rf[3], aux[20];
    //unsigned char reg[16], fb, sb;
    //char fb1[8], sb1[8];
    //char aux1[8], aux2[8];

    while(pc < (program_size))
    {
        //print_inst(pc);

        cpy_tam(op, program[pc].byte, 2);
        cpy_tam(rf, program[pc+1].byte, 2);

        op_code = atoi(op);
        ref = atoi(rf);

        //printf("%d %d\n", op_code, ref);

        pc+=2;

        switch(op_code)
        {
            case 1:
                endereco = (ref+(pc));
                cpy_tam(aux, program[endereco].byte, 4);
                //char2int(&ac, aux);
                ac = atoi(aux);
                break;
            case 2:
                ac = 1;
                endereco = (ref+(pc));
                sprintf(program[endereco].byte, "%d", ac);
                break;
            case 3:
                endereco = (ref+(pc));
                cpy_tam(aux, program[endereco].byte, 4);
                ac += atoi(aux);
                break;
            case 4:
                endereco = (ref+(pc));
                cpy_tam(aux, program[endereco].byte, 4);
                ac -= atoi(aux);
                break;
            case 5:
                endereco = (ref+(pc));
                scanf("%d", &in);
                sprintf(program[endereco].byte, "%d", in);
                break;
            case 6:
                endereco = (ref+(pc));
                cpy_tam(aux, program[endereco].byte, 4);
                printf("%d", atoi(aux));
                break;
            case 7:
                endereco = (ref+(pc));
                pc = endereco;
                break;
            case 8:
                if(ac>0)
                {
                    endereco = (ref+(pc));
                    pc = endereco;
                }//if
                break;
            case 9:
                if(ac<0)
                {
                    endereco = (ref+(pc));
                    pc = endereco;
                }//if
                break;
            case 10:
                if(ac==0)
                {
                    endereco = (ref+(pc));
                    pc = endereco;
                }//if
                break;
            case 11:
                return 0;
                break;
            default: return -1;
        }//switch

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
    for(i=0; i<=tam_program; i += 2)
    {
        printf("%d:\t", i);
        print_str_tam(&(program[i].byte), 2);
        if(i<tam_program)
            print_str_tam(&(program[i+1].byte), 2);
        //printf("%s", &(program[i].first_byte)/*, &(program[i].second_byte)*/);
        printf("\n");
    }

}//printProgram()

void readProgram(FILE *f)
{
    int aux, buf_size;
    char buffer[7], *pointer, inst[7];
    while(fgets(buffer, sizeof(buffer), f) != NULL)
    {
        buf_size = strlen(buffer);
        /*if(strcmp(buffer, "END"))
        {*/
            if(buf_size == 6)
            {
                tam_program+=2;
            }/*else
            {
                tam_program++;
            }//else*/
        /*}else break;*/
        aux = atoi(buffer);

        pointer = strtok(buffer, " ");
        strcpy(inst, pointer);
        strcpy(&(program[load_position].byte), inst);

        pointer = strtok(NULL, " ");
        if(pointer != NULL)
        {
            strcpy(inst, pointer);
            strcpy(&(program[load_position+1].byte), inst);
            //printf("%s", &(program[load_position].second_byte));
        }else
        {
            strcpy(program[load_position+1].byte, "00");
            program[load_position].byte[1] = '0';
        }
        cleanstr(program[load_position].byte, 4);
        //cleanstr(program[load_position].second_byte, 2);
        if(buf_size == 6)
        {
            load_position += 2;
        }else
        {
            load_position++;
        }//else
    }//while
    if(buf_size == 6)
    {
        tam_program -= 2;
    }else
    {
        tam_program--;
    }//else
}//readProgram()

int main(int argc, char *argv[]) {

    load_position = 0;
    tam_program = 0;

/*    char aux[2];
    int2char(aux, 256);

    int x;
    char2int(&x, aux);*/

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

void byte2bits(int c, char *s, int t)
{
    int i=t-1;
    while(i>=0)
    {
        *s++ = '0' + getbit(c, i--);
    }//while
    *s = 0;
}//byte2bits()

void byte2bitsC(char c, char *s, int t)
{
    int i=t-1;
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
    print_str_tam(&(program[i].byte), 2);
    printf(" ");
    print_str_tam(&(program[i+1].byte), 2);
    printf("\n");
}//print_inst()

void cpy_tam(char *dest, char *src, int t)
{
    int i;
    for (i=0; i<t; i++)
    {
        dest[i] = src[i];
    }//for
    dest[i] = '\0';
}//cpy_tam()

void cleanstr(char *str, int tam)
{
    int i;
    for (i=0; i<tam; i++)
    {
        if(str[i]=='\n' || str[i]=='\0')
        {
            str[i] = '0';
        }//if
    }//for
}

void convbuffer(char *buf, int sizeb, int x)
{
    char aux[sizeb];
    while(x%10>=10)
    {
        aux[sizeb] = x%10;
        x = x/10;
    }
}

void int2char(char *str, int x)
{
    unsigned char conv2str[16], tok[8], conv2char;
    byte2bits(x, conv2str, 16);
    cpy_tam(tok, conv2str, 8);
    conv2char = bit2byte(tok);
    str[0] = conv2char;
    if (str[0] == 1) str[0] = 1; else str[0] = 0;

    cpy_tam(tok, conv2str+8, 8);
    conv2char = bit2byte(tok);
    str[1] = conv2char;
    if (str[1] == 1) str[1] = 1; else str[1] = 0;

}//int2char()

void char2int(int *x, char *str)
{
    unsigned char conv2str[16], tok[8], conv2char;

    byte2bitsC(str[0], tok, 8);
    //strcpy(tok, );
    cpy_tam(conv2str, tok, 8);
    byte2bits(str[1], tok, 8);
    cpy_tam(conv2str+8, tok, 8);

    *x = bit2byte(conv2str);

}//char2int()

char* byte2bit( char c)
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

int bit2byte(char *binstr)
{
    return (int) (strtoul(binstr, NULL, 2) );
}//bit2byte()