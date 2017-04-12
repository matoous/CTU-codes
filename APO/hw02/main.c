#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned char red,green,blue;
} pix;

#define RGB_COMPONENT_COLOR 255

// By Matous Dzivjak
// Dont be lame, make your own
int main(int argc, char* argv[]){

    // open given file
    FILE *fp = fopen(argv[1], "rb");

    int x, y, n_;
    // read image size information
    if (fscanf(fp, "P6 %d %d %d\n", &x, &y, &n_) != 3) return 1;
    int xy = x*y;

    //memory allocation for pixel data
    pix *img = (pix*)malloc(xy * sizeof(pix));
    if(!fread(img, 3*x, y, fp)) return 1;

    // close input file
    fclose(fp);

    // new img create
    pix* new_img = (pix*)malloc(xy*sizeof(pix));

    // histogram
    int histogram[] = {0, 0, 0, 0, 0, 0};

    // NEW IMAGE
    // first row
    int i = 0, r;
    int value1, value2, value3;
    for(;i < x; i++){
        new_img[i] = img[i];
        r = (0.2126*new_img[i].red + 0.7152*new_img[i].green + 0.0722*new_img[i].blue + 0.5)/51;
        histogram[r]++;
    }
    // body - rows
    register short u = 2;
    for(;i < xy-y; i++, u++){
        // copy first pixel
        new_img[i] = img[i];
        r = (0.2126*new_img[i].red + 0.7152*new_img[i].green + 0.0722*new_img[i].blue + 0.5)/51;
        histogram[r]++;
        i++;
        // body
        for(; i < (x*u)-1; i++){
            value1 = 5*img[i].red - img[i-1].red - img[i+1].red - img[i-x].red - img[i+x].red;
            value2 = 5*img[i].green - img[i-1].green - img[i+1].green - img[i-x].green - img[i+x].green;
            value3 = 5*img[i].blue - img[i-1].blue - img[i+1].blue - img[i-x].blue - img[i+x].blue;
            new_img[i].red = (value1 > 0) ? ((value1 < 256) ? (value1) : (255)) : (0);
            new_img[i].green = (value2 > 0) ? ((value2 < 256) ? (value2) : (255)) : (0);
            new_img[i].blue = (value3 > 0) ? ((value3 < 255) ? (value3) : (255)) : (0);
            r = (0.2126*new_img[i].red + 0.7152*new_img[i].green + 0.0722*new_img[i].blue + 0.5)/51;
            histogram[r]++;
        }
        // copy last pixel
        new_img[i] = img[i];
        r = (0.2126*new_img[i].red + 0.7152*new_img[i].green + 0.0722*new_img[i].blue + 0.5)/51;
        histogram[r]++;
    }

    // last row
    while(i < x*y){
        new_img[i] = img[i];
        r = (0.2126*new_img[i].red + 0.7152*new_img[i].green + 0.0722*new_img[i].blue + 0.5)/51;
        histogram[r]++;
        i++;
    }
    free(img);

    // write output.ppm
    fp = fopen("output.ppm", "wb");
    fprintf(fp, "P6\n%d\n%d\n255\n",x,y);
    fwrite(new_img, 3 * x, y, fp);
    fclose(fp);

    // free memory
    free(new_img);

    // write histogram to file
    fp = fopen("output.txt", "wb");
    fprintf(fp, "%d %d %d %d %d ", histogram[0], histogram[1], histogram[2], histogram[3], histogram[4] + histogram[5]);
    fclose(fp);
}
