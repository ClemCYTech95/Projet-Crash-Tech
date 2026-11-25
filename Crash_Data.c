#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Victime{
    int age;
    int gender;
    int severity;
    struct Victime *suivant;
}Victime;


typedef struct Vehicule{
    int vehicule_id;
    int purpose;
    struct Victime *victimes;
    struct Vehicule *suivant;
}Vehicule;


typedef struct Accident{
    long long cle;
    int year;
    int collision_id;
    int hauteur;
    struct Vehicule *vehicules;
    struct Accident *gauche;
    struct Accident *droite;
}Accident;


Victime *ajouterVictime(Victime *liste, int age, int gender, int severity){
    Victime *n = malloc(sizeof(Victime));
    if (n == NULL)
        return liste;
    n->age = age;
    n->gender = gender;
    n->severity = severity;
    n->suivant = liste;
    return n;
}


Vehicule *ajouterVehicule(Vehicule *liste, int id, int purpose){
    Vehicule *n = malloc(sizeof(Vehicule));
    if (n == NULL)
        return liste;
    n->vehicule_id = id;
    n->purpose = purpose;
    n->victimes = NULL;
    n->suivant = liste;
    return n;
}


Accident *creerAccident(int year, int collision_id){
    Accident *n = malloc(sizeof(Accident));
    if (n == NULL)
        return NULL;
    n->year = year;
    n->collision_id = collision_id;
    n->cle = year * 1000000LL + collision_id;
    n->hauteur = 1;
    n->vehicules = NULL;
    n->gauche = NULL;
    n->droite = NULL;
    return n;
}


int comparerAccidents(Accident *a, Accident *b){
    if (a->cle < b->cle)
        return -1;
    if (a->cle > b->cle)
        return 1;
    return 0;
}


Accident *insererAccident(Accident *a, Accident *n){
    if (a == NULL)
        return n;
    int comparaison = comparerAccidents(n, a);
    if (comparaison < 0)
        a->gauche = insererAccident(a->gauche, n);
    else if (comparaison > 0)
        a->droite = insererAccident(a->droite, n);
    else{
        while (n->vehicules != NULL){
            Vehicule *v = n->vehicules;
            n->vehicules = v->suivant;
            v->suivant = a->vehicules;
            a->vehicules = v;
        }
        free(n);
    }
    return a;
}


Accident *rechercherAccident(Accident *a, long long cle) {
    if (a == NULL)
        return NULL;
    if (cle == a->cle)
        return a;
    if (cle < a->cle)
        return rechercherAccident(a->gauche, cle);
    return rechercherAccident(a->droite, cle);
}
