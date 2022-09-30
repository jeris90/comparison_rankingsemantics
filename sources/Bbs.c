#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "propa.h"

static int lt(graph *gr, int node1, int node2, int limite)
{
    int i;
    for( i=0 ; i<limite; i++)
    {
        if ((gr->noeuds[node1].score[i] > gr->noeuds[node2].score[i]))
        {
            return 1;
        }
        if ((gr->noeuds[node1].score[i] < gr->noeuds[node2].score[i]))
        {
            return 0;
        }
    }
    return 2;
}

/**
    Function to sort arguments according to their degree of acceptability using the bubble sort algorithm.
    @param graph *gr : the graph representing the argumentation framwork
    @param int seuil : the number of rounds that were necessary to achieve convergence of the evaluation method
*/
void bubbleSortScoreBbs(graph *gr, int seuil)
{
    int i,tmp,size = (gr->nbNoeuds)-1;
    int fin = 1;
    while ((size > 0) && fin)
    {
        fin = 0;
        for (i = 0; i<size ; i++)
        {
            if (lt(gr,gr->ordre[i],gr->ordre[i+1],seuil) == 1)
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

/***
 * Function that assigns the initial score of 1 to all arguments 
 ***/
static void initBbs(graph *gr)
{
    int i;

    for (i=0 ; i < gr->nbNoeuds ; i++)
    {
        gr->noeuds[i].score[0] = 1.;
    }
}

void displayRankingBbs(graph *gr,int seuil)
{
    int j;
    printf("%s",gr->noeuds[gr->ordre[0]].nom);
    for (j = 1 ; j < gr->nbNoeuds ; j++)
    {
        if (lt(gr,gr->ordre[j-1],gr->ordre[j],seuil) == 2) /* Si equivalence */
            printf(" %s",gr->noeuds[gr->ordre[j]].nom);
        else
            printf(">%s",gr->noeuds[gr->ordre[j]].nom);
    }
    printf("\n");
}

static void computeScoreBbs(graph *gr, int stepmax)
{
    int step,node,j;
    double score = 0;

    initBbs(gr);
    for (step = 1; step < stepmax ; step ++)
    {
        for (node=0 ; node < gr->nbNoeuds ; node++)
        {
            score = 1;
            for(j=0;j < gr->noeuds[node].nbAttacked;j++)
            {
            	score += 1. / gr->noeuds[gr->noeuds[node].attacked[j]].score[step-1];
            }
            gr->noeuds[node].score[step] = score;
        }
    }
}

void bbs(graph *gr, char **argv)
{
    int seuil = 0;
    const char *fichierEntree = argv[2];
    gr = load_graph(fichierEntree);
    seuil = gr->nbNoeuds * gr->nbNoeuds + 1;
    computeScoreBbs(gr, seuil);
    bubbleSortScoreBbs(gr,seuil);
    displayRankingBbs(gr,seuil);
}
