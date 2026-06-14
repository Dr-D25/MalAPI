#include <stdio.h>
#include <string.h>

#define QUINTITY 10000000
#define FILENAME_PREFIX "Rev0x1337_"

int main() {
    for (int i = 0; i < QUINTITY; ++i) {
        char filename[100];

        snprintf(filename, sizeof(filename), "%s%d.txt", FILENAME_PREFIX, i);


        FILE *fp = fopen(filename, "w");
        if (!fp) {
            perror("ERROR");
            continue;
        }

        fclose(fp);
    }

    printf("%d DONE.\n", QUINTITY);
    return 0;
}
