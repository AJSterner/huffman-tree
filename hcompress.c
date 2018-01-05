#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main (int argc, char *argv[])
{
    assert(argc == 2);
    FILE *f = fopen(argv[1], "r");
    unsigned counts[256];

    for (int i = 0; i < 256; i++) {
        counts[i] = 0;
    }

    int c;
    do {
        c = fgetc(f);
        if (c != EOF) {
            counts[c]++;
        }
    } while(c != EOF);
    
    for (unsigned i = 0; i < 256; i++){
        
        if (counts[i] > 0) 
            printf("%u %u\n", i, counts[i]);
    }
    fclose(f);
    return EXIT_SUCCESS;
}


