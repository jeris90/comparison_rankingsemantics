#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "propa.h"

static int lt2(graph *gr, int node1, int node2, int limite)
{
    int i;
    for( i=0 ; i<limite; i++) /* for( i=0 ; i<((gr->nbNoeuds)*(gr->nbNoeuds)+1) ; i++) */
    {
        if ((gr->noeuds[node1].scoreNull[i] < gr->noeuds[node2].scoreNull[i]) ||
            ((gr->noeuds[node1].scoreNull[i] == gr->noeuds[node2].scoreNull[i])
             && (gr->noeuds[node1].score[i] < gr->noeuds[node2].score[i])))
        {
            /*printf("Plus petit, %.10f and %.10f = %f\n",gr->noeuds[node1].score[i],gr->noeuds[node2].score[i],gr->noeuds[node1].score[i]-gr->noeuds[node2].score[i]);*/
            return 1;
        }
        if ((gr->noeuds[node1].scoreNull[i] > gr->noeuds[node2].scoreNull[i]) ||
            ((gr->noeuds[node1].scoreNull[i] == gr->noeuds[node2].scoreNull[i])
             && (gr->noeuds[node1].score[i] > gr->noeuds[node2].score[i])))
        {
            /*printf("Plus grand, %.20f and %.20f = %f\n",gr->noeuds[node1].score[i],gr->noeuds[node2].score[i],gr->noeuds[node1].score[i]-gr->noeuds[node2].score[i]);*/
            return 0;
        }
    }
    return 2;
}

void bubbleSortScorePropa2(graph *gr)
{
    int i,tmp,size = (gr->nbNoeuds)-1;
    int fin = 1;
    while ((size > 0) && fin)
    {
        fin = 0;
        for (i = 0; i<size ; i++)
        {
            if (lt2(gr,gr->ordre[i],gr->ordre[i+1],(gr->nbNoeuds)+(gr->nbNoeuds)+1) == 1)
            {
                tmp = gr->ordre[i];
                gr->ordre[i] = gr->ordre[i+1];
                gr->ordre[i+1] = tmp;
                fin = 1;
            }
        }
        size--;
    }
}

void displayRankingPropa2(graph *gr, int seuil)
{
    int j;
    printf("%s",gr->noeuds[gr->ordre[0]].nom);
    for (j = 1 ; j < gr->nbNoeuds ; j++)
    {
        if (lt2(gr,gr->ordre[j-1],gr->ordre[j],seuil) == 2)
        {
            printf(" %s",gr->noeuds[gr->ordre[j]].nom);
        }
        else
            printf(">%s",gr->noeuds[gr->ordre[j]].nom);
    }
    printf("\n");
}

void p2lex(graph *gr, char **argv)
{
    double epsilon = (double)atof(argv[2]);
    double delta  = (double)atof(argv[3]);
    int seuil=0,set=0;
    const char *fichierEntree = argv[5];

    if (!strcmp(argv[4],"set"))
        set=1;
        
    gr = load_graph(fichierEntree);
    seuil = gr->nbNoeuds * gr->nbNoeuds + 1;
    calculScoreLexico(gr,epsilon,delta,seuil,set);
    bubbleSortScorePropa2(gr);
    displayRankingPropa2(gr,seuil);
}
