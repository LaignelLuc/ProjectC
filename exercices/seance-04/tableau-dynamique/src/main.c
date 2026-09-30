#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    size_t size = 3;
    size_t i;
    int sum = 0;
    int *values = malloc(size * sizeof *values);
    int *temp;

    if (values == NULL) {
        perror("malloc");
        return 1;
    }

    for (i = 0; i < size; i++) {
        values[i] = (int)i + 1;
    }

    /*
     * TODO 1 : passer size à 5, puis utiliser realloc avec un pointeur
     * temporaire. En cas d'échec, libérer values avant de retourner 1.
     */
    size = 5;
    temp = realloc(values, size * sizeof * values);
    if(temp == NULL){
        perror("realloc");
        free(values);
        return 1;
    }
    values = temp;

    /* TODO 2 : initialiser les deux nouveaux éléments à 4 et 5. */
    for(i = 3; i < size; i++){
        values[i] = (int)i + 1;
    }
    /* TODO 3 : parcourir le tableau pour calculer sum. */
    for(i = 3; i < size; i++){
        sum += values[i];
    }
    printf("taille=%zu somme=%d\n", size, sum);
    free(values);
    return 0;
}
