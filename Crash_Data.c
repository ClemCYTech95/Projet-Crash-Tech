#include <stdio.h>
#include <stdlib.h>

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
    long long id;
    struct Vehicule *vehicules;
    struct Accident *fg;
    struct Accident *fd;
    int equilibre;
}Accident;


Victime *ajouterVictime(Victime *liste,int age,char *gender,char *severity){
    Victime *n=malloc(sizeof(Victime));
        if (n==NULL)
            return liste;
        n->age=age;
        n->gender=strdup(gender);
        n->severity=strdup(severity);
        n->suivant=liste;
        return n;
}

Vehicule *ajouterVehicule(Vehicule *liste, int id, char *purpose) {
    Vehicule *n = malloc(sizeof(Vehicule));
    if (n == NULL)
        return liste;
    n->vehicule_id = id;
    n->purpose = strdup(purpose);
    n->victimes = NULL;
    n->suivant = liste;
    return n;
}

Accident *creerAccident(int year, int collision_id) {
    Accident *n = malloc(sizeof(Accident));
    if (n == NULL)
        return NULL;
    n->year = year;
    n->collision_id = collision_id;
    n->hauteur = 1;
    n->vehicules = NULL;
    n->gauche = NULL;
    n->droite = NULL;
    return n;
}
