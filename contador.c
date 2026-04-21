#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define PASADAS 100  // repetir la busqueda N veces para exigir CPU

typedef struct {
    char *contenido;
    long inicio;
    long fin;
    char *palabra;
    int  len_palabra;
    long conteo;
} Fragmento;

void *contar(void *arg) {
    Fragmento *f = (Fragmento *)arg;
    long total = 0;
    long fin_ext = f->fin + f->len_palabra - 1;

    for (int p = 0; p < PASADAS; p++) {
        for (long i = f->inicio; i < fin_ext - f->len_palabra + 1; i++) {
            if (strncmp(&f->contenido[i], f->palabra, f->len_palabra) == 0)
                total++;
        }
    }

    f->conteo = total / PASADAS;  // resultado real sin multiplicar
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Uso: %s <archivo> <palabra> <num_hilos>\n", argv[0]);
        return 1;
    }

    char *archivo  = argv[1];
    char *palabra  = argv[2];
    int num_hilos  = atoi(argv[3]);
    int len_pal    = strlen(palabra);

    if (num_hilos < 1 || num_hilos > 16) {
        printf("Error: num_hilos debe estar entre 1 y 16\n");
        return 1;
    }

    FILE *fp = fopen(archivo, "r");
    if (!fp) { perror("Error abriendo archivo"); return 1; }

    fseek(fp, 0, SEEK_END);
    long tam = ftell(fp);
    rewind(fp);

    char *contenido = malloc(tam + len_pal);
    long leido = fread(contenido, 1, tam, fp);
    memset(contenido + leido, 0, len_pal);
    fclose(fp);

    printf("Archivo: %s (%ld bytes)\n", archivo, tam);
    printf("Palabra: \"%s\" | Hilos: %d | Pasadas: %d\n", palabra, num_hilos, PASADAS);

    pthread_t *hilos      = malloc(num_hilos * sizeof(pthread_t));
    Fragmento *fragmentos = malloc(num_hilos * sizeof(Fragmento));
    long tam_frag = tam / num_hilos;

    for (int i = 0; i < num_hilos; i++) {
        fragmentos[i].contenido   = contenido;
        fragmentos[i].palabra     = palabra;
        fragmentos[i].len_palabra = len_pal;
        fragmentos[i].inicio      = i * tam_frag;
        fragmentos[i].fin         = (i == num_hilos - 1) ? tam : (i + 1) * tam_frag;
        fragmentos[i].conteo      = 0;
        pthread_create(&hilos[i], NULL, contar, &fragmentos[i]);
    }

    long total = 0;
    for (int i = 0; i < num_hilos; i++) {
        pthread_join(hilos[i], NULL);
        total += fragmentos[i].conteo;
    }

    printf("Total apariciones: %ld\n", total);

    free(hilos);
    free(fragmentos);
    free(contenido);
    return 0;
}
