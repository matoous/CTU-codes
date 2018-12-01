#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define TOP 69
#define BOTTOM 3

int main(void) {

    char width_line[21];
    char height_line[21];
    char length_line[21];
    char* end;
    int width, height, length, ladder_begin_row;

    scanf("%20s %20s", width_line, height_line);

    for (int i = 0; i < strlen(width_line); i++) {
        if ((isdigit(width_line[i]) == 0) && (width_line[i] != '-')) {
            fprintf(stderr, "Error: Chybny vstup!\n");
            return 100;
        }
    }
    for (int i = 0; i < strlen(height_line); i++) {
        if ((isdigit(height_line[i]) == 0) && (height_line[i] != '-')) {
            fprintf(stderr, "Error: Chybny vstup!\n");
            return 100;
        }
    }

    width = (int) strtol(width_line, &end, 10);
    height = (int) strtol(height_line, &end, 10);

    /* 3d plot */
    if (width == height) {

        scanf("%20s", length_line);
        length = (int) strtol(length_line, &end, 10);

        for (int i = 0; i < strlen(length_line); i++) {
            if ((isdigit(length_line[i]) == 0) && (length_line[i] != '-')) {
                fprintf(stderr, "Error: Chybny vstup!\n");
                return 100;
            }
        }

        if (   width < BOTTOM
            || width > TOP
            || height < BOTTOM
            || height > TOP) {
            fprintf(stderr, "Error: Vstup mimo interval!\n");
            return 101;
        }

        if (width % 2 == 0) {
            fprintf(stderr, "Error: Sirka neni liche cislo!\n");
            return 102;
        }

        if (length <= 0 || length >= height) {
            fprintf(stderr, "Error: Neplatna velikost plotu!\n");
            return 103;
        }

        /* roof */
        for (int i = (width / 2) - 1, right = 1; i > 0; i--, right += 2) {

            /* if first line */
            if (i == (width / 2) - 1) {
                for (int j = 0; j < i + 1; j++) {
                    putchar(' ');
                }
                putchar('X');
                putchar('\n');
            }

            /* print left X */
            for (int j = 0; j < i; j++) {
                putchar(' ');
            }
            putchar('X');

            /* print right X */
            for (int j = 0; j < right; j++) {
                putchar(' ');
            }
            putchar('X');
            putchar('\n');

        }

        /* body */
        ladder_begin_row = height - length;
        for (int row = 0; row < height; row++) {

            /* first line */
            if (row == 0) {
                for (int j = 0; j < width; j++)
                    putchar('X');
                putchar('\n');
            }
            /* last line */
            else if (row == height - 1) {
                for (int j = 0; j < width; j++)
                    putchar('X');
                /* if building a ladder (the trick is that the ladder starts in between) */
                if (length % 2 == 0) {
                    for (int ch = 0; ch < length; ch++) {
                        if (ch % 2 == 0)
                            putchar('-');
                        else
                            putchar('|');
                    }
                }
                else {
                    for (int ch = 0; ch < length; ch++) {
                        if (ch % 2 == 0)
                            putchar('|');
                        else
                            putchar('-');
                    }
                }
                putchar('\n');
            }
            else { /* in between */
                putchar('X');

                /* filling space between X and X */

                /* odd row */
                if (row % 2 != 0) {
                    for (int ch = 0; ch < width - 2; ch++) {
                        /* even character */
                        if (ch % 2 == 0)
                            putchar('o');
                        else /* odd character */
                            putchar('*');
                    }
                }
                else { /* even row */
                    for (int ch = 0; ch < width - 2; ch++) {
                        /* even character */
                        if (ch % 2 == 0)
                            putchar('*');
                        else /* odd character */
                            putchar('o');
                    }
                }
                putchar('X');

                /* building a "ladder" */
                if (row >= ladder_begin_row) {

                    /* first line of the ladder */
                    if (row == ladder_begin_row) {
                        if (length % 2 == 0) {
                            for (int ch = 0; ch < length; ch++) {
                                if (ch % 2 == 0)
                                    putchar('-');
                                else
                                    putchar('|');
                            }
                        }
                        else {
                            for (int ch = 0; ch < length; ch++) {
                                if (ch % 2 == 0)
                                    putchar('|');
                                else
                                    putchar('-');
                            }
                        }
                    }
                    else { /* in between */
                        if (length % 2 == 0) {
                            for (int ch = 0; ch < length; ch++) {
                                if (ch % 2 == 0)
                                    putchar(' ');
                                else
                                    putchar('|');
                            }
                        }
                        else {
                            for (int ch = 0; ch < length; ch++) {
                                if (ch % 2 == 0)
                                    putchar('|');
                                else
                                    putchar(' ');
                            }
                        }
                    }
                }
                putchar('\n');
            }
        }
    }
    else { /* 2d plot */

        if (width < BOTTOM
            || width > TOP
            || height < BOTTOM
            || height > TOP) {
            fprintf(stderr, "Error: Vstup mimo interval!\n");
            return 101;
        }

        if (width % 2 == 0) {
            fprintf(stderr, "Error: Sirka neni liche cislo!\n");
            return 102;
        }

        /* roof */
        for (int i = (width / 2) - 1, right = 1; i > 0; i--, right += 2) {

            /* if first line */
            if (i == (width / 2) - 1) {
                for (int j = 0; j < i + 1; j++) {
                    putchar(' ');
                }
                putchar('X');
                putchar('\n');
            }

            /* print left X */
            for (int j = 0; j < i; j++) {
                putchar(' ');
            }
            putchar('X');

            /* print right X */
            for (int j = 0; j < right; j++) {
                putchar(' ');
            }
            putchar('X');
            putchar('\n');

        }

        /* body */
        for (int i = 0; i < height; i++) {

            /* first and last line */
            if (i == 0 || i == height - 1) {
                for (int j = 0; j < width; j++)
                    putchar('X');
                putchar('\n');
            }
            else { /* in between */
                putchar('X');
                for (int j = 0; j < width - 2; j++) {
                    putchar(' ');
                }
                putchar('X');
                putchar('\n');
            }
        }
    }

    return 0;
}

