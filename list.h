#ifndef LISTE_H
#define LISTE_H
#include "cellule.h"

typedef struct {
    cell *head;
} Liste;

Liste creerListe();
void ajouterCellule(Liste *liste, int arrivee, float proba);
void afficherListe(Liste liste);

#endif
