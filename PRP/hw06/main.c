#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define SIXTEEN 16
#define MAX_SHIFT 51

char *secret;
int secret_count = 0;
int secret_max = SIXTEEN;
char *eavesdrop;
int eavesdrop_count = 0;
int eavesdrop_max = SIXTEEN;
int **levan;
char *temparr;

int Minimum(int a, int b, int c){
    if(a < b && a < c){
        return a;
    }
    if(b < c){
        return b;
    }
    return c;
}

int LevanshteinDist(char *string1, int string1_lenght, char *string2, int string2_lenght){
    levan[0][0] = 0;
    for(int i = 1; i <= string1_lenght; i++){
        levan[i][0] = i;
    }
    for(int i = 1; i <= string2_lenght; i++){
        levan[0][i] = i;
    }
    for(int i = 1; i <= string1_lenght; i++){
        for(int u = 1; u <= string2_lenght; u++){
            levan[i][u] = Minimum(levan[i-1][u] + 1, levan[i][u-1] + 1, levan[i-1][u-1] + (string1[i-1] == string2[u-1] ? 0 : 1));
        }
    }
    int num = levan[string1_lenght][string2_lenght];
    for(int i = 1; i <= string1_lenght; i++){
        for(int u = 1; u <= string2_lenght; u++){
            levan[i][u] = 0;
        }
    }
    return num;
}

int main(int argc, char *argv[]){
    char c;
    short optional = 0;
    char optional_str[] = "-prp-optional";
    if(argc == 2 && strcmp(argv[1], optional_str) == 0){
        optional = 1;
    }
    secret = (char*)malloc(secret_max* sizeof(char));
    eavesdrop = (char*)malloc(eavesdrop_max* sizeof(char));
    while(scanf("%c", &c) != EOF){
        if(c == '\n'){
            break;
        }
        if(!((c > 64 && c < 91) || (c > 96 && c < 123)) && c != '\n'){
            free(secret);
            free(eavesdrop);
            fputs("Error: Chybny vstup!\n", stderr);
            return 100;
        }
        secret[secret_count++] = c;
        if(secret_count == secret_max){
            secret_max *= 2;
            secret = (char*)realloc(secret,secret_max*sizeof(char));
        }
    }
    while(scanf("%c", &c) != EOF){
        if(c == '\n' || c == EOF){
            break;
        }
        if(!((c > 64 && c < 91) || (c > 96 && c < 123)) && c != '\n'){
            free(secret);
            free(eavesdrop);
            fputs("Error: Chybny vstup!\n", stderr);
            return 100;
        }
        eavesdrop[eavesdrop_count++] = c;
        if(eavesdrop_count > secret_count && !optional){
            free(secret);
            free(eavesdrop);
            fputs("Error: Chybna delka vstupu!\n", stderr);
            return 101;
        }
        if(eavesdrop_count == eavesdrop_max){
            eavesdrop_max *= 2;
            eavesdrop = (char*)realloc(eavesdrop,eavesdrop_max*sizeof(char));
        }
    }
    if(!optional){
        int match = 0;
        int best_shift = 0;
        int best_match = 0;
        for(int i = 0; i < MAX_SHIFT; i++){
            match = 0;
            for(int u = 0; u < secret_count; u++){
                short ch = (short)secret[u];
                if(ch < 91 && ch + i > 90){
                    ch += i + 6;
                } else{
                    ch += i;
                }
                if(ch > 122){
                    ch -= 58;
                    if(ch > 90){
                        ch += 6;
                    }
                }
                if((char)ch == eavesdrop[u]){
                    match++;
                }
            }
            if(match > best_match){
                best_shift = i;
                best_match = match;
            }
        }
        for(int i = 0; i < secret_count; i++){
            short ch = (short)secret[i];
            if(ch < 91 && ch + best_shift > 90){
                ch += best_shift + 6;
            } else{
                ch += best_shift;
            }
            if(ch > 122){
                ch -= 58;
                if(ch > 90){
                    ch += 6;
                }
            }
            printf("%c",(char)ch);
        }
        printf("\n");
    } else{
        int dist = INT_MAX;
        int best_shift = 0;
        int best_dist = INT_MAX;
        temparr = (char*)malloc(secret_count*sizeof(char));
        levan = (int**)malloc((secret_count + 1) * sizeof(int*));
        for(int i = 0; i <= secret_count; i++){
            levan[i]=(int*)calloc(eavesdrop_count + 1,sizeof(int));
        }
        for(int i = 0; i < MAX_SHIFT; i++){
            for(int u = 0; u < secret_count; u++){
                short ch = (short)secret[u];
                if(ch < 91 && ch + i > 90){
                    ch += i + 6;
                } else{
                    ch += i;
                }
                if(ch > 122){
                    ch -= 58;
                    if(ch > 90){
                        ch += 6;
                    }
                }
                temparr[u] = ch;
            }
            dist = LevanshteinDist(temparr,secret_count,eavesdrop,eavesdrop_count);
            if(dist <= best_dist){
                best_shift = i;
                best_dist = dist;
            }       
        }
        for(int i = 0; i <= secret_count; i++){
            free(levan[i]);
        }
        free(levan);
        free(temparr);
        for(int i = 0; i < secret_count; i++){
            short ch = (short)secret[i];
            if(ch < 91 && ch + best_shift > 90){
                ch += best_shift + 6;
            } else{
                ch += best_shift;
            }
            if(ch > 122){
                ch -= 58;
                if(ch > 90){
                    ch += 6;
                }
            }
            printf("%c",(char)ch);
        }
        printf("\n");
    }

    free(secret);
    free(eavesdrop);
    return 0;
}
