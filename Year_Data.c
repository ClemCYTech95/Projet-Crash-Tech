#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Faire un fichier .h */

#define PREMIERE_ANNEE 2005
#define NB_ANNEES 20
#define NB_ACCIDENTS_MAX 1286097

typedef struct {
    int annee;
    long collision;
    int severite;
} Tri;

typedef struct {
    int annee;
    long accidents;
    long indemnes;
    long morts;
    long blesses_graves;
    long blesses_legers;
} Annee;

int yearData(char* nomFichier, int annee);
void affichage(Annee tableau[], int annee);

/* Utilisation de la fonction qsort() pour le tri */
int comparaison(const void* pa, const void* pb)
{
    const Tri* p1 = pa;
    const Tri* p2 = pb;

    /* On compare d'abord par année */
    if (p1->annee < p2->annee)
        return -1;
    if (p1->annee > p2->annee)
        return 1;

    /* puis par numéro d'accident lorsque les années sont identiques */
    if (p1->collision < p2->collision)
        return -1;
    if (p1->collision > p2->collision)
        return 1;

    return 0;
}

int tricsv(char* nomFichier) {

    FILE* fichier = fopen(nomFichier, "r");

    if (fichier == NULL) {
        printf("Fichier incorrect.\n");
        return 1;
    }
    
    char ligne[256];

    /* On saute la première ligne */
    if (fgets(ligne, 255, fichier) == NULL) {
        printf("Fichier vide.\n");
        fclose(fichier);
        return 1;
    }

    Tri *tab = NULL;
    long nb = 0; /* Nombre de cases = Nombre de lignes du fichiers */

    Tri tmp;
    int a, b, c, d, e, f;

    /* Récupération des 3 colonnes utiles dans un tableau */

    while (fscanf(fichier, "%d;%ld;%d;%d;%d;%d;%d;%d;%d", &tmp.annee, &tmp.collision,
                  &a, &b, &c, &tmp.severite, &d, &e, &f) == 9)
    {
        Tri *nouveau = realloc(tab, (nb + 1) * sizeof(Tri));

        if (nouveau == NULL)
        {
            free(tab);
            fclose(fichier);
            return 1;
        }

        tab = nouveau;
        tab[nb] = tmp;
        nb++;
    }

    fclose(fichier);

    /* On tri le tableau obtenu */

    qsort(tab, nb, sizeof(Tri), comparaison);

    /* Nouveau fichier trié */

    FILE *sortie = fopen("cy_crash_tech_yearData.csv", "w");

    if (sortie == NULL)
    {
        free(tab);
        return 1;
    }

    for (int i = 0; i < nb; i++)
    {
        fprintf(sortie,
                "%d;%ld;%d\n",
                tab[i].annee,
                tab[i].collision,
                tab[i].severite);
    }

    if (fclose(fichier) == -1) {
        printf("Erreur lors de la fermeture du fichier.\n");
    }

    free(tab);

    printf("%ld lignes triees.\n", nb);

    return 0;
}

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
    }

    char ligne[256];

    int retour;
    long nombre_accidents_max=0;
    long numero_ligne = 2;
    int annee_lue;
    long numero_accident;
    long numero_accident_precedent = 0;
    long nombre_lignes_ko=0;
    int annee_precedente = 0;
    int severite;

    while ((retour=fscanf(fichier, "%d;%ld;%d", &annee_lue, &numero_accident, &severite)) == 3) {
        

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

        if ((annee_lue != annee_precedente) || (numero_accident != numero_accident_precedent)) {
            resultat->accidents++;
            nombre_accidents_max++;
        }
        numero_accident_precedent = numero_accident;
        annee_precedente = annee_lue;
       
        if (severite < 1 || severite > 4) {
            printf("Ligne %ld ignoree : %d severite incorrecte.\n", numero_ligne, severite);
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

    if (nombre_lignes_ko != 0) printf("\nNombre de lignes incorrectes = %d ?\n", nombre_lignes_ko);

    if (nombre_accidents_max > NB_ACCIDENTS_MAX) {
        printf("Maximum d'accidents depasse ?.\n");
    }
    else printf("\nNombre total d'accidents : %ld\n\n", nombre_accidents_max);

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

int menu() {

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
        if (yearData("cy_crash_tech_yearData.csv",annee) == 1) retour = 1;
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
    
    if (tricsv(nomFichier) == 1) return 1;
    
    return(menu());

    /* printf("Traitement incorrect.\n");
    return 1;*/
} 