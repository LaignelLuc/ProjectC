#include "projet/statistics.h"

/**
 * Calcule les statistiques d'une seule temperature dans une serie deja
 * construite. Cette fonction ne doit ni allouer de memoire ni modifier la
 * serie : au jalon S3, les mesures sont fournies par un tableau sur la pile.
 *
 * Etapes attendues :
 *
 * 1. Refuser `series == NULL`, `statistics == NULL`, une serie vide, ou une
 *    serie incoherente (`series->items == NULL` alors que `series->count > 0`).
 * 2. Refuser une valeur de `kind` differente de TEMPERATURE_INDOOR et
 *    TEMPERATURE_OUTDOOR.
 * 3. Pour chaque element de `series->items`, choisir soit
 *    `indoor_temperature`, soit `outdoor_temperature` selon `kind`.
 * 4. Initialiser minimum, maximum et la somme avec la premiere valeur, puis
 *    parcourir les valeurs restantes pour mettre a jour minimum, maximum et
 *    la somme.
 * 5. En cas de succes, affecter `statistics->count`, `statistics->minimum`,
 *    `statistics->maximum` et `statistics->average` (somme / count), puis
 *    retourner 1. Retourner 0 dans tous les cas invalides.
 */

static double temperature_at(const MeasureSeries *series, size_t i,TemperatureKind kind)
{
    if (kind == TEMPERATURE_INDOOR) {
        return series->items[i].indoor_temperature;
    }
    return series->items[i].outdoor_temperature;
}

int temperature_statistics_compute(
    const MeasureSeries *series,
    TemperatureKind kind,
    TemperatureStatistics *statistics
)
{
    double mini;
    double max;
    double sum;
    if(series == NULL || statistics == NULL || series->count == 0 || series->items == NULL){
        return 0;
    }
    if(kind != TEMPERATURE_INDOOR && kind != TEMPERATURE_OUTDOOR){
        return 0;
    }

    mini = temperature_at(series, 0, kind);
    max = mini;
    sum = mini;

    for(size_t i = 1; i < series->count; i++){
        double temp = temperature_at(series, i, kind);

        if (temp < mini) {
            mini = temp;
        }
        if (temp > max) {
            max = temp;
        }
        sum += temp;
    }

    statistics->count = series->count;
    statistics->minimum = mini;
    statistics->maximum = max;
    statistics->average = sum / series->count;

    (void)series;
    (void)kind;
    (void)statistics;

    /* TODO S3 : suivre les etapes decrites au-dessus. */
    return 1;
}

int temperature_average_gap(const MeasureSeries *series, double *gap)
{
    TemperatureStatistics indoor;
    TemperatureStatistics outdoor;

    if (gap == NULL) {
        return 0;
    }
    if (!temperature_statistics_compute(series, TEMPERATURE_INDOOR, &indoor)
        || !temperature_statistics_compute(series, TEMPERATURE_OUTDOOR, &outdoor)) {
        return 0;
    }
    *gap = indoor.average - outdoor.average;
    return 1;
}
