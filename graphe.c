#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphe.h"

ListeAdj creerListeAdj(int taille) {
    ListeAdj g;
    g.taille = taille;
    g.tableau = (Liste *)malloc(taille * sizeof(Liste));
    if (!g.tableau) {
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < taille; i++)
        g.tableau[i] = creerListe();
    return g;
}

void afficherListeAdj(ListeAdj g) {
    for (int i = 0; i < g.taille; i++) {
        printf("Sommet %d : ", i + 1);
        afficherListe(g.tableau[i]);
    }
}

ListeAdj readGraph(const char *filename) {
    FILE *file = fopen(filename, "rt");
    if (!file) {
        perror("Erreur ouverture fichier");
        exit(EXIT_FAILURE);
    }

    int nbvert, depart, arrivee;
    float proba;
    fscanf(file, "%d", &nbvert);

    ListeAdj g = creerListeAdj(nbvert);

    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3) {
        // Attention : on stocke arrivee telle quelle (ex: 2), mais l'index du tableau est depart-1
        ajouterCellule(&g.tableau[depart - 1], arrivee, proba);
    }

    fclose(file);
    return g;
}

int estGrapheMarkov(ListeAdj *g) {
    int ok = 1;
    for (int i = 0; i < g->taille; i++) {
        float somme = 0;
        for (cell *c = g->tableau[i].head; c != NULL; c = c->suiv)
            somme += c->proba;
        if (somme < 0.99 || somme > 1.01) { // Tolérance float
            printf("Sommet %d : somme = %.2f\n", i + 1, somme);
            ok = 0;
        }
    }
    if (ok)
        printf("Le graphe est un graphe de Markov.\n");
    else
        printf("Le graphe n'est pas un graphe de Markov.\n");
    return ok;
}

char *getId(int num) {
    static char id[10];
    if (num <= 26)
        sprintf(id, "%c", 'A' + num - 1);
    else
        sprintf(id, "S%d", num);
    return id;
}

void exportMermaid(ListeAdj g, const char *filename) {
    FILE *f = fopen(filename, "wt");
    if (!f) {
        exit(EXIT_FAILURE);
    }

    fprintf(f, "---\nconfig:\n   layout: elk\n   theme: neo\n   look: neo\n---\n\n");
    fprintf(f, "flowchart LR\n");

    for (int i = 0; i < g.taille; i++)
        fprintf(f, "%s((%d))\n", getId(i + 1), i + 1);

    fprintf(f, "\n");
    for (int i = 0; i < g.taille; i++) {
        for (cell *c = g.tableau[i].head; c != NULL; c = c->suiv)
            fprintf(f, "%s -->|%.2f|%s\n", getId(i + 1), c->proba, getId(c->sommet_arrivee));
    }

    fclose(f);
    printf("Fichier Mermaid exporté dans %s\n", filename);
}