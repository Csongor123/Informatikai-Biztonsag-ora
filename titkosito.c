#include <stdio.h>
#include <stdlib.h>

// Meghatározza, hogy egyszerre legfeljebb hány bájtot olvasson be a program
#define BUFFER_SIZE 1024

/* Egyszerű belső kulcs ebből  állítja elő a kulcsbájtokat */
static unsigned int GLOBAL_KEY = 0xA5B35791;
// Ez a kezdeti kulcs, amiből a program a titkosításhoz használt kulcsbájtokat előállítja

/*  kulcsbájt generálása */
unsigned char next_key_byte(unsigned int *state, long index) {


    *state ^= (*state << 13);
    *state ^= (*state >> 17);
    *state ^= (*state << 5);



    *state += (unsigned int)(index * 31 + 17);

    // Az állapot legalacsonyabb 8 bitjét adjuk vissza egyetlen bájtként.
    return (unsigned char)(*state & 0xFF);
}

/* Fájl titkosítása */
int process_file(const char *input_name, const char *output_name) {
    // Bemeneti fájl megnyitása bináris olvasásra
    FILE *input = fopen(input_name, "rb");

    // Kimeneti fájl megnyitása bináris írásra
    FILE *output = fopen(output_name, "wb");


    unsigned char buffer[BUFFER_SIZE];


    size_t bytes_read;

    // A teljes fájlban lévő aktuális bájt pozíciója
    long global_index = 0;


    unsigned int state = GLOBAL_KEY;

    // Az előző titkosított bájt tárolása a láncolt működéshez
    unsigned char previous = 0;

    // Ellenőrizzük, hogy sikerült-e megnyitni a bemeneti fájlt
    if (input == NULL) {
        printf("Hiba: a bemeneti fajl nem nyithato meg: %s\n", input_name);
        return 1;
    }

    // Ellenőrizzük, hogy sikerült-e létrehozni a kimeneti fájlt
    if (output == NULL) {
        printf("Hiba: a kimeneti fajl nem hozhato letre: %s\n", output_name);
        fclose(input);
        return 1;
    }

    // Addig olvasunk a fájlból, amíg van benne adat
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, input)) > 0) {
        size_t i;

        // A beolvasott blokk minden bájtját külön feldolgozzuk
        for (i = 0; i < bytes_read; i++) {
            // Következő kulcsbájt előállítása az aktuális állapotból történik
            unsigned char key_byte = next_key_byte(&state, global_index);

            // Az eredeti aktuális bájt eltárolása
            unsigned char current = buffer[i];

            // Titkosítás XOR műveletekkel:
            // - aktuális eredeti bájt
            // - kulcsbájt
            // - pozíció maradéka 256-tal
            // - előző titkosított bájt
            buffer[i] = current ^ key_byte ^ (unsigned char)(global_index % 256)
                        ^ previous;

            // A most elkészült titkosított bájt lesz a következő körben az "előző"
            previous = buffer[i];

            // Lépünk a következő bájt pozíciójára
            global_index++;
        }

        // Kimeneti fájlba kiíratás
        fwrite(buffer, 1, bytes_read, output);
    }

    // Fájlok lezárása
    fclose(input);
    fclose(output);

    // Sikeres futás
    return 0;
}

/* Visszafejtés*/
int decrypt_file(const char *input_name, const char *output_name) {
    // A titkosított fájl megnyitása olvasásra
    FILE *input = fopen(input_name, "rb");

    // A visszafejtett fájl megnyitása írásra
    FILE *output = fopen(output_name, "wb");

    // Puffer a beolvasott adatok tárolására
    unsigned char buffer[BUFFER_SIZE];

    // Az aktuálisan beolvasott bájtok száma
    size_t bytes_read;

    // A teljes fájlban lévő aktuális bájt pozíciója
    long global_index = 0;

    // A kulcsgenerálás belső állapota, ugyanazzal a kezdőkulccsal indul
    unsigned int state = GLOBAL_KEY;

    // Az előző titkosított bájt tárolása
    unsigned char previous = 0;

    // Titkosított bemeneti fájl ellenőrzése, hogy ugyan az -e
    if (input == NULL) {
        printf("Hiba: a titkositott fajl nem nyithato meg: %s\n", input_name);
        return 1;
    }

    // Kimeneti fájl ellenőrzése, hogy létrejött -e
    if (output == NULL) {
        printf("Hiba: a kimeneti fajl nem hozhato letre: %s\n", output_name);
        fclose(input);
        return 1;
    }

    // Addig olvassa, amíg van még a titkosított fájlban adat
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, input)) > 0) {
        size_t i;

        // A beolvasott blokk minden bájtját visszafejti külön-külön
        for (i = 0; i < bytes_read; i++) {


            unsigned char encrypted = buffer[i];

            // Előállítjuk ugyanazt a kulcsbájtot, mint a titkosításkor
            unsigned char key_byte = next_key_byte(&state, global_index);

            // Visszafejtés ugyanazzal az XOR művelettel:
            // titkosított bájt XOR kulcs XOR pozíció XOR előző titkosított bájt
            buffer[i] = encrypted ^ key_byte
                        ^ (unsigned char)(global_index % 256)
                        ^ previous;


            previous = encrypted;

            // Következő bájt pozíciójára lépünk
            global_index++;
        }

        // A visszafejtett blokk kiírása a kimeneti fájlba
        fwrite(buffer, 1, bytes_read, output);
    }

    // Fájlok lezárása
    fclose(input);
    fclose(output);

    // Sikeres futás
    return 0;
}

int main(void) {

    const char *eredeti = "dokumentum.txt";


    const char *titkositott = "titkositott.bin";


    const char *visszafejtett = "visszafejtett.txt";

    // Először titkosítjuk az eredeti fájlt
    if (process_file(eredeti, titkositott) == 0) {
        printf("A dokumentum titkositasa sikeres: %s\n", titkositott);
    } else {
        return 1; // Hiba esetén kilépünk
    }

    // Ezután visszafejtjük a titkosított fájlt
    if (decrypt_file(titkositott, visszafejtett) == 0) {
        printf("A dokumentum visszafejtese sikeres: %s\n", visszafejtett);
    } else {
        return 1; // Hiba esetén kilépünk
    }

    // A program sikeresen lefutott
    return 0;
}
