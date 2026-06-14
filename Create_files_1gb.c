#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FILE_SIZE_MB 1024
#define QUINTITY 100000
void create_file(const char *filename, size_t file_size_mb) {
    const int buffer_size = 8192;
    unsigned char buffer[buffer_size];

    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("ERROR");
        exit(EXIT_FAILURE);
    }


    for (size_t i = 0; i < buffer_size; ++i)
        buffer[i] = rand() % 256;


    long blocks_to_write = (long)(file_size_mb * 1024 * 1024 / buffer_size);

    for (long block = 0; block < blocks_to_write; ++block) {
        fwrite(buffer, sizeof(unsigned char), buffer_size, file);
    }

    fclose(file);
}

int main(int argc, char **argv) {
    srand(time(NULL));

    printf("Create files...\n");

    for (int i = 0; i < QUINTITY; ++i) {
        char filename[50];
        snprintf(filename, sizeof(filename), "Rev0x1337_%d.skogen", i + 1);
        create_file(filename, FILE_SIZE_MB);
    }

    return EXIT_SUCCESS;
}
