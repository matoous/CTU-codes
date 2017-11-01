#!/bin/bash
gcc -x c - <<EOF
#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#define BOOL char
#define TRUE 1
#define FALSE 0
BOOL count_total = FALSE, zip = FALSE, display_order = FALSE, has_error = FALSE;
int cf = 0, cd = 0, cl = 0;
char* file_names;
int file_names_len = 0;
int cntLines(const char* name){
    FILE* f = fopen(name, "r");
    if (f == NULL) return -1;
    int lines = 0;
    char ch;
    while(!feof(f))
    {
        ch = fgetc(f);
        if (ferror (f)) return -1;
        if(ch == '\n') lines++;
    }
    return lines == 0 ? -1 : lines;
}
int handleInput(char str[1000]){
    if (!(str[0] == 'F' && str[1] == 'I' && str[2] == 'L' && str[3] == 'E' && str[4] == ' ')) return 0;
    for(int i = 5; i < 1000; i++) str[i-5] = str[i]; // shift the string by 5 chars
    struct stat path_stat;
    if(!lstat(str, &path_stat)){
        if (S_ISLNK(path_stat.st_mode)){
            char buf[1000];
            ssize_t tmplen;
            if ((tmplen = readlink(str, buf, sizeof(buf)-1)) != -1)
                buf[tmplen] = '\0';
            display_order ? printf("LINK %d '%s' '%s'\n", cl+1, str, buf) : printf("LINK '%s' '%s'\n", str, buf);
            cl++;
        } else if(S_ISREG(path_stat.st_mode)){
            FILE* f = fopen(str, "r");
            if (f == NULL) return 2;
            char line[8000];
            int ll = 0;
            char ch;
            while(!feof(f))
            {
                ch = fgetc(f);
                if (ferror (f)) return 2;
                if(ch != '\n' && ch != EOF) line[ll++] = ch;
                else break;
            }
            line[ll] = '\0';
            fclose(f);
            int lc = cntLines(str);
            if (lc < 0)
                return 2;
            display_order ?  printf("FILE %d '%s' %d '%s'\n", cf+1, str, lc, line) : printf("FILE '%s' %d '%s'\n", str, lc, line);
            cf++;
            if(zip){
                int idx = 0;
                file_names[file_names_len++] = ' ';
                file_names[file_names_len++] = '"';
                while(str[idx] != '\0'){
                    file_names[file_names_len++] = str[idx];
                    idx++;
                }
                file_names[file_names_len++] = '"';
            }
        } else if (S_ISDIR(path_stat.st_mode)){
            display_order ? printf("DIR %d '%s'\n", cd+1, str) : printf("DIR '%s'\n", str);
            cd++;
        } else {
            return 2;
        }
    } else {
        if (errno == ENOENT) {
            fprintf(stderr, "ERROR '%s'\n", str);
            has_error = TRUE;
            return 0;
        }
        return 2;
    }
    return 0;
}
int main(int argc, char** argv) {
    if (argc > 1){
        for(int i = 1; i < argc; i++){
            if (strcmp(argv[i], "-h") == 0){
                printf("Usage:\nscript.sh atd.");
                return 0;
            }
            if (strcmp(argv[i], "-v") == 0){
                count_total = TRUE;
                continue;
            }
            if (strcmp(argv[i], "-z") == 0){
                zip = TRUE;
                file_names = (char*)calloc(100000,sizeof(char));
                continue;
            }
            if (strcmp(argv[i], "-n") == 0){
                display_order = TRUE;
                continue;
            }
            return 2;
        }
    }
    char c;
    char str[1000];
    int strlen = 0;
    while((c = getchar()) != EOF){
        if(c == '\n'){
            str[strlen] = '\0';
            int res = handleInput(str);
            if (res != 0)
                return res;
            strlen = 0;
            continue;
        } else str[strlen++] = c;
    }
    if(count_total)
        printf("%d\n%d\n%d\n", cf, cd, cl);
    if(zip){
        file_names[file_names_len] = '\0';
        char buff[file_names_len+100];
        sprintf(buff, "tar czf output.tgz%s", file_names);
        int ret = system(buff);
        if(ret != 0) return 2;
    }
    return has_error;
}
EOF
./a.out "$@"
