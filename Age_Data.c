#include <stdio.h>
#include <stdlib.h>

//Faire fonction filtre
//Faire fonction repartition age
//FONCTIONNE UNIQUEMENT AVEC UN TABLEAU .CSV TRIEE donc faire fonction insert TRI 
//Faire un .h
//Faire une fonction lecture des données du fichier au lieu de le faire dans le main?
//Manque des cas particulier dans certaines fonctions?
//Ameliorer l'affichage de la pyramide des ages?

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

void ajoutStat(Chainon *p, int type, int genre){//Sert à ajouter les stats plutot que de mettre dans insertion
    if ( type == 1 ){
        p->e.nbr_type1++;
    } else if ( type == 2 ){
        p->e.nbr_type2++;
    } else {
        p->e.nbr_type3++;
    }

    if ( genre == 1 ){
        p->e.nbr_gender1++;
    } else {
        p->e.nbr_gender1++;
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
            ajoutStat(n, type, genre);
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
        return ((p->e.nbr_type1) / total ) * 100;
    } else if ( type == 2 ){
        return ((p->e.nbr_type2) / total ) * 100;
    } else {
        return ((p->e.nbr_type3) / total ) * 100;
    }
    //message d'erreur si le type est au dessus de 4?      
}

float statGender(Chainon *p, int genre){//Permet de calculer les pourcentages des genres
    int total = p->e.nbr_gender1 + p->e.nbr_gender2; 
    
    if( total <= 0 ){
        printf("ERREUR, le total est inférieur ou égale à 0");
    }
    if ( genre == 1 ){
        return ((p->e.nbr_gender1) / total ) * 100;
    } else {
        return ((p->e.nbr_gender2) / total ) * 100;
    }
}

void affichage(Chainon *p){ //affiche liste chainée
    Chainon *actuel = p;
    while( actuel != NULL ){
        printf("| %d |" " %.2f | %.2f | %.2f | %.2f | %.2f |\n",
            actuel->e.age,
            statType(actuel, 1),
            statType(actuel, 2),
            statType(actuel, 3),
            statGender(actuel, 1),
            statGender(actuel, 2));

        actuel = actuel->suivant;
    }
}

int main(){ //int argc, char ** argv 
    FILE *fichier = fopen("cy_crash_tech_small_sorted.csv", "r");//lecture du fichier pour récuperer les infos

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
        p = insertion(p, calculAge(year, birth_year), type, gender);
        //printf("\n%d", calculAge(birth_year));
    }

    printf("___AFFICHAGE PYRAMIDE DES AGES___\n\n");
    printf("| AGE | CONDUCTEUR | PASSAGER | PIETON | HOMME | FEMME | \n");

    affichage(p);

    fclose(fichier);
    
    return 0;
}
