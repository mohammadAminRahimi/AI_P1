#include <stdlib.h>
#include <string.h>
#include <stdio.h>



struct Key{
    int k;
};

int  main(){
    struct Key* k = (struct Key*)malloc(sizeof(struct Key));
    k->k = 0;
    printf("%d", k->k);
}

5 3 5
5r 4r 3r 1e
1r 2r 
2e 3e 4e
5e
5t  1t 2t 3t  4t
