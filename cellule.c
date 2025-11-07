#include <stdio.h>
#include <stdlib.h>
#include "cellule.h"

cell *creerCellule(int arrivee, float proba) {
    cell *nouv = (cell *)malloc(sizeof(cell));
    if (!nouv) {
        exit(EXIT_FAILURE);
    }
    nouv->sommet_arrivee = arrivee;
    nouv->proba = proba;
    nouv->suiv = NULL;
    return nouv;
}
