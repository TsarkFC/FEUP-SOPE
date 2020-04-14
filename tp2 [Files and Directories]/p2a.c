#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//COPIES CONTENT FROM ONE FILE TO ANOTHER

int main(int argc, char *argv[]){
    //2.a)
    FILE *src, *dst;
    char buf[256];

    char *destination = argv[2];
    char *source = argv[1];

    dst = fopen(destination, "w");
    src = fopen(source, "r");

    while (fread(buf, 38, 1, src)){
        fwrite(buf, 38, 1, dst);
    }

    fclose(dst);
    fclose(src);
}