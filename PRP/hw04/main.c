/*
 *
 * Matoous, 5. 10. 2016
 *
 */

#include <stdio.h>
#include <stdlib.h>
#define MAX 1000000

long *input;
int incount = 0;
int *primes;
int primescount = 0;

void Eratosthen(){
    int arr[MAX];
    for(int i = 0; i < MAX; i++){
        arr[i] = i;
    }
    int index = 2;
    while(index != MAX){
        if(arr[index] != 0){
            primes[primescount++] = arr[index];
            for(int u = index; u < MAX; u = u*2){
                arr[u] = 0;
            }
            index++;
        } else{
            index++;
        }
    }
}

void Decompose(long num){
    printf("Prvociselny rozklad cisla %li je:\n", num);
    int temp = 0; //cislo
    int temp2 = 0; //mocnina
    if(num == 1){
        printf("1\n");
        return;
    }
    while(num != 1){
        for(int i = 0; i < primescount; i++){
            if(num % primes[i] == 0){
                num = num / primes[i];
                if(primes[i] == temp){
                    temp2++;
                } else{
                    if(temp2 == 1){
                        printf("%d x ", temp);
                    }
                    if(temp2 > 1){
                        printf("%d^%d x ", temp, temp2);
                    }
                    temp = primes[i];
                    temp2 = 1;
                }
                break;
            } 
        }
    }   
    if(temp2 == 1){
        printf("%d", temp);
    } else if(temp2 > 1){
        printf("%d^%d", temp, temp2);
    }
    printf("\n");
}

/* The main program */
int main(int argc, char *argv[])
{
    input = (long*)calloc(100,sizeof(int));
    primes = (int*)calloc(MAX,sizeof(int));
    Eratosthen();
    if(!(scanf("%li", &input[incount++]))){
        fputs("Error: Chybny vstup!\n", stderr);
        return 100;
    }
    while(1){
        if(!(scanf("%li", &input[incount++]))){
            fputs("Error: Chybny vstup!\n", stderr);
            return 100;
        }
        if(input[incount - 1] < 0){
            fputs("Error: Chybny vstup!\n", stderr);
            return 100;
        }
        if(input[incount - 1] == 0){
            break;
        }
    }
    for(int i = 0; i < incount - 1; i++){
        Decompose(input[i]);
    }
    free(primes);
    free(input);
    return 0;
}

