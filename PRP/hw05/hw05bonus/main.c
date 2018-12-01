#include <stdio.h>
#include <stdlib.h>

#define R1 26
#define R2 27

int ***matice;
int *matice_size_x, *matice_size_y, *max_size_x, *max_size_y;
unsigned char oper1, oper2;

int main(int argc, char * argv[]){
    //ALLOC
    matice = (int***)malloc(28*sizeof(int**));
    matice_size_x = (int*)calloc(28,sizeof(int));
    matice_size_y = (int*)calloc(28,sizeof(int));
    max_size_x = (int*)calloc(28,sizeof(int));
    max_size_y = (int*)calloc(28,sizeof(int));
    int max_x = 0;
    int max_y = 0;
    unsigned char name;
    scanf("%c", &name);
    while(name >= 65 && name <= 90){
        name -= 65;
        matice[name] = (int**)malloc(1*sizeof(int*));
        matice[name][0] = (int*)malloc(4*sizeof(int));
        max_size_x[name] = 1;
        max_size_y[name] = 4;
        unsigned char c;
        scanf("%c", &c);
        scanf("%c", &c); 
        while(c != ']'){
            if(c == ';'){
                if(max_size_x[name] == matice_size_x[name]){
                    max_size_x[name] *= 2;
                    matice[name] = (int**)realloc(matice[name], max_size_x[name]*sizeof(int*));
                    for(int i = matice_size_x[name]; i < max_size_x[name]; i++){
                        matice[name][i]= (int*)malloc(max_size_y[name]*sizeof(int));
                    }
                }
                for(int i = 0; i < matice_size_y[name]; i++){
                    scanf("%d", &matice[name][matice_size_x[name]][i]);
                }
                matice_size_x[name]++;
            }
            if(c == '['){
                int temp;
                while(scanf("%d", &temp)){
                    matice[name][matice_size_x[name]][matice_size_y[name]++] = temp;
                    if(max_size_y[name] == matice_size_y[name]){
                        max_size_y[name] *= 2;
                        matice[name][matice_size_x[name]] = (int*)realloc(matice[name][matice_size_x[name]], max_size_y[name]*sizeof(int));
                    }
                }
                matice_size_x[name]++;
                if(max_y < matice_size_y[name]){
                    max_y = matice_size_y[name];
                }
            }
            scanf("%c", &c);
        }
        if(max_x < matice_size_x[name]){
            max_x = matice_size_x[name];
        }
        scanf("%c", &name);
        scanf("%c", &name);

    }
    matice_size_x[R1] = matice_size_x[R2] = max_size_x[R1] = max_size_x[R2] = max_x;
    matice_size_y[R1] = matice_size_y[R2] = max_size_y[R1] = max_size_y[R2] = max_y;
    matice[R1] = (int**)malloc(max_x*sizeof(int*));
    for(int i = 0; i < max_x; i++){
        matice[R1][i] = (int*)calloc(max_y,sizeof(int));
    }
    matice[R2] = (int**)malloc(max_x*sizeof(int*));
    for(int i = 0; i < max_x; i++){
        matice[R2][i] = (int*)calloc(max_y,sizeof(int));
    }
    /*for(int i = 0; i < 28; i++){
        if(matice_size_x[i] != 0){
            printf("%c: [", (char)i+65);
            for(int u = 0; u < matice_size_x[i]; u++){
                for(int l = 0; l < matice_size_y[i]; l++){
                    printf("%d ", matice[i][u][l]);
                }
                printf("; ");
            }
            printf("]\n");
        }
    }*/
    unsigned char m1name, m2name, m3name;
    scanf("%c", &m1name);
    m1name -= 65;
    scanf("%c", &oper1);
    while(oper1 == '*' || oper1 == '-' || oper1 == '+'){
        if(!scanf("%c", &m2name)){
            if(m2name == '\n'){
                break;
            }
            fputs("Error: Chybny vstup!\n", stderr);
            return 100;
        }
        m2name -= 65;
        here:
        if(oper1 == '*'){
            if(matice_size_y[m1name] != matice_size_x[m2name]){
                fputs("Error: Chybny vstup!\n", stderr);
                return 100;
            }
            int **newmatice = (int**)malloc(matice_size_x[m1name]*sizeof(int*));
            for(int i = 0; i < matice_size_x[m1name]; i++){
                newmatice[i] = (int*)calloc(matice_size_y[m2name],sizeof(int));
            }
            for(int i = 0; i < matice_size_x[m1name]; i++){
                for (int j = 0; j < matice_size_y[m2name]; j++){
                    for (int k = 0; k < matice_size_y[m1name]; k++){
                        newmatice[i][j] += matice[m1name][i][k] * matice[m2name][k][j];
                    }
                }
            }
            matice_size_y[R1] = matice_size_y[m2name];
            matice_size_x[R1] = matice_size_x[m1name];
            for(int i = 0; i < matice_size_x[R1]; i++){
                for(int u = 0; u < matice_size_y[R1]; u++){
                    matice[R1][i][u] = newmatice[i][u];
                }
            }
            m1name = R1;
            for(int i = 0; i < matice_size_x[m1name]; i++){
                free(newmatice[i]);
            }
            free(newmatice);
            if(!scanf("%c", &oper1)){
                goto end;
            }
            continue;
        } else if(oper1 == '+' || oper1 == '-'){
            if(!scanf("%c", &oper2)){
                goto end;
            }
            if(oper2 == '\n'){
                goto end;
            } else{
                if(oper2 == '*'){
                    if(!scanf("%c", &m3name)){
                        goto end;
                    }   
                    if(m3name == '\n'){
                        goto end;
                    } 
                    m3name -= 65;               
                    if(matice_size_y[m2name] != matice_size_x[m3name]){
                        fputs("Error: Chybny vstup! a\n", stderr);
                        return 100;
                    }
                    int **newmatice = (int**)malloc(matice_size_x[m2name]*sizeof(int*));
                    for(int i = 0; i < matice_size_x[m2name]; i++){
                        newmatice[i] = (int*)calloc(matice_size_y[m3name],sizeof(int));
                    }
                    for(int i = 0; i < matice_size_x[m2name]; i++){
                        for (int j = 0; j < matice_size_y[m3name]; j++){
                            for (int k = 0; k < matice_size_y[m2name]; k++){
                                newmatice[i][j] += matice[m2name][i][k] * matice[m3name][k][j];
                            }
                        }
                    }
                    matice_size_y[R2] = matice_size_y[m3name];
                    matice_size_x[R2] = matice_size_x[m2name];
                    for(int i = 0; i < matice_size_x[R2]; i++){
                        for(int u = 0; u < matice_size_y[R2]; u++){
                            matice[R2][i][u] = newmatice[i][u];
                        }
                    }
                    m2name = R2;
                    for(int i = 0; i < matice_size_x[m2name]; i++){
                        free(newmatice[i]);
                    }
                    free(newmatice);
                    oper2 = ' ';
                    goto here;
                } else{
                    if(oper1 == '+'){
                        if(matice_size_x[m1name] != matice_size_x[m2name] || matice_size_y[m1name] != matice_size_y[m2name]){
                            fputs("Error: Chybny vstup!\n", stderr);
                            return 100;
                        }
                        matice_size_y[R1] = matice_size_y[m1name];
                        matice_size_x[R1] = matice_size_x[m1name];
                        for(int i = 0; i < matice_size_x[m1name]; i++){
                            for(int u = 0; u < matice_size_y[m1name]; u++){
                                matice[R1][i][u] = matice[m1name][i][u] + matice[m2name][i][u];
                            }
                        }
                        oper1 = oper2;
                        oper2 ='\n';
                        m1name = R1;
                        continue;
                    }
                    if(oper1 == '-'){
                        if(matice_size_x[m1name] != matice_size_x[m2name] || matice_size_y[m1name] != matice_size_y[m2name]){
                            fputs("Error: Chybny vstup!\n", stderr);
                            return 100;
                        }
                        matice_size_y[R1] = matice_size_y[m1name];
                        matice_size_x[R1] = matice_size_x[m1name];
                        for(int i = 0; i < matice_size_x[m1name]; i++){
                            for(int u = 0; u < matice_size_y[m1name]; u++){
                                matice[R1][i][u] = matice[m1name][i][u] - matice[m2name][i][u];
                            }
                        }
                        oper1 = oper2;
                        oper2 ='\n';
                        m1name = R1;
                        continue;
                    }
                }
            }
        }
        end:
        if(oper1 == '+'){
            if(matice_size_x[m1name] != matice_size_x[m2name] || matice_size_y[m1name] != matice_size_y[m2name]){
                fputs("Error: Chybny vstup!\n", stderr);
                return 100;
            }
            for(int i = 0; i < matice_size_x[m1name]; i++){
                for(int u = 0; u < matice_size_y[m1name]; u++){
                    matice[R1][i][u] = matice[m1name][i][u] + matice[m2name][i][u];
                }
            }
            matice_size_x[R1] = matice_size_x[m1name];
            matice_size_y[R1] = matice_size_y[m1name];
            m1name = R1;
            break;
        }
        if(oper1 == '-'){
            if(matice_size_x[m1name] != matice_size_x[m2name] || matice_size_y[m1name] != matice_size_y[m2name]){
                fputs("Error: Chybny vstup!\n", stderr);
                return 100;
            }
            for(int i = 0; i < matice_size_x[m1name]; i++){
                for(int u = 0; u < matice_size_y[m1name]; u++){
                    matice[R1][i][u] = matice[m1name][i][u] - matice[m2name][i][u];
                }
            }
            matice_size_x[R1] = matice_size_x[m1name];
            matice_size_y[R1] = matice_size_y[m1name];
            m1name = R1;
            break;
        }
        scanf("%c", &oper1);
    }
    printf("[");
    for(int i = 0; i < matice_size_x[R1]; i++){
        for(int u = 0; u < matice_size_y[R1]; u++){
            if(u == 0){
                printf("%d", matice[R1][i][u]);
            } else{
                printf(" %d", matice[R1][i][u]);                
            }
        }
        if(i != matice_size_x[R1] - 1){
            printf("; ");
        }
    }
    printf("]\n");
    for(int i = 0; i < 28; i++){
        if(max_size_x[i] != 0){
            for(int u = 0; u < max_size_x[i]; u++){
                free(matice[i][u]);
            }
            free(matice[i]);
        }
    }
    free(matice);
    free(matice_size_x);
    free(matice_size_y);
    free(max_size_x);
    free(max_size_y);
    return 0;
}
