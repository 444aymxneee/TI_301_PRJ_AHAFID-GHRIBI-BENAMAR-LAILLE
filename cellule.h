#ifndef CELLULE_H
#define CELLULE_H

typedef struct cellule {
    int sommet_arrivee;
    float proba;
    struct cellule *suiv;
} cell;

cell *creerCellule(int arrivee, float proba);

#endif
