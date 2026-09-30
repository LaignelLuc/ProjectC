#include <stdio.h>
#include <stdlib.h>
// stdint pour SIZE_MAX
#include <stdint.h>
#include <string.h>

enum {
    TEXTE_MAX = 32,
    LIGNE_MAX = 128
};

struct Personne {
    char prenom[TEXTE_MAX];
    char nom[TEXTE_MAX];
    int jour;
    int mois;
    int annee;
};

struct Annuaire {
    // taille correspond au nombre de personnes actuellement dans l'annuaire
    size_t taille;
    // Capacité va servir pour l'allocation dynamique : si taille > capacité, on doit réallouer en mémoire
    size_t capacite;
    // Liste de personnes
    struct Personne *personnes;
};

void init_annuaire(struct Annuaire *annuaire)
{
    annuaire->personnes = NULL;
    annuaire->taille = 0;
    annuaire->capacite = 0;
}

static void afficher_annuaire(const struct Annuaire *annuaire)
{
    size_t i;

    for (i = 0; i < annuaire->taille; i++) {
        const struct Personne *personne = &annuaire->personnes[i];

        printf("%s %s (%02d/%02d/%04d)\n", personne->prenom, personne->nom,
               personne->jour, personne->mois, personne->annee);
    }
}

// Fonction pour ajouter dynamiquement une personne à l'annuaire
int ajouter_personne(struct Annuaire *annuaire, struct Personne personne)
{
    // Si annuaire pointe vers NULL
    if (annuaire == NULL) {
        return 1;
    }
    
    // Liste de personne, lorsque l'on doit malloc ou realloc
    struct Personne *pers_redim;

    // Si on a atteint la capacité en mémoire, on doit réallouer pour pouvoir ajouter une autre personne
    if (annuaire->taille == annuaire->capacite) {
        // Prochaine capacité 
        size_t proc_capa;

        // On vient juste d'initialiser, aucune personne n'a encore été ajoutée et aucune zone mémoire n'a été réservée
        if (annuaire->capacite == 0) 
        {
            proc_capa = 4;
            // malloc : première allocation mémoire
            pers_redim = malloc(proc_capa * sizeof *pers_redim);
        } 
        
        // On a déjà alloué en mémoire, il n'y a plus assez de place pour ajouter une nouvelle personne
        else {
            // On a atteint la taille max possible réservable en mémoire
            if (annuaire->capacite > SIZE_MAX / 2) {
                return 1;
            }

            // On va realloc 2 fois plus de mémoire qu'avant
            proc_capa = annuaire->capacite * 2;
            // On dépasse la limite mémoire avec la prochaine capacité
            if (proc_capa > SIZE_MAX / sizeof *pers_redim) {
                return 1;
            }

            // Realloc 
            pers_redim = realloc(annuaire->personnes, proc_capa * sizeof *pers_redim);
        }
        // Si le malloc ou le realloc s'est mal passé
        if (pers_redim == NULL) {
            return 1;
        }
        // Si c'est ok, on met à jour les champs de annuaire
        annuaire->personnes = pers_redim;
        annuaire->capacite = proc_capa;
    }
    // On ajoute la prochaine personne à l'annuaire 
    annuaire->personnes[annuaire->taille] = personne;
    // On augmente la taille de 1
    annuaire->taille += 1;
    return 0;
}

static void liberer_annuaire(struct Annuaire *annuaire)
{
    free(annuaire->personnes);
    //annuaire->personnes = NULL;
    annuaire->taille = 0;
    annuaire->capacite = 0;
}


static int ecrire_csv(const struct Annuaire *annuaire, const char *chemin)
{
    /* TODO : ouvrir chemin en écriture, écrire chaque personne et fermer. */
    int ecrit;

    // On vérifie qu'on a pas des pointeurs nuls
    if (annuaire == NULL || chemin == NULL) {
        return 1;
    }
    FILE *f_csv = fopen(chemin, "w");
    if(f_csv==NULL)
    {
        return 1;
    }

    // On écrit avec fprintf
    // La première ligne est le nom des colonnes
    ecrit = fprintf(f_csv,"Prenom, Nom, Date naissance(jj/mm/aaaa)\n");
    // Echec de l'écriture
    if(ecrit == 1)
    {
        return 1;
    }
    // On écrit chaque personne avec un boucle for
    int i = 0;
    for(i=0; i<(int)annuaire->taille; i++)
    {
        ecrit = fprintf(f_csv, "%s, %s, %d/%d/%d\n", annuaire->personnes[i].prenom, annuaire->personnes[i].nom,
                        annuaire->personnes[i].jour, annuaire->personnes[i].mois,annuaire->personnes[i].annee);
        // Echec de l'écriture
        if(ecrit == 1)
        {
            return 1;
        }
    }

    // On ferme et on vérifie que c'est ok
    if(fclose(f_csv) == 1)
    {
        return 1;
    }
    return 0;
}

static int lire_csv(struct Annuaire *annuaire, const char *chemin)
{
    /*
     * TODO : ouvrir chemin en lecture, lire chaque ligne avec fgets, analyser
     * le CSV avec sscanf, agrandir annuaire->personnes avec realloc, puis fermer.
     */

    // Ouverture fichier, en mode read 
    FILE *f_csv = fopen(chemin, "r");
    if(f_csv==NULL)
    {
        return 1;
    }

    // Lecture ligne par ligne avec fgets
    // On saute la première ligne (noms des colonnes)
    char ligne[4096];
    fgets(ligne, 4096,f_csv);
    // Variable pour ajouter une personne lue à l'annuaire
    struct Personne personne;
    int ajout; 
    // Parcours du fichier
    while(fgets(ligne, 4096,f_csv))
    {
        // Découpage de la ligne avec sscanf, le second argument est le format de la ligne
        sscanf(ligne, "%s %s %d/%d/%d\n", personne.prenom, personne.nom, &personne.jour,
               &personne.mois, &personne.annee);

        // Le dernier caractère (la virgule) est supprimée du nom et prénom de la personne
        personne.prenom[strlen(personne.prenom) - 1] = '\0';
        personne.nom[strlen(personne.nom) - 1] = '\0';
        // Ajout d'un personne, de manière dynamique
        ajout = ajouter_personne(annuaire, personne);
        if(ajout == 1)
        {
            return 1;
        }
    }

    // Fermeture fichier
    if(fclose(f_csv) == 1)
    {
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    struct Personne personnes[] = {
        {"Ada", "Lovelace", 10, 12, 1815},
        {"Alan", "Turing", 23, 6, 1912}
    };
    struct Annuaire source = {2, 4, personnes};

    // Initialisation de l'annuaire reconstruit
    struct Annuaire reconstruit;
    init_annuaire(&reconstruit);
    // int ret_ajout = ajouter_personne(&reconstruit, personnes[0]);
    // ret_ajout = ajouter_personne(&reconstruit, personnes[1]);
    if (argc != 2) {
        fprintf(stderr, "usage: %s FICHIER.csv\n", argv[0]);
        return 1;
    }

    int ret_ecrit = ecrire_csv(&source, argv[1]);
    // Si l'écriture s'est mal passée, échec
    if (ret_ecrit == 1) {
        return 1;
    }

    int ret_lire = lire_csv(&reconstruit, argv[1]);
    // Si la lecture s'est mal passée, échec
    if (ret_lire == 1) {
        liberer_annuaire(&reconstruit);
        return 1;
    }
    //afficher_annuaire(&source);
    afficher_annuaire(&reconstruit);
    // liberer_annuaire(&source);
    liberer_annuaire(&reconstruit);
    return 0;
}
