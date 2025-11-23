#include <stdio.h>
#include <stdlib.h>
#include "list.h"

Liste creerListe() {
    Liste l;
    l.head = NULL;
    return l;
}

void ajouterCellule(Liste *liste, int arrivee, float proba) {
    cell *nouv = creerCellule(arrivee, proba);
    nouv->suiv = liste->head;
    liste->head = nouv;
}

void afficherListe(Liste liste) {
    cell *courant = liste.head;
    while (courant != NULL) {
        printf("(%d, %.2f) -> ", courant->sommet_arrivee, courant->proba);
        courant = courant->suiv;
    }
    printf("NULL\n");
}