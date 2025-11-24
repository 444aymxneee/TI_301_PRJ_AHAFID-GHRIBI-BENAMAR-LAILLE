#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tarjan.h"

void init_pile(t_pile *p, int taille_max) {
    p->data = (int*)malloc(taille_max * sizeof(int));
    p->top = -1;
    p->capacite = taille_max;
}

void empiler(t_pile *p, int v) {
    if (p->top < p->capacite - 1) {
        p->data[++(p->top)] = v;
    }
}

int depiler(t_pile *p) {
    if (p->top >= 0) {
        return p->data[(p->top)--];
    }
    return -1;
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

void ajouter_sommet_classe(t_classe *c, int val_sommet) {
    t_noeud_sommet* nouveau = (t_noeud_sommet*)malloc(sizeof(t_noeud_sommet));
    nouveau->val = val_sommet;
    nouveau->suiv = c->sommets;
    c->sommets = nouveau;
}

void parcours(int u, ListeAdj *g, t_tarjan_vertex *tab_t, t_pile *p, int *indice, t_partition *part) {
    // Initialisation
    tab_t[u].num = *indice;
    tab_t[u].num_accessible = *indice;
    (*indice)++;
    
    empiler(p, u);
    tab_t[u].dans_pile = true;

    cell *curr = g->tableau[u].head;
    while (curr != NULL) {
        int v_idx = curr->sommet_arrivee - 1; // Conversion en index 0..N-1

        if (tab_t[v_idx].num == -1) {
            parcours(v_idx, g, tab_t, p, indice, part);
            tab_t[u].num_accessible = min(tab_t[u].num_accessible, tab_t[v_idx].num_accessible);
        } else if (tab_t[v_idx].dans_pile) {
            tab_t[u].num_accessible = min(tab_t[u].num_accessible, tab_t[v_idx].num);
        }
        curr = curr->suiv;
    }

    if (tab_t[u].num_accessible == tab_t[u].num) {

        int idx_cls = part->nb_classes;
        
        sprintf(part->tab_classes[idx_cls].nom, "C%d", idx_cls + 1);
        part->tab_classes[idx_cls].sommets = NULL;
        part->tab_classes[idx_cls].est_puits = true;

        int w;
        do {
            w = depiler(p);
            tab_t[w].dans_pile = false;
            ajouter_sommet_classe(&part->tab_classes[idx_cls], w + 1);
        } while (w != u);
        
        part->nb_classes++;
    }
}

t_partition tarjan(ListeAdj *g) {
    t_partition part;
    part.nb_classes = 0;
    part.tab_classes = (t_classe*)malloc(g->taille * sizeof(t_classe));

    t_tarjan_vertex *tab_t = (t_tarjan_vertex*)malloc(g->taille * sizeof(t_tarjan_vertex));
    for(int i=0; i<g->taille; i++) {
        tab_t[i].id = i;
        tab_t[i].num = -1;
        tab_t[i].num_accessible = -1;
        tab_t[i].dans_pile = false;
    }

    t_pile pile;
    init_pile(&pile, g->taille);
    int indice = 0;

    for (int i = 0; i < g->taille; i++) {
        if (tab_t[i].num == -1) {
            parcours(i, g, tab_t, &pile, &indice, &part);
        }
    }

    free(tab_t);
    free(pile.data);
    return part;
}

void afficherPartition(t_partition p) {
    printf("\n--- Partition en Classes ---\n");
    for(int i=0; i<p.nb_classes; i++) {
        printf("Composante %s: {", p.tab_classes[i].nom);
        t_noeud_sommet *cur = p.tab_classes[i].sommets;
        while(cur) {
            printf("%d", cur->val);
            if(cur->suiv) printf(", ");
            cur = cur->suiv;
        }
        printf("}\n");
    }
}

void analyseHasse(ListeAdj *g, t_partition *p) {
    int *map_sommet_classe = (int*)malloc(g->taille * sizeof(int));
    
    for(int c=0; c < p->nb_classes; c++) {
        t_noeud_sommet *cur = p->tab_classes[c].sommets;
        while(cur) {
            map_sommet_classe[cur->val - 1] = c;
            cur = cur->suiv;
        }
    }

    printf("\n--- Liens entre Classes (Diagramme de Hasse) ---\n");
    printf("flowchart TD\n");

    int **mat_classe = (int**)malloc(p->nb_classes * sizeof(int*));
    for(int i=0; i<p->nb_classes; i++) 
        mat_classe[i] = (int*)calloc(p->nb_classes, sizeof(int));

    for(int u=0; u < g->taille; u++) {
        int c_u = map_sommet_classe[u];
        
        cell *curr = g->tableau[u].head;
        while(curr) {
            int v = curr->sommet_arrivee - 1;
            int c_v = map_sommet_classe[v];

            if(c_u != c_v) {
                if(mat_classe[c_u][c_v] == 0) {
                    printf("%s --> %s\n", p->tab_classes[c_u].nom, p->tab_classes[c_v].nom);
                    mat_classe[c_u][c_v] = 1;

                    p->tab_classes[c_u].est_puits = false;
                }
            }
            curr = curr->suiv;
        }
    }

    printf("\n--- Caracteristiques du Graphe ---\n");

    if (p->nb_classes == 1) {
        printf("-> Le graphe est IRREDUCTIBLE (une seule classe).\n");
    } else {
        printf("-> Le graphe n'est PAS irréductible (%d classes).\n", p->nb_classes);
    }

    for(int i=0; i<p->nb_classes; i++) {
        t_classe *c = &p->tab_classes[i];
        printf("\nClasse %s : ", c->nom);

        if (c->est_puits) {
            printf("PERSISTANTE (Aucune sortie vers une autre classe). ");
            if (c->sommets->suiv == NULL) {
                printf("-> Le sommet %d est un ETAT ABSORBANT.", c->sommets->val);
            }
        } else {
            printf("TRANSITOIRE (On peut en sortir).");
        }
    }
    printf("\n");

    free(map_sommet_classe);
    for(int i=0; i<p->nb_classes; i++) free(mat_classe[i]);
    free(mat_classe);
}