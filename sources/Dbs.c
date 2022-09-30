#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "propa.h"

static int lt(graph *gr, int node1, int node2, int limite)
{
    int i;
    for( i=0 ; i<limite; i++) /* for( i=0 ; i<((gr->nbNoeuds)*(gr->nbNoeuds)+1) ; i++) */
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
void bubbleSortScoreDbs(graph *gr, int seuil)
{
    int i,tmp,size = (gr->nbNoeuds)-1;
    int fin = 1;
    while ((size > 0) && fin)
    {
        fin = 0;
        for (i = 0; i<size ; i++)
        {
            if (lt(gr,gr->ordre[i],gr->ordre[i+1],seuil) == 1) /*(gr->nbNoeuds)*(gr->nbNoeuds)+1*/
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

static void initDbs(graph *gr)
{
    int i;

    for (i=0 ; i < gr->nbNoeuds ; i++)
    {
        gr->noeuds[i].score[0] = -1;
        gr->noeuds[i].branchLengthN[i] = 1;
    }
}

void displayRankingDbs(graph *gr,int seuil)
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

void computeScoreDbs(graph *gr, int stepmax, int set)
{
    int step,node,j;
    double score = 0;

    initDbs(gr);
    for (step = 1; step < stepmax ; step ++)
    {
        for (node=0 ; node < gr->nbNoeuds ; node++)
        {
        	nbBranchSet(gr,node,step,set);
            score = 0;
            for(j=0 ; j<gr->nbNoeuds ; j++)
            {
            	score += gr->noeuds[node].branchLengthN[j];
            }
            if (step%2)
            	gr->noeuds[node].score[step] = score;
            else
            	gr->noeuds[node].score[step] = (-1) * score;
        }
    }
}

void dbs(graph *gr, char **argv)
{
    int seuil = 0, set = 0;
    const char *fichierEntree = argv[2];
    gr = load_graph(fichierEntree);
    seuil = gr->nbNoeuds * gr->nbNoeuds + 1;
    computeScoreDbs(gr, seuil, set);
    bubbleSortScoreDbs(gr,seuil);
    displayRankingDbs(gr,seuil);
}

