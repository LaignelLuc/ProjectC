#include "projet/report.h"

#include <stdlib.h>
#include <string.h>

#include "projet/statistics.h"

int sensor_report_options_init(SensorReportOptions *options, const char *label)
{
    /* TODO S4 : verifier le libelle, le copier avec strlen/malloc/memcpy,
     * puis affecter la copie a options->label. Refuser ',', '"', '\n' et
     * '\r'. Ce stub accepte un libelle non nul pour que le chargement des
     * donnees des jalons precedents reste executable avant l'etape S4. */
    size_t size;
    char *copy;

    if (options == NULL) {
        return 0;
    }

    options->label = NULL;

    if(label == NULL || label[0] == '\0' || strpbrk(label, ",\"\n\r") != NULL){
        return 0;
    }

    size = strlen(label) + 1;
    copy = malloc(size);
    if (copy == NULL) {
        return 0;
    }
    memcpy(copy, label, size);
    options->label = copy;
    
    return 1;
}

void sensor_report_options_clear(SensorReportOptions *options)
{
    /* TODO S4 : liberer options->label et remettre le pointeur a NULL. */
    if(options == NULL){
        return;
    }
    free(options->label);
    options->label = NULL;
}

void sensor_report_print(
    FILE *output,
    const SensorDataset *dataset,
    const SensorReportOptions *options
){
    /* TODO S4: afficher le rapport a partir des statistiques. */
    TemperatureStatistics indoor;
    TemperatureStatistics outdoor;

    if(output == NULL || dataset == NULL || options == NULL || options->label == NULL){
        return;
    }

    fprintf(output, "Rapport : %s\n", options->label);
    fprintf(output, "Mesures: %zu\n", dataset->points.count);
    fprintf(output, "Mise a jour: %s\n", dataset->updated_at);

    if (!temperature_statistics_compute(&dataset->points, TEMPERATURE_INDOOR, &indoor)
    || !temperature_statistics_compute(&dataset->points, TEMPERATURE_OUTDOOR, &outdoor)) {
    return;
    }

    fprintf(output, "Temperature interieure: min=%.2f max=%.2f moyenne=%.2f\n",indoor.minimum, indoor.maximum, indoor.average);
    fprintf(output, "Temperature exterieure: min=%.2f max=%.2f moyenne=%.2f\n",outdoor.minimum, outdoor.maximum, outdoor.average);
    fprintf(output, "Ecart interieur-exterieur moyen: %.2f\n", indoor.average - outdoor.average);
}

int sensor_report_write_csv(
    FILE *output,
    const SensorDataset *dataset,
    const SensorReportOptions *options
)
{
    /* TODO S4 : calculer les statistiques et ecrire l'en-tete CSV puis la
     * ligne de synthese. Utiliser fprintf et verifier ses valeurs de retour. */
    
    if(output == NULL || dataset == NULL || options == NULL || options->label == NULL){
        return 0;
    }

    TemperatureStatistics indoor;
    TemperatureStatistics outdoor;

    if (!temperature_statistics_compute(&dataset->points, TEMPERATURE_INDOOR, &indoor)
    || !temperature_statistics_compute(&dataset->points, TEMPERATURE_OUTDOOR, &outdoor)) {
        return 0;
    }

    if (fprintf(output,
            "label,updated_at,count,indoor_minimum,indoor_maximum,indoor_average,"
            "outdoor_minimum,outdoor_maximum,outdoor_average,average_gap\n") < 0) {
        return 0;
    }

    if (fprintf(output, "%s,%s,%zu,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n", 
        options->label, dataset->updated_at, dataset->points.count, indoor.minimum, indoor.maximum, indoor.average,
        outdoor.minimum, outdoor.maximum, outdoor.average, indoor.average - outdoor.average) < 0) {
        return 0;
    }
    return 1;
}
