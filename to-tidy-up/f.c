#include <exec/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HEADER_CARDS 256  // Máx. 256 tarjetas de 80 bytes = 20 KB
#define CARD_SIZE        80
#define BLOCK_SIZE       2880
#define HEADER_MAX (MAX_HEADER_CARDS * CARD_SIZE)

int read_header(FILE *f, char *header_buf, size_t buf_size, size_t *header_bytes) {
    char card[CARD_SIZE];
    size_t total = 0;
    int found_end = 0;

    while (1) {
        if ((total + CARD_SIZE) > buf_size) {
            printf("Error: se excedió el búfer de cabecera (%lu bytes)\n", buf_size);
            return 1;
        }

        size_t n = fread(card, 1, CARD_SIZE, f);
        if (n != CARD_SIZE) {
            printf("Error: no se pudo leer una tarjeta completa\n");
            return 2;
        }

        for (int i = 0; i < CARD_SIZE; i++) {
            header_buf[total + i] = card[i];
        }

        total += CARD_SIZE;

        if (strncmp(card, "END", 3) == 0) {
            found_end = 1;
            break;
        }
    }

    if (!found_end) {
        printf("Error: tarjeta END no encontrada\n");
        return 3;
    }
    /*
    // Saltar padding si es necesario
    size_t pad = (BLOCK_SIZE - (total % BLOCK_SIZE)) % BLOCK_SIZE;
    if (pad > 0) {
        char dummy[BLOCK_SIZE];
        if (pad > sizeof(dummy)) pad = sizeof(dummy);  // safety
        fread(dummy, 1, pad, f);
        total += pad;
    }

    *header_bytes = total;
    */
    return 0;
}


LONG get_value(const char *header, size_t hdr_bytes, const char *key) {
    for (size_t i = 0; i + CARD_SIZE <= hdr_bytes; i += CARD_SIZE) {
        if (strncmp(header + i, key, strlen(key)) == 0 && header[i+8] == '=') {
            char val[70];
            memcpy(val, header + i + 10, CARD_SIZE - 10);
            val[CARD_SIZE - 10] = '\0';
            return atol(val);
        }
    }
    printf("Clave '%s' no encontrada en cabecera\n", key);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s archivo.fits\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "rb");
    if (!f) { printf("No se pudo abrir FITS\n"); return 1; }

    char header_buf[HEADER_MAX];
    size_t header_bytes = 0;
    read_header(f, header_buf, HEADER_MAX, &header_bytes);
    /* 
    LONG bitpix = get_value(header, hdr_bytes, "BITPIX");
    LONG naxis  = get_value(header, hdr_bytes, "NAXIS");

    if (naxis < 1 || naxis > 3) {
        printf("Solo soportado NAXIS = 1–3, este es %ld\n", naxis);
        return 1;
    }

    LONG naxes[3];
    for (int i = 0; i < naxis; i++) {
        char key[16];
        sprintf(key, "NAXIS%d", i+1);
        naxes[i] = get_value(header, hdr_bytes, key);
    }

    printf("BITPIX = %ld, NAXIS = %ld, dimensiones =", bitpix, naxis);
    for (int i = 0; i < naxis; i++) printf(" %ld", naxes[i]);
    printf("\n");

    LONG npixels = naxes[0];
    for (int i = 1; i < naxis; i++) npixels *= naxes[i];

    size_t bytes_per_pix = (bitpix > 0 ? bitpix : -bitpix) / 8;
    size_t data_size = npixels * bytes_per_pix;

    void *data = malloc(data_size);
    if (!data) { printf("Error de memoria\n"); return 1; }

    fread(data, 1, data_size, f);

    if (bitpix == 8) {
        UBYTE *pix = data;
        for (int i = 0; i < 10 && i < npixels; i++)
            printf("%u ", pix[i]);
    } else {
        printf("Formato BITPIX no manejado\n");
    }

    printf("\n");
    free(data);
    */
    fclose(f);
    return 0;
}

