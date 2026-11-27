#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct Victime{
    int age;
    int genre;
    int gravite;
    struct Victime *suivant;
}Victime;


typedef struct Vehicule{
    int vehicule_id;
    int type_trajet;
    struct Victime *victimes;
    struct Vehicule *suivant;
}Vehicule;


typedef struct Accident{
    long long cle;
    int annee;
    int collision_id;
    int hauteur;
    struct Vehicule *vehicules;
    struct Accident *gauche;
    struct Accident *droite;
}Accident;


Victime *ajouterVictime(Victime *liste, int age, int genre, int gravite){
    Victime *n = malloc(sizeof(Victime));
    if (n == NULL)
        return liste;
    n->age = age;
    n->genre = genre;
    n->gravite = gravite;
    n->suivant = liste;
    return n;
}


Vehicule *ajouterVehicule(Vehicule *liste, int id, int type_trajet){
    Vehicule *n = malloc(sizeof(Vehicule));
    if (n == NULL)
        return liste;
    n->vehicule_id = id;
    n->type_trajet = type_trajet;
    n->victimes = NULL;
    n->suivant = liste;
    return n;
}


Accident *creerAccident(int annee, int collision_id){
    Accident *n = malloc(sizeof(Accident));
    if (n == NULL)
        return NULL;
    n->annee = annee;
    n->collision_id = collision_id;
    n->cle = annee * 1000000LL + collision_id;
    n->hauteur = 1;
    n->vehicules = NULL;
    n->gauche = NULL;
    n->droite = NULL;
    return n;
}


Accident *insererAccident(Accident *a, Accident *n){
    if (a == NULL)
        return n;
    if (n->cle < a->cle)
        a->gauche = insererAccident(a->gauche, n);
    else if (n->cle > a->cle)
        a->droite = insererAccident(a->droite, n);
    return a;
}


Accident *obtenirAccident(Accident *a, long long cle) {
    if (a == NULL)
        return NULL;
    if (cle == a->cle)
        return a;
    if (cle < a->cle)
        return obtenirAccident(a->gauche, cle);
    return obtenirAccident(a->droite, cle);
}

Vehicule *obtenirVehicule(Accident *a, int id){
    if (a == NULL)
        return NULL;
    Vehicule *v = a->vehicules;
    while (v != NULL) {
        if (v->vehicule_id == id)
            return v;
        v = v->suivant;
    }
    return NULL;
}

void ajouterLigne(Accident **arbre, int annee, int collision_id, int vehicule_id, int type_trajet, int age, int genre, int gravite){
    long long cle = annee * 1000000LL + collision_id;
    Accident *a = obtenirAccident(*arbre, cle);
    if (a == NULL) {
        a = creerAccident(annee, collision_id);
        if (a == NULL)
            return;
        *arbre = insererAccident(*arbre, a);
    }
    Vehicule *v = obtenirVehicule(a, vehicule_id);
    if (v == NULL) {
        Vehicule *ancienneListe = a->vehicules;
        a->vehicules = ajouterVehicule(a->vehicules, vehicule_id, type_trajet);
        if (a->vehicules == ancienneListe)
            return;
        v = a->vehicules;
    }
    Victime *ancienneListe = v->victimes;
    v->victimes = ajouterVictime(v->victimes, age, genre, gravite);
    if (v->victimes == ancienneListe)
        return;
}

void lireCSV(Accident **arbre, char *nomFichier){
    FILE *fichier = fopen("Projet_Crash_Tech/Crashtest_small.csv", "r");
    if (fichier == NULL) {
        printf("Erreur ouverture fichier\n");
        return;
    }
    char ligne[1024];
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        printf("%s", ligne);
    }
    fclose(fichier);
}


int main() {
    Accident *arbre = NULL;
    lireCSV(&arbre, "Projet_Crash_Tech/Crashtest_small.csv");
    return 0;
}
