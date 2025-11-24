#ifndef TARJAN_H
#define TARJAN_H

#include <stdbool.h>
#include "graphe.h"

typedef struct {
    int id;
    int num;
    int num_accessible;
    bool dans_pile;
} t_tarjan_vertex;

typedef struct NoeudSommet {
    int val;
    struct NoeudSommet* suiv;
} t_noeud_sommet;

typedef struct {
    char nom[10];
    t_noeud_sommet* sommets;
    bool est_puits;
} t_classe;

typedef struct {
    int nb_classes;
    t_classe* tab_classes;
} t_partition;

typedef struct {
    int *data;
    int top;
    int capacite;
} t_pile;

t_partition tarjan(ListeAdj *g);
void afficherPartition(t_partition p);
void analyseHasse(ListeAdj *g, t_partition *p);

#endif