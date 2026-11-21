#include <stdio.h>

/* Faire fonction Tri du fichier csv */
/* Faire un test avec colonne 2 non triée */
/* Faire un fichier .h */
/* Faire fonction Menu */

#define PREMIERE_ANNEE 2005
#define NB_ANNEES 20
#define NB_ACCIDENTS_MAX 1286097

typedef struct {
    int annee;
    long accidents;
    long indemnes;
    long morts;
    long blesses_graves;
    long blesses_legers;
} Annee;

int yearData(char*);
void affichage(Annee[]);

int yearData(char* nomFichier) {  
    
    FILE* fichier = fopen(nomFichier, "r");

    if (fichier == NULL) {
        printf("Fichier incorrect.\n");
        return 1;
    }

    /* Initialisation du tableau */
    Annee tableau[NB_ANNEES] = {0};
    for (int i = 0; i < NB_ANNEES; ++i) {
        tableau[i].annee = PREMIERE_ANNEE + i;
    }

    char ligne[256];

    /* On saute la première ligne */
    if (fgets(ligne, sizeof(ligne), fichier) == NULL) {
        printf("Fichier vide.\n");
        fclose(fichier);
        return 1;
    }

    int retour;
    int a, b, c, d, e, f;
    long nombre_accidents_max=0;
    long numero_ligne = 2;
    int annee_lue;
    long numero_accident;
    long numero_accident_precedent = 0;
    long nombre_lignes_ko=0;
    int severite;

    while ((retour=fscanf(fichier, "%d;%ld;%d;%d;%d;%d;%d;%d;%d", &annee_lue, &numero_accident, &a, &b, &c,
                               &severite, &d, &e, &f)) == 9) {
        

        if (retour == -1) {
        printf("Erreur lecture fichier.\n");
        return 1;    
        }

        if (annee_lue < PREMIERE_ANNEE || annee_lue >= PREMIERE_ANNEE + NB_ANNEES) {
            printf("Ligne %ld ignoree : annee incorrecte.\n", numero_ligne);
            nombre_lignes_ko++;
            continue; /* on ne traite pas cette ligne */
        }

        /* On se positionne sur la bonne case du tableau entre 0 et 19 */
        Annee *resultat = &tableau[annee_lue - PREMIERE_ANNEE];

        /* On compte le nombre d'accidents total par année */

        if (numero_accident != numero_accident_precedent) {
            resultat->accidents++;
            nombre_accidents_max++;
        }
        numero_accident_precedent = numero_accident;
       
        if (severite < 1 || severite > 4) {
            printf("Ligne %ld ignoree : severite incorrecte.\n", numero_ligne);
            nombre_lignes_ko++;
            continue; /* on ne traite pas cette ligne */
        }

        /* On compte les nombres des différents types de sévérités par année */
        switch (severite) {
        case 1:
            resultat->indemnes++;
            break;
        case 2:
            resultat->morts++;
            break;
        case 3:
            resultat->blesses_graves++;
            break;
        case 4:
            resultat->blesses_legers++;
            break;
        }

        numero_ligne++;
    }

    printf("\nNombre de lignes incorrectes ! = %d.\n", nombre_lignes_ko);

    if (nombre_accidents_max > NB_ACCIDENTS_MAX) {
        printf("Maximum d'accidents depassé ?.\n");
    }
    else printf("\nNombre total d'accidents : %ld.\n", nombre_accidents_max);

    if (fclose(fichier) == -1) {
    printf("Erreur lors de la fermeture du fichier.\n");
    }

    affichage(tableau);
    return 0;
}

void affichage(Annee tableau[]) {
   
    printf(" ________________________________________________________________________________\n");
    printf("%s %s %s %s %s %s\n",
           "| Annee | ", "Nb Accidents | ", "Indemnes | ", "Morts | ", "Blesses graves | ", "Blesses legers |");
    printf("|--------------------------------------------------------------------------------|\n");

    for (int i = 0; i < NB_ANNEES; ++i) {
        printf("| %d  |  %ld        |  %ld    |  %ld  |  %ld          |  %ld          |\n",
               tableau[i].annee,
               tableau[i].accidents, 
               tableau[i].indemnes,
               tableau[i].morts,
               tableau[i].blesses_graves,
               tableau[i].blesses_legers);
    }
    printf(" ________________________________________________________________________________\n\n");
}

int main(int argc, char** argv) {
  
    /* if (argc != 3) {
        printf("Ligne de commande incorrecte.\n");
        return 1;
    }*/

    char* nomFichier = "C:\\Users\\CLEMENT\\OneDrive\\Desktop\\test2.csv";
    /* char* nomFichier = argv[2]; */

    /*
    if (strcmp(argv[1],"YEAR_DATA") == 0) return(yearData(nomFichier));
    if (strcmp(argv[1],"AGE_DATA") == 0) return(ageData(nomFichier));
    if (strcmp(argv[1],"CRASH_DATA") == 0) return(crashData(nomFichier));
    */

    return(yearData(nomFichier));

    /* printf("Traitement incorrect.\n");
    return 1;*/
} 