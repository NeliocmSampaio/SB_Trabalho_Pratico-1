//
// Created by nelio on 20/09/17.
//

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int num_param;

    num_param = argc;

    int i;
    for(i = 1; i<num_param; i++)
    {
        printf("%s ", argv[i]);
    }//for

    return 0;
}//main()