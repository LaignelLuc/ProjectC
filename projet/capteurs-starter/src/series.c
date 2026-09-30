#include "projet/series.h"
#include <stdlib.h>

void measure_series_init(MeasureSeries *series)
{
    series->items = NULL;
    series->count = 0;
    series->capacity = 0;
}

int measure_series_append(MeasureSeries *series, TemperatureMeasure measure)
{
    if (series == NULL) {
        return 0;
    }
    if (series->count == series->capacity) {
        if (series->items == NULL) {
            /* Premier ajout : on reserve 1 case. */
            series->items = malloc(sizeof(TemperatureMeasure));
            if (series->items == NULL) {
                return 0;
            }
            series->capacity = 1;
        } else {
            /* Tableau plein : on double sa taille. */
            TemperatureMeasure *tmp = realloc(series->items,
                series->capacity * 2 * sizeof(TemperatureMeasure));
            if (tmp == NULL) {
                return 0;
            }
            series->items = tmp;
            series->capacity = series->capacity * 2;
        }
    }

    series->items[series->count] = measure;
    series->count++;
    return 1;
    /* TODO S4: malloc lors de la premiere insertion, puis realloc si besoin. */
}

void measure_series_clear(MeasureSeries *series)
{
    free(series->items);
    measure_series_init(series);

    /* TODO S4: liberer le tableau et remettre la serie dans son etat initial. */
}
