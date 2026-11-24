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
