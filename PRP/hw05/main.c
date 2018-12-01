#include <stdio.h>
#include <stdlib.h>

#define MIN(x,y) ((x < y) ? (x) : (y))
#define MAX(x,y) ((x > y) ? (x) : (y))

int **matice1,**matice2,**matice3;
int n1, m1, n2, m2, n3, m3;
char oper1, oper2;
int main(int argc, char *argv[]){
    // MATICE 1
    if(!(scanf("%d %d", &n1, &m1))){
        fputs("Error: Chybny vstup!\n", stderr);
        return 100;
    }
    matice1 =(int**)malloc(n1*sizeof(int*));
    for(int i = 0; i < n1; i++){
        matice1[i] = (int*)calloc(m1, sizeof(int));
    }
    for(int i = 0; i < n1; i++){
        for(int u = 0; u < m1; u++){
            if(!(scanf("%d ", &matice1[i][u]))){
                fputs("Error: Chybny vstup!\n", stderr);
                return 100;
            }
        }
    }
    while(scanf("%c", &oper1) != EOF){
        if(!(scanf("%d %d", &n2, &m2))){
            fputs("Error: Chybny vstup!\n", stderr);
            return 100;
        }
        matice2 =(int**)malloc(n2*sizeof(int*));
        for(int i = 0; i < n2; i++){
            matice2[i] = (int*)calloc(m2, sizeof(int));
        }
        for(int i = 0; i < n2; i++){
            for(int u = 0; u < m2; u++){
                if(!(scanf("%d ", &matice2[i][u]))){
                    fputs("Error: Chybny vstup!\n", stderr);
                    return 100;
                }
            }
        }
        if(oper1 == '*'){
            if(m1 != n2){
                fputs("Error: Chybny vstup!\n", stderr);
                return 100;
            }
            int **newmatice = (int**)malloc(n1*sizeof(int*));
            for(int i = 0; i < n1; i++){
                newmatice[i] = (int*)calloc(m2,sizeof(int));
            }
            for(int i = 0; i < n1; i++){
                for (int j = 0; j < m2; j++){
                    for (int k = 0; k < m1; k++){
                        newmatice[i][j] += matice1[i][k] * matice2[k][j];
                    }
                }
            }
            for(int i = 0; i < n1; i++){
                matice1[i] = (int*)realloc(matice1[i],m2*sizeof(int));
            }
            m1 = m2;
            for(int i = 0; i < n1; i++){
                for(int u = 0; u < m1; u++){
                    matice1[i][u] = newmatice[i][u];
                }
            }
            for(int i = 0; i < n1; i++){
                free(newmatice[i]);
            }
            free(newmatice);
        }
	    again:
        if(oper1 == '+' || oper1 == '-'){
            if(scanf("%c", &oper2) != EOF){
                if(!(scanf("%d %d", &n3, &m3))){
                    fputs("Error: Chybny vstup!\n", stderr);
                    return 100;
                }
                matice3 =(int**)malloc(n3*sizeof(int*));
                for(int i = 0; i < n3; i++){
                    matice3[i] = (int*)calloc(m3, sizeof(int));
                }
                for(int i = 0; i < n3; i++){
                    for(int u = 0; u < m3; u++){
                        if(!(scanf("%d ", &matice3[i][u]))){
                            fputs("Error: Chybny vstup!\n", stderr);
                            return 100;
                        }
                    }
                }
                while(oper2 == '*'){
                    if(m2 != n3){
			            printf("%d %d, %d %d\n", n2, m2,  n3, m3);
                        fputs("Error: Chybny vstup!\n", stderr);
                        return 100;
                    }
                    int **newmatice = (int**)malloc(n2*sizeof(int*));
                    for(int i = 0; i < n2; i++){
                        newmatice[i] = (int*)calloc(m3,sizeof(int));
                    }
                    for(int i = 0; i < n2; i++){
                        for (int j = 0; j < m3; j++){
                            for (int k = 0; k < m2; k++){
                                newmatice[i][j] += matice2[i][k] * matice3[k][j];
                            }
                        }
                    }
                    for(int i = 0; i < n2; i++){
                        matice2[i] = (int*)realloc(matice2[i],m3*sizeof(int));
                    }
                    m2 = m3;
                    for(int i = 0; i < n2; i++){
                        for(int u = 0; u < m2; u++){
                            matice2[i][u] = newmatice[i][u];
                        }
                    }
                    for(int i = 0; i < n1; i++){
                        free(newmatice[i]);
                    }
                    free(newmatice);
                    for(int i = 0; i < n3; i++){
                        free(matice3[i]);
                    }
                    free(matice3);            
                        goto again;
                }
            }
            if(oper1 == '+'){
                for(int i = 0; i < n1; i++){
                    for(int u = 0; u < m1; u++){
                        matice1[i][u] += matice2[i][u];
                    }
                }
            }
            if(oper1 == '-'){
                for(int i = 0; i < n1; i++){
                    for(int u = 0; u < m1; u++){
                        matice1[i][u] -= matice2[i][u];
                    }
                }
            }
            if(oper2 == '+' || oper2 == '-'){
                oper1 = oper2;
                oper2 = ' ';
                matice2 = (int**)realloc(matice2,n3*(sizeof(int*)));
                for(int i = 0; i < n3; i++){
                    matice2[i] = (int*)realloc(matice2[i],m3*sizeof(int));
                }
                n2 = n3;
                m2 = m3;
                for(int i = 0; i < n2; i++){
                    for(int u = 0; u < m2; u++){
                        matice2[i][u] = matice3[i][u];
                    }
                }
		    for(int i = 0; i < n3; i++){
            	    free(matice3[i]);
        	}
        	free(matice3);
                goto again;
            }
        }
        for(int i = 0; i < n2; i++){
            free(matice2[i]);
        }
        free(matice2);
    }
    printf("%d %d\n", n1, m1);
        for(int i = 0; i < n1; i++){
            for(int u = 0; u < m1; u++){
                if(u != 0){
                    printf(" %d", matice1[i][u]);
                }
                else{
                    printf("%d", matice1[i][u]);                    
                }
            }
        printf("\n");
    }
    for(int i = 0; i < n1; i++){
        free(matice1[i]);
    }
    free(matice1);
    return 0;
}
