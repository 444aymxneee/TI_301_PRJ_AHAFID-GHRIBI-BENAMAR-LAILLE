#ifndef TARJAN_H
#define TARJAN_H

#include <stdbool.h>
#include "graphe.h"

// Structures Etape 1.1 & 1.2 [cite: 76-82]
typedef struct {
    int id;             // Identifiant du sommet (0 à N-1)
    int num;            // Numéro ordre de visite
    int num_accessible; // Low-link
    bool dans_pile;     // Indicateur booléen
} t_tarjan_vertex;

// Structure pour liste chainée simple d'entiers (pour stocker les sommets dans une classe)
typedef struct NoeudSommet {
    int val; // Numéro du sommet (format 1..N pour l'affichage)
    struct NoeudSommet* suiv;
} t_noeud_sommet;

// Structure Classe (Etape 1.3) [cite: 89]
typedef struct {
    char nom[10];             // Ex: "C1"
    t_noeud_sommet* sommets;  // Liste des sommets
    bool est_puits;           // Pour Etape 3 (Persistante)
} t_classe;

// Structure Partition (Etape 1.4) [cite: 94]
typedef struct {
    int nb_classes;
    t_classe* tab_classes;    // Tableau dynamique
} t_partition;

// Structure Pile (Etape 2.2) [cite: 104]
typedef struct {
    int *data;
    int top;
    int capacite;
} t_pile;

// Prototypes
t_partition tarjan(ListeAdj *g);
void afficherPartition(t_partition p);
void analyseHasse(ListeAdj *g, t_partition *p);

#endif