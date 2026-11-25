#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Faire un fichier .h */
/* Il manque des numéros d'accidents entre 2005 et 2010 */
/* Tableau avec les numéros précédents et si différent alors resultat->accidents++ */
/* Ajouter contrôle sur scanf() et autres */

#define PREMIERE_ANNEE 2005
#define NB_ANNEES 20
#define NB_ACCIDENTS_MAX 1286097
#define NB_ACCIDENTS_ANNEE 100000

typedef struct {
    int annee;
    char tab[NB_ACCIDENTS_ANNEE];
    long accidents;
    long indemnes;
    long morts;
    long blesses_graves;
    long blesses_legers;
} Annee;

int yearData(char* nomFichier, int annee);
void affichage(Annee tableau[], int annee);
int menu(char* nomFichier);

int yearData(char* nomFichier, int annee) {  
    
    FILE* fichier = fopen(nomFichier, "r");

    if (fichier == NULL) {
        printf("Fichier incorrect.\n");
        return 1;
    }

    /* Initialisation du tableau */
    Annee tableau[NB_ANNEES] = {0};
    for (int i = 0; i < NB_ANNEES; ++i) {
        tableau[i].annee = PREMIERE_ANNEE + i;
        tableau[i].accidents = 0;
        for (int j = 0; j < NB_ACCIDENTS_ANNEE; j++) tableau[i].tab[j]='0';
    }
    /* On saute la première ligne */
    char ligne[256]; 
    if (fgets(ligne, 255, fichier) == NULL) {
        printf("Fichier vide.\n");
        fclose(fichier);
        return 1;
    }
   
    int a, b, c, d, e, f;
    long nombre_accidents_max = 0;
    long numero_ligne = 2;
    int annee_lue;
    long numero_accident;
    long nombre_lignes_ko = 0;
    int severite;

    while (fscanf(fichier, "%d;%ld;%d;%d;%d;%d;%d;%d;%d", &annee_lue, &numero_accident,
                  &a, &b, &c, &severite, &d, &e, &f) == 9) {
        
        if (annee_lue < PREMIERE_ANNEE || annee_lue >= PREMIERE_ANNEE + NB_ANNEES) {
            printf("Ligne %ld ignoree : annee incorrecte.\n", numero_ligne);
            nombre_lignes_ko++;
            continue; /* on ne traite pas cette ligne */
        }

        /* On se positionne sur la bonne case du tableau entre 0 et 19 */
        Annee *resultat = &tableau[annee_lue - PREMIERE_ANNEE];

        /* On compte le nombre d'accidents total par année */

        if (resultat->tab[numero_accident] != '1') {
            resultat->tab[numero_accident] = '1';
            resultat->accidents++;
        }

        // if (numero_accident > resultat->accidents) resultat->accidents = numero_accident;

        if (severite < 1 || severite > 4) {
            // printf("Ligne %ld ignoree : %d severite incorrecte.\n", numero_ligne, severite);
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

    if (nombre_lignes_ko != 0) printf("\nNombre de lignes incorrectes = %ld ?\n\n", nombre_lignes_ko);

    for (int i = 0; i < NB_ANNEES; ++i) nombre_accidents_max+=tableau[i].accidents;

    printf("Nombre d'accidents prevu : %ld\n", NB_ACCIDENTS_MAX);
    printf("\nNombre total d'accidents comptes : %ld\n\n", nombre_accidents_max);

    if (fclose(fichier) == -1) {
    printf("Erreur lors de la fermeture du fichier.\n");
    }

    affichage(tableau, annee);
    return 0;
}

void affichage(Annee tableau[], int annee) {
   
    printf("+-----------------------------------------------------------------------------------+\n");
    printf("| %-6s | %-13s | %-10s | %-7s | %-15s | %-15s |\n",
           "Annee", "Nb Accidents", "Indemnes", "Morts", "Blesses graves", "Blesses legers");
    printf("+-----------------------------------------------------------------------------------+\n");

    for (int i = 0; i < NB_ANNEES; ++i) {
        if (tableau[i].annee == annee) {
        printf("| %-6d | %13ld | %10ld | %7ld | %15ld | %15ld |\n",
               tableau[i].annee,
               tableau[i].accidents, 
               tableau[i].indemnes,
               tableau[i].morts,
               tableau[i].blesses_graves,
               tableau[i].blesses_legers);
        }
    }
    printf("+-----------------------------------------------------------------------------------+\n\n");

    printf("+-----------------------------------------------------------------------------------+\n");
    printf("| %-6s | %-13s | %-10s | %-7s | %-15s | %-15s |\n",
           "Annee", "Nb Accidents", "Indemnes", "Morts", "Blesses graves", "Blesses legers");
    printf("+-----------------------------------------------------------------------------------+\n");

    for (int i = 0; i < NB_ANNEES; ++i) {
        printf("| %-6d | %13ld | %10ld | %7ld | %15ld | %15ld |\n",
               tableau[i].annee,
               tableau[i].accidents, 
               tableau[i].indemnes,
               tableau[i].morts,
               tableau[i].blesses_graves,
               tableau[i].blesses_legers);
        }
    printf("+-----------------------------------------------------------------------------------+\n\n");
}

int menu(char* nomFichier) {

    char reponse[64];
    int annee;
    char* fin;
    int retour = 0;

    do {
        printf("Les statistiques de quelle annee souhaitez vous voir ?\n");
        printf("Pour terminer tapez Exit.\n");
        printf("A vous : ");
        scanf("%s",reponse); 
        if (strcmp(reponse,"Exit") == 0) break; 
        annee = (int) strtol(reponse, &fin, 10);
        if (*fin != '\0') {
            printf("Saisie invalide.\n");
            retour = 1;
            break;
        }
        if (annee < 2005 || annee > 2024) {
            printf("L'annee doit etre comprise entre 2005 et 2024.\n");
            retour = 1;
            break;
        }
        if (yearData(nomFichier,annee) == 1) retour = 1;
    } while (strcmp(reponse,"Exit") != 0); 

    return retour;
}

int main(int argc, char** argv) {
  
    /* if (argc != 3) {
        printf("Ligne de commande incorrecte.\n");
        return 1;
    }*/

    // char* nomFichier = "cy_crash_tech_big_sorted.csv";
    char* nomFichier = "cy_crash_tech_big_shuffled.csv";
    // char* nomFichier = "cy_crash_tech_small_shuffled.csv";
    // char* nomFichier = "cy_crash_tech_small_sorted.csv";
    // char* nomFichier = "cy_crash_tech_small2_shuffled.csv";
    // char* nomFichier = "cy_crash_tech_small2_sorted.csv";

    /* char* nomFichier = argv[2]; */

    /*
    if (strcmp(argv[1],"YEAR_DATA") == 0) return(yearData(nomFichier));
    if (strcmp(argv[1],"AGE_DATA") == 0) return(ageData(nomFichier));
    if (strcmp(argv[1],"CRASH_DATA") == 0) return(crashData(nomFichier));
    */
      
    return(menu(nomFichier));

    /* printf("Traitement incorrect.\n");
    return 1;*/
} 