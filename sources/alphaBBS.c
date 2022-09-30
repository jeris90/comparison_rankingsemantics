#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "propa.h"

/**
    Function to sort arguments according to their degree of acceptability using the bubble sort algorithm.
    @param graph *gr : the graph representing the argumentation framwork
    @param int max : the number of rounds that were necessary to achieve convergence of the evaluation method
*/
void bubbleSortScoreAlphaBBS(graph *gr, int max)
{
    int i,tmp,size = (gr->nbNoeuds)-1;
    int fin = 1;
    while ((size > 0) && fin)
    {
        fin = 0;
        for (i = 0; i<size ; i++)
        {
            if (gr->noeuds[gr->ordre[i]].score[max] > gr->noeuds[gr->ordre[i+1]].score[max])
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
  Function that assigns the initial score of 1 to all arguments (before starting the propagation phase)
  @param graph *gr : the graph representing the argumentation framwork
 ***/
static void initalphaBBS(graph *gr)
{
    int i;

    for (i=0 ; i < gr->nbNoeuds ; i++)
    {
        gr->noeuds[i].score[0] = 1.;
    }
}

void displayRankingAlphaBBS(graph *gr,int max)
{
    int j;
    printf("%s",gr->noeuds[gr->ordre[0]].nom);
    for (j = 1 ; j < gr->nbNoeuds ; j++)
    {
        if (gr->noeuds[gr->ordre[j-1]].score[max] == gr->noeuds[gr->ordre[j]].score[max]) /* Si equivalence */
            printf(" %s",gr->noeuds[gr->ordre[j]].nom);
        else
            printf(">%s",gr->noeuds[gr->ordre[j]].nom);
    }
    printf("\n");
}

int computeScoreAlphaBBS(graph *gr, double alpha)
{
    int step = 1,node,j;
    double score = 0.;
    int pointFixe = 0;
    initalphaBBS(gr);
    while (pointFixe != gr->nbNoeuds)
    {
        pointFixe = 0;
        for (node=0 ; node < gr->nbNoeuds ; node++)
        {
            score = gr->noeuds[node].score[step-1];
            if (!gr->noeuds[node].nbAttacked)
            {
                gr->noeuds[node].score[step] = score;
            }
            else
            {
                score = 0;
                for(j=0;j < gr->noeuds[node].nbAttacked;j++)
                {
                    score += 1. / pow(gr->noeuds[gr->noeuds[node].attacked[j]].score[step-1],alpha);
                }
                score = 1 + pow(score,1/alpha);
                gr->noeuds[node].score[step] = score;
            }
            if(fabs(gr->noeuds[node].score[step] - gr->noeuds[node].score[step-1]) < DIFF)
                pointFixe++;
        }
        step++;
    }
    return step-1;
}

void alphaBBS(graph *gr, char **argv)
{
    int laststep;
    double alpha = (double)atof(argv[2]);
    const char *fichierEntree = argv[3];
    
    gr = load_graph(fichierEntree);
    laststep = computeScoreAlphaBBS(gr,alpha);
    bubbleSortScoreAlphaBBS(gr,laststep);
    displayRankingAlphaBBS(gr,laststep);
}
