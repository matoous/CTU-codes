#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "lock.h"

#define EIGHT 9
#define true 1
#define false 0

char *arr;
bool DoIt(char*,short,char*,short);
short arr_lenght;
bool opt;
int tries;

bool DoIt(char *did, short did_lenght, char *todo, short todo_lenght){
    if(todo_lenght == 0){
        did[did_lenght] = '\0';
        if(!opt){
            for(int i = 0; i < did_lenght; i++){
                printf("%c", did[i]);
            }
            printf("\n");
            return false;
        } else{
            tries++;
            if(unlock(did)){
                for(int i = 0; i < did_lenght; i++){
                    printf("%c", did[i]);
                }
                printf(" - %dx\n", tries);
                return true;
            } else{
                return false;
            }
        }
    } else{
        for(int i = 0; i < todo_lenght; i++){
            char new_todo[todo_lenght - 1];
            short new_todo_lenght = 0;
            did[did_lenght] = todo[i];
            for(int u = 0; u < todo_lenght; u++){
                if(u != i){
                    new_todo[new_todo_lenght++] = todo[u];
                }
            }
            bool complete = DoIt(did, did_lenght+1, new_todo, new_todo_lenght);
            if(complete){
		        return true;
            }
        }
    }
    return false;
}

int main(int argc, char *argv[])
{
    arr = (char*)calloc(EIGHT,sizeof(char));
    arr_lenght = 0;
    char c;
    while(1){
        opt = false;
        if( argc == 2) {
            if(!strcmp(argv[1],"-prp-optional"))
                opt = true;
                tries = 0;
        }
        if(scanf("%c", &c) == EOF || c == '\n'){
            break;
        }
        if(!((c > 64 && c < 91) || (c > 96 && c < 123) || (c > 48 && c < 58))){
            fputs("Error: Chybny vstup!\n", stderr);
            return 100;
        }
        for(int i = 0; i < arr_lenght; i++){
            if(arr[i] == c){
                fputs("Error: Chybny vstup!\n", stderr);
                return 100;
            }
        }
        arr[arr_lenght++] = c;
    }
    if(arr_lenght == 0 || arr_lenght > 8){
        fputs("Error: Chybny vstup!\n", stderr);
        return 100;
    }
    bool unlocked = false;
    for(int i = 0; i < arr_lenght; i++){
        char did[arr_lenght];
        did[0] = arr[i];
        int did_lenght = 1;
        char todo[arr_lenght];
        int todo_lenght = 0;
        for(int u = 0; u < arr_lenght; u++){
            if(u != i){
               todo[todo_lenght++] = arr[u];
            }
        }
        if(DoIt(did, did_lenght, todo, todo_lenght)){
            unlocked = true;
            break;
        }
    }
    free(arr);
    if(opt && !unlocked){
        fputs("Error: Heslo nebylo nalezeno!\n", stderr);
        return 101;
    }
    return 0;
}
