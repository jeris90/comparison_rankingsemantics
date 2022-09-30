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
void bubbleSortScoreBH(graph *gr, int max)
{
    int i,tmp,size = (gr->nbNoeuds)-1;
    int end = 1;
    while ((size > 0) && end)
    {
        end = 0;
        for (i = 0; i<size ; i++)
        {
            if (gr->noeuds[gr->ordre[i]].score[max] < gr->noeuds[gr->ordre[i+1]].score[max])
            {
                tmp = gr->ordre[i];
                gr->ordre[i] = gr->ordre[i+1];
                gr->ordre[i+1] = tmp;
                end = 1;
            }
        }
        size--;
    }
}

/***
  Function that assigns the initial score of 1 to all arguments (before starting the propagation phase)
  @param graph *gr : the graph representing the argumentation framwork
 ***/
static void initBH(graph *gr)
{
    int i;

    for (i=0 ; i < gr->nbNoeuds ; i++)
    {
        gr->noeuds[i].score[0] = 1.;
    }
}

void displayRankingBH(graph *gr,int max)
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

static int computeScoreBH(graph *gr)
{
    int node,j;
    double score = 0.;

    int step = 1; /* Variable indicating the number of steps needed to reach the fixed point */

    int pointFixe = 0; /*Variable indicating the number of arguments having reached its fixed point */

    /* Initialization of argument scores (Function that assigns the initial score of 1 to all arguments)*/
    initBH(gr);

    while (pointFixe != gr->nbNoeuds) /* as long as the fixed point has not been reached (i.e., there exists an argument such that 
                                        the absolute value of the difference between its scores at step i and at step i-1 is greater than DIFF)*/
    {
        pointFixe = 0;
        for (node=0 ; node < gr->nbNoeuds ; node++) /* we compute the new score of each argument*/ 
        {
            score = gr->noeuds[node].score[step-1];
            if (!gr->noeuds[node].nbAttacked) /* if the argument is not attacked, its score does not change (i.e. it is equal to the score at the previous step */
            {
                gr->noeuds[node].score[step] = score;
            }
            else /* if the argument is attacked by at least one argument, we compute its new score */
            {
                score = 0;
                for(j=0;j < gr->noeuds[node].nbAttacked;j++) /* We sum the score of all its direct attackers */
                {
                    score += gr->noeuds[gr->noeuds[node].attacked[j]].score[step-1];
                }
                score = 1. / (1. + score); /* we apply the formula of h-categoriser */
                gr->noeuds[node].score[step] = score; /* we store the score of this argument*/
            }
            if(fabs(gr->noeuds[node].score[step] - gr->noeuds[node].score[step-1]) < DIFF) /* we check if the absolute value of the difference 
                                            between its score at step this step and its score at the previous step is lower than a given value DIFF */
                pointFixe++;
        }
        step++;
    }
    return step-1;
}

void bh(graph *gr, char **argv)
{
    int laststep;
    const char *inputFile = argv[2];
    gr = load_graph(inputFile);
    laststep = computeScoreBH(gr);
    bubbleSortScoreBH(gr,laststep);
    displayRankingBH(gr,laststep);
}
