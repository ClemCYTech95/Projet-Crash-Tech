#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PREMIERE_ANNEE 2005
#define NB_ANNEES 20
#define NB_ACCIDENTS 1286097

typedef struct {
    int annee;
    long accidents;
    long indemnes;
    long morts;
    long blesses_graves;
    long blesses_legers;
} Annee;

int lire_champs_utiles(char* ligne, int* annee, long* numero_accident, int* severite)
{
    char* position = ligne;

    for (int colonne = 1; colonne <= 6; ++colonne) {
        char* separateur = strchr(position, ';');
        if (separateur == NULL) { /* Format invalide */
            return 0;
        }

        if (colonne == 1 || colonne == 2 || colonne == 6) { /* on récupère les 3 colonnes */
            char *fin;
            
            long valeur = strtol(position, &fin, 10);
            
            if (position == fin || errno == ERANGE) { /* Format invalide ou erreur de capacité */
                return 0;
            }

            if (colonne == 1) {
                *annee = valeur;
            } else if (colonne == 2) {
                *numero_accident = valeur;
            } else {
                *severite = valeur;
            }
        }

        position = separateur + 1; /* Champ suivant */
    }

    return 1;
}

int main(int argc, char** argv)
{
    const char* nom_fichier = argc > 1
        ? argv[1]
        : "C:\\Users\\CLEMENT\\OneDrive\\Desktop\\test.csv";
    FILE* fichier = fopen(nom_fichier, "r");

    if (fichier == NULL) {
        perror(nom_fichier);
        return EXIT_FAILURE;
    }

    Annee tableau[NB_ANNEES] = {0};
    for (int i = 0; i < NB_ANNEES; ++i) {
        tableau[i].annee = PREMIERE_ANNEE + i;
    }

    char ligne[1024];
    long numero_ligne = 0;
    long nombre_accidents = 0;
    int a_precedent = 0;
    int annee_precedente = 0;
    long long accident_precedent = 0;
    int erreur = 0;

    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        ++numero_ligne;

        int annee_lue;
        long numero_accident;
        int severite;
        if (!lire_champs_utiles(ligne, &annee_lue, &numero_accident, &severite)) {
            if (numero_ligne > 1) {
                fprintf(stderr, "Ligne %ld ignoree : format invalide.\n", numero_ligne);
            }
            continue;
        }

        if (annee_lue < PREMIERE_ANNEE || annee_lue >= PREMIERE_ANNEE + NB_ANNEES
            || severite < 1 || severite > 4) {
            fprintf(stderr, "Ligne %ld ignoree : annee ou severite invalide.\n", numero_ligne);
            continue;
        }

        int annee = annee_lue;

        if (a_precedent
            && (annee < annee_precedente
                || (annee == annee_precedente && numero_accident < accident_precedent))) {
            fprintf(stderr, "Le fichier n'est pas trie a la ligne %ld.\n", numero_ligne);
            erreur = 1;
            break;
        }

        if (!a_precedent || annee != annee_precedente
            || numero_accident != accident_precedent) {
            if (nombre_accidents >= NB_ACCIDENTS) {
                fprintf(stderr, "Le fichier depasse la limite de %d accidents.\n", NB_ACCIDENTS);
                erreur = 1;
                break;
            }
            ++tableau[annee - PREMIERE_ANNEE].accidents;
            ++nombre_accidents;
        }
        a_precedent = 1;
        annee_precedente = annee;
        accident_precedent = numero_accident;

        Annee *resultat = &tableau[annee - PREMIERE_ANNEE];
        switch (severite) {
        case 1:
            ++resultat->indemnes;
            break;
        case 2:
            ++resultat->morts;
            break;
        case 3:
            ++resultat->blesses_graves;
            break;
        case 4:
            ++resultat->blesses_legers;
            break;
        }
    }

    if (ferror(fichier)) {
        perror("Erreur de lecture");
        erreur = 1;
    }
    if (fclose(fichier) != 0) {
        perror("Erreur de fermeture du fichier");
        erreur = 1;
    }
    if (erreur) {
        return EXIT_FAILURE;
    }

    printf("%-8s %12s %10s %10s %15s %15s\n",
           "Annee", "Accidents", "Indemnes", "Morts", "Blesses graves", "Blesses legers");
    for (int i = 0; i < NB_ANNEES; ++i) {
        printf("%-8d %12ld %10ld %10ld %15ld %15ld\n",
               tableau[i].annee,
               tableau[i].accidents,
               tableau[i].indemnes,
               tableau[i].morts,
               tableau[i].blesses_graves,
               tableau[i].blesses_legers);
    }

    return EXIT_SUCCESS;
}