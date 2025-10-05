#include <exec/types.h>
#include <exec/memory.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <dos/dosextens.h>
#include <proto/dos.h>
#include <proto/dos.h>
#include <dos/dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <exec/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define CARD_SIZE   80
#define BLOCK_SIZE  2880
#define MAX_CARDS   256  // Máximo 256 tarjetas: 20 KB de cabecera
#define HEADER_BUF_SIZE (CARD_SIZE * MAX_CARDS)

extern struct ExecBase *SysBase;

/**
 * Lee la cabecera de un archivo FITS usando solo funciones compatibles con Amiga.
 * Usa AllocMem en vez de malloc y copia manualmente.
 */
int read_header(BPTR fh, UBYTE *header_buf, size_t buf_size, size_t *header_bytes) {
    UBYTE card[CARD_SIZE];
    size_t total = 0;
    int found_end = 0;

    while (1) {
        if ((total + CARD_SIZE) > buf_size) {
            printf("Error: búfer insuficiente\n");
            return 1;
        }

        LONG n = Read(fh, card, CARD_SIZE);
        if (n != CARD_SIZE) {
            printf("Error: no se pudo leer tarjeta completa\n");
            return 2;
        }

        // Copia manual
        for (int i = 0; i < CARD_SIZE; i++) {
            header_buf[total + i] = card[i];
        }

        total += CARD_SIZE;

        if (card[0] == 'E' && card[1] == 'N' && card[2] == 'D') {
            found_end = 1;
            break;
        }
    }

    if (!found_end) {
        printf("Error: tarjeta END no encontrada\n");
        return 3;
    }

    // Saltar padding
    size_t pad = (BLOCK_SIZE - (total % BLOCK_SIZE)) % BLOCK_SIZE;
    if (pad > 0) {
        UBYTE dummy[BLOCK_SIZE];
        if (pad > sizeof(dummy)) pad = sizeof(dummy);
        Read(fh, dummy, pad);
        total += pad;
    }

    *header_bytes = total;
    return 0;
}


LONG find_keyword_value(UBYTE *header, size_t hdr_bytes, const char *key) {
    const int CARD_LEN = 80;
    const int KEY_LEN = strlen(key);

    printf("Buscando clave: '%s'\n", key);

    for (size_t offset = 0; offset + CARD_LEN <= hdr_bytes; offset += CARD_LEN) {
        char *card = (char *)(header + offset);

        // Mostrar la tarjeta actual para depurar
        char debug_card[81];
        strncpy(debug_card, card, CARD_LEN);
        debug_card[80] = '\0';
        printf("Tarjeta %4lu: [%.8s]\n", offset / 80, debug_card);

        // Coincidencia exacta al principio de la línea
        if (strncmp(card, key, KEY_LEN) == 0 && card[KEY_LEN] == ' ' && card[8] == '=') {
            char valbuf[32];
            int i;

            // Saltar los 10 primeros caracteres: KEY + " = "
            for (i = 0; i < sizeof(valbuf) - 1 && (10 + i) < CARD_LEN; i++) {
                valbuf[i] = card[10 + i];
            }
            valbuf[i] = '\0';

            // Eliminar espacios y barras inclinadas
            for (i = 0; valbuf[i]; i++) {
                if (valbuf[i] == '/' || valbuf[i] == '\'') {
                    valbuf[i] = '\0';
                    break;
                }
            }

            printf("Valor bruto encontrado: '%s'\n", valbuf);
            return atol(valbuf);
        }
    }

    printf("Clave '%s' no encontrada en la cabecera.\n", key);
    return -1;
}


int starts_with(const char *card, const char *key) {
    int i = 0;
    while (key[i] != '\0') {
        if (card[i] != key[i]) {
            return 0; // No coincide
        }
        i++;
    }
    return 1; // Coincide
}


long extract_value(const char *card) {
    char valbuf[32];
    int i, v = 0;

    // El valor empieza normalmente en la posición 10 (índice 9 en C)
    for (i = 10; i < CARD_SIZE; i++) {
        if (card[i] != ' ') { // saltar espacios iniciales
            valbuf[v++] = card[i];
        }
        if (card[i] == '/') break; // parar en comentario
    }
    valbuf[v] = '\0';
    return atol(valbuf); // convertir a número
}

void print_fits_cards(UBYTE *header, size_t hdr_bytes) {
    const int CARD_LEN = 80;
    char card[CARD_LEN + 1];
    const int nCARDS = CARD_LEN * 10; // first 10

    printf("Total bytes en cabecera: %lu\n", (unsigned long)hdr_bytes);
    printf("Tarjetas FITS:\n");
    

    //for (size_t i = 0; i + CARD_LEN <= hdr_bytes; i += CARD_LEN) {
    for (size_t i = 0; i + CARD_LEN <= nCARDS; i += CARD_LEN) {
        for (int j = 0; j < CARD_LEN; j++) {
            card[j] = header[i + j];
        }
        card[CARD_LEN] = '\0';  // Terminador nulo para printf
        printf("%s\n", card);
  
         if (starts_with(card, "BITPIX")) {
           long bitpix = extract_value(card);
           printf("BITPIX encontrado: %ld\n", bitpix);
         } 	
    }
    
}


int main(int argc, char *argv[]) {
     
    ULONG before, after;

    before = AvailMem(MEMF_ANY);
    printf("Memoria libre antes: %lu bytes\n", before);

    if (argc != 2) {
        printf("Uso: programa nombre.fits\n");
        return RETURN_FAIL;
    }

    BPTR fh = Open(argv[1], MODE_OLDFILE);
    
    before = AvailMem(MEMF_ANY);
    printf("Memoria libre antes: %lu bytes\n", before);

    if (!fh) {
        printf("Error al abrir archivo\n");
        return RETURN_FAIL;
    }

    // Reservamos memoria para la cabecera
     
    UBYTE *header_buf = (UBYTE *)AllocMem(HEADER_BUF_SIZE, MEMF_ANY | MEMF_CLEAR);
    if (!header_buf) {
        printf("Error: no hay memoria\n");
        Close(fh);
        return RETURN_FAIL;
    }
    
    before = AvailMem(MEMF_ANY);
    printf("Memoria libre antes: %lu bytes\n", before);

    
    size_t header_bytes = 0;
    int result = read_header(fh, header_buf, HEADER_BUF_SIZE, &header_bytes);

    if (result == 0) {
        printf("Cabecera FITS leída correctamente\n");
    }
   
    print_fits_cards(header_buf,header_bytes);
    //LONG bitpix = find_keyword_value(header_buf, header_bytes, "BITPIX");
    //LONG naxis = find_keyword_value(header_buf, header_bytes, "NAXIS");    
    //printf("BITPIX: %ld",bitpix);
    
    Seek(fh,header_bytes,OFFSET_BEGINNING);

    UBYTE pixbuf[150];
    UBYTE nreads=0;
    nreads=Read(fh,pixbuf,150);
    printf("nreads: %u",(unsigned)nreads);

    for (int i = 0; i < 150; i++) {
        printf("%u ", (unsigned)pixbuf[i]);
    }



    FreeMem(header_buf, HEADER_BUF_SIZE);
    Close(fh);
    return (result == 0) ? RETURN_OK : RETURN_FAIL;
}


