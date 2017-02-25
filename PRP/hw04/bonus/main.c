

#include <stdio.h>
#include <stdlib.h>


#define STO 101
#define STOTISIC 100000
#define MAX 1000000

short **arr;
int *arr_count;
int *primes;
int primescount;
char help;

void Eratosthen(){
    int *nums = (int*)calloc(MAX,sizeof(int));
    for(int i = 0; i < MAX; i++){
        nums[i] = i;
    }
    int index = 2;
    while(index != MAX){
        if(nums[index] != 0){
            primes[primescount++] = nums[index];
            for(int u = index; u < MAX; u += primes[primescount - 1]){
                nums[u] = 0;
            }
            index++;
        } else{
            index++;
        }
    }
    free(nums);
}

short Devides(int input_index, int prime_index){
    int prime = primes[prime_index];
    int prime_num_lenght = 0;
    int input_num_lenght = arr_count[input_index];
    short *result = (short*)calloc(arr_count[input_index], sizeof(short));
    short result_lenght = -1;
    while(prime != 0){
        prime_num_lenght++;
        prime /= 10;
    }
    prime = primes[prime_index];
    int index = prime_num_lenght - 1;
    int num = 0;
    for(int i = 0; i < prime_num_lenght; i++){
        num *= 10;
        num += arr[input_index][i];
    }
    if(num < prime){
        num *= 10;
        num += arr[input_index][prime_num_lenght];
        index++;
    }
    while(index != input_num_lenght){
        int count = 0;
        while(num >= prime){
            num -= prime;
            count++;
        }
        result_lenght++;
        result[result_lenght] = count;
        index++;
        if(index != input_num_lenght){
            num *= 10;
            num += arr[input_index][index];
        }
    }
    if(num == 0){
        for(int i = 0; i < arr_count[input_index]; i++){
            if(i <= result_lenght){
                arr[input_index][i] = result[i];
            } else{
                arr[input_index][i] = 0;
            }
        }
        arr_count[input_index] = result_lenght + 1;
        free(result);
        return 1;
    } else{
        free(result);
        return 0;
    }
}

short IsOne(int input_index){
    for(int i = 0; i < arr_count[input_index] - 1; i++){
        if(arr[input_index][i] != 0){
            return 0;
        }
    }
    if(arr[input_index][arr_count[input_index] - 1] != 1){
        return 0;
    }
    return 1;
}

void Output(int input_index){
    printf("Prvociselny rozklad cisla ");
    for(int i = 0; i < arr_count[input_index]; i++){
        printf("%d", arr[input_index][i]);
    }
    printf(" je:\n");
    if(arr_count[input_index] == 1 && arr[input_index][0] == 1){
        printf("1\n");
        return;
    }
    int temp = 0; //cislo
    int temp2 = 0; //mocnina
    while(!IsOne(input_index)){
        for(int i = 0; i < primescount; i++){
            if(Devides(input_index, i) == 1){
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

int main(int argc, char *argv[]){
    primescount = 0;
    primes = (int*)calloc(STOTISIC,sizeof(int));
    arr_count = (int*)calloc(STO,sizeof(int));
    arr = (short**)malloc(STO*sizeof(short*));
    for(int i = 0; i < STO; i++){
        arr[i] = (short*)calloc(STO,sizeof(short));
    }
    Eratosthen();
    char c = '1';
    int inputs_count = 0;
    while(1){
        c = fgetc(stdin);
        if(c == '\n'){
            Output(inputs_count++);
            continue;
        } else{
            if(c == '0' && arr_count[inputs_count] == 0){
                break;
            }
            if(c > '9' || c < '0'){
                fputs("Error: Chybny vstup!\n", stderr);
                return 100;
            }
            switch(c){
                case '0':
                    arr[inputs_count][arr_count[inputs_count]++] = 0;
                    break;
                case '1':
                    arr[inputs_count][arr_count[inputs_count]++] = 1;
                    break;
                case '2':
                    arr[inputs_count][arr_count[inputs_count]++] = 2;
                    break;
                case '3':
                    arr[inputs_count][arr_count[inputs_count]++] = 3;
                    break;
                case '4':
                    arr[inputs_count][arr_count[inputs_count]++] = 4;
                    break;
                case '5':
                    arr[inputs_count][arr_count[inputs_count]++] = 5;
                    break;
                case '6':
                    arr[inputs_count][arr_count[inputs_count]++] = 6;
                    break;
                case '7':
                    arr[inputs_count][arr_count[inputs_count]++] = 7;
                    break;
                case '8':
                    arr[inputs_count][arr_count[inputs_count]++] = 8;
                    break;
                case '9':
                    arr[inputs_count][arr_count[inputs_count]++] = 9;
                    break;
            }
        }
    }
  
    free(primes);
    free(arr_count);
    for(int i = 0; i < STO; i++){
        free(arr[i]);
    }
    free(arr);
}
