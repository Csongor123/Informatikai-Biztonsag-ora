#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

/* Egyszerû belsõ kulcs */
static unsigned int GLOBAL_KEY = 0xA5B35791;

/* Álvéletlen kulcsbájt generálása */
unsigned char next_key_byte(unsigned int *state, long index) {
    *state ^= (*state << 13);
    *state ^= (*state >> 17);
    *state ^= (*state << 5);
    *state += (unsigned int)(index * 31 + 17);

    return (unsigned char)(*state & 0xFF);
}

/* Fájl titkosítása vagy visszafejtése */
int process_file(const char *input_name, const char *output_name) {
    FILE *input = fopen(input_name, "rb");
    FILE *output = fopen(output_name, "wb");

    unsigned char buffer[BUFFER_SIZE];
    size_t bytes_read;
    long global_index = 0;
    unsigned int state = GLOBAL_KEY;
    unsigned char previous = 0;

    if (input == NULL) {
        printf("Hiba: a bemeneti fajl nem nyithato meg: %s\n", input_name);
        return 1;
    }

    if (output == NULL) {
        printf("Hiba: a kimeneti fajl nem hozhato letre: %s\n", output_name);
        fclose(input);
        return 1;
    }

    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, input)) > 0) {
        size_t i;

        for (i = 0; i < bytes_read; i++) {
            unsigned char key_byte = next_key_byte(&state, global_index);
            unsigned char current = buffer[i];

            buffer[i] = current ^ key_byte ^ (unsigned char)(global_index % 256)
                        ^ previous;

            previous = buffer[i];
            global_index++;
        }

        fwrite(buffer, 1, bytes_read, output);
    }

    fclose(input);
    fclose(output);

    return 0;
}

/* Visszafejtéshez külön függvény, mert a láncolás miatt más a sorrend */
int decrypt_file(const char *input_name, const char *output_name) {
    FILE *input = fopen(input_name, "rb");
    FILE *output = fopen(output_name, "wb");

    unsigned char buffer[BUFFER_SIZE];
    size_t bytes_read;
    long global_index = 0;
    unsigned int state = GLOBAL_KEY;
    unsigned char previous = 0;

    if (input == NULL) {
        printf("Hiba: a titkositott fajl nem nyithato meg: %s\n", input_name);
        return 1;
    }

    if (output == NULL) {
        printf("Hiba: a kimeneti fajl nem hozhato letre: %s\n", output_name);
        fclose(input);
        return 1;
    }

    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, input)) > 0) {
        size_t i;

        for (i = 0; i < bytes_read; i++) {
            unsigned char encrypted = buffer[i];
            unsigned char key_byte = next_key_byte(&state, global_index);

            buffer[i] = encrypted ^ key_byte
                        ^ (unsigned char)(global_index % 256)
                        ^ previous;

            previous = encrypted;
            global_index++;
        }

        fwrite(buffer, 1, bytes_read, output);
    }

    fclose(input);
    fclose(output);

    return 0;
}

int main(void) {
    const char *eredeti = "dokumentum.txt";
    const char *titkositott = "titkositott.bin";
    const char *visszafejtett = "visszafejtett.txt";

    if (process_file(eredeti, titkositott) == 0) {
        printf("A dokumentum titkositasa sikeres: %s\n", titkositott);
    } else {
        return 1;
    }

    if (decrypt_file(titkositott, visszafejtett) == 0) {
        printf("A dokumentum visszafejtese sikeres: %s\n", visszafejtett);
    } else {
        return 1;
    }

    return 0;
}
