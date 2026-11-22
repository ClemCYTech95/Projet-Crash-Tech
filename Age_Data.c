#include <stdio.h>
#include <stdlib.h>

//Faire fonction filtre
//Faire fonction repartition age
//Faire un .h
//Faire une fonction lecture des données du fichier au lieu de le faire dans le main?
//Manque des cas particulier dans certaines fonctions?

typedef struct Age{
    int age;
    int nbr_type1;// 1 = conducteur
    int nbr_type2;// 2 = passager
    int nbr_type3;// 3 = pieton
    int nbr_gender1;// 1 = homme
    int nbr_gender2;// 2 = femme
}Age;

typedef struct Chainon{
    Age e;
    struct Chainon *suivant;
}Chainon;

typedef struct {
    int annee;
    int type;
    int gender;
    int collision;
} Tri;

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

    while (fscanf(fichier, "%d;%d;%d;%d;%d;%d;%d;%d;%d", &tmp.annee, &a,
                  &b, &c, &tmp.type, &d, &tmp.gender, &e, &f) == 9)
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

    FILE *sortie = fopen("cy_crash_tech_small_shuffled.csv", "w");

    if (sortie == NULL) {
        free(tab);
        return 1;
    }


    if (fclose(fichier) == -1) {
        printf("Erreur lors de la fermeture du fichier.\n");
    }

    free(tab);

    printf("%ld lignes triees.\n", nb);

    return 0;
}

void ajoutStat(Chainon *p, int type, int genre){//Sert à ajouter les stats plutot que de mettre dans insertion
    if ( type == 1 ){
        p->e.nbr_type1++;
    } else if ( type == 2 ){
        p->e.nbr_type2++;
    } else if ( type == 3 ){
        p->e.nbr_type3++;
    } else {
        printf("ERREUR - Le type est non renseigné");
    }

    if ( genre == 1 ){
        p->e.nbr_gender1++;
    } else if ( genre == 2){
        p->e.nbr_gender2++;
    } else {
        printf("ERREUR - Le genre est non renseigné");
    }
}

Chainon *creationChainon(int age, int type, int genre){
    Chainon *n = malloc(sizeof(Chainon));

    if (n == NULL){
        exit(1);
    }

    //initialisation 
    n->e.nbr_gender1 = 0;
    n->e.nbr_gender2 = 0;
    n->e.nbr_type1 = 0;
    n->e.nbr_type2 = 0;
    n->e.nbr_type3 = 0;
    ajoutStat(n, type, genre);
    n->e.age = age;
    n->suivant = NULL;    
    
    return n;
}

int calculAge(int annee, int naissance){
    return annee - naissance;//calcul l'âge de chaque personne
}

Chainon *insertion(Chainon *p, int age,int type, int genre){
    Chainon *n = creationChainon(age, type, genre); 

    if ( p == NULL ){
        return n;
    }

    //au début
    if ( n->e.age <= p->e.age ){
        if ( n->e.age == p->e.age ){
            ajoutStat(p, type, genre);
            free(n);
            return p;
        }
        n->suivant = p;
        return n;
    }

    //au milieu et à la fin
    Chainon *actuel = p;
    while ( actuel->suivant != NULL && n->e.age > actuel->suivant->e.age ){
        actuel = actuel->suivant;
    }

    if ( actuel->suivant != NULL && n->e.age == actuel->suivant->e.age ){ //si l'age existe déjà
        ajoutStat(actuel->suivant, type, genre);
        free(n);
        return p;
    }

    n->suivant = actuel->suivant;
    actuel->suivant = n;
    return p;
}

float statType(Chainon *p, int type){//Permet de calculer les pourcentages des types

    int total = p->e.nbr_type1 + p->e.nbr_type2 + p->e.nbr_type3; 
    
    if( total <= 0 ){
        printf("ERREUR, le total est inférieur ou égale à 0");
    }
    if ( type == 1 ){
        return (((float)p->e.nbr_type1) / total ) * 100; //rajoute des floats, sinon c'est une division entière
    } else if ( type == 2 ){
        return (((float)p->e.nbr_type2) / total ) * 100;
    } else {
        return (((float)p->e.nbr_type3) / total ) * 100;
    }
    //message d'erreur si le type est au dessus de 4?      
}


float statGender(Chainon *p, int genre){//Permet de calculer les pourcentages des genres
    int total = p->e.nbr_gender1 + p->e.nbr_gender2; 
    
    if( total <= 0 ){
        printf("ERREUR, le total est inférieur ou égale à 0");
    }
    if ( genre == 1 ){
        return (((float)p->e.nbr_gender1) / total ) * 100; //rajoute des floats, sinon c'est une division entière
    } else {
        return (((float)p->e.nbr_gender2) / total ) * 100;
    }
    //message d'erreur pour les cas particuliers?
}

void affichage(Chainon *p) {
    Chainon *actuel = p;

    printf("_______________AFFICHAGE PYRAMIDE DES AGES_______________\n\n");
    printf("|-----|------------|----------|--------|--------|--------|\n");
    printf("| AGE | CONDUCTEUR | PASSAGER | PIETON | HOMME  | FEMME  |\n");
    printf("|-----|------------|----------|--------|--------|--------|\n");

    while (actuel != NULL) {
        printf("| %3d |   %6.2f%%  | %6.2f%%  |%6.2f%% |%6.2f%% |%6.2f%% |\n", //%3d pour 3 caractères et %6.2f pour 6 caractères 2 décimaux
            actuel->e.age,
            statType(actuel, 1),
            statType(actuel, 2),
            statType(actuel, 3),
            statGender(actuel, 1),
            statGender(actuel, 2));
        
        actuel = actuel->suivant;
    }

    printf("|-----|------------|----------|--------|--------|--------|\n");
}

int main(){ //int argc, char ** argv 
    FILE *fichier = fopen("cy_crash_tech_big_sorted.csv", "r");//lecture du fichier pour récuperer les infos

    if (fichier == NULL) {
        printf("Erreur d'ouverture");
        return 1;
    }
    
    int year;
    int gender;
    int type;
    int birth_year;
    int a,b,c,d,e;

    Chainon *p = NULL;//initialise le pointeur de la liste chainée
    char tmp[1000];// char pour sauter première ligne
    fgets(tmp, 999, fichier);//saute la première ligne

    while(fscanf(fichier,"%d;%d;%d;%d;%d;%d;%d;%d;%d",&year, &a, &b, &c, &type, &d, &gender, &birth_year, &e) == 9){       
        //printf("\n%d", calculAge(birth_year));
        int age = calculAge(year, birth_year);

        if ( birth_year > 1900 && age >= 0 && age <= 150
            && type >= 1 && type <= 3 && (gender == 1 || gender == 2)) {
            //pour ignorer les valeurs incohérentes.
            p = insertion(p, age, type, gender);
        }
    }

    affichage(p);

    fclose(fichier);
    
    return 0;
}
