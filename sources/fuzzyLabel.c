#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "propa.h"

void bubbleSortScoreFL(graph *gr, int max)
{
    int i,tmp,size = (gr->nbNoeuds)-1;
    int fin = 1;

    while ((size > 0) && fin)
    {
        fin = 0;
        for (i = 0; i<size ; i++)
        {
            if (gr->noeuds[gr->ordre[i]].score[max] < gr->noeuds[gr->ordre[i+1]].score[max])
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

static void initFL(graph *gr)
{
    int i;

    for (i=0 ; i < gr->nbNoeuds ; i++)
    {
        gr->noeuds[i].score[0] = 1.;
    }
}

void displayRankingFL(graph *gr,int max)
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

int computeScoreFL(graph *gr)
{
    int step = 1,node,j;
    double score = 0. , max = 0., tmp = 0.;
    int pointFixe = 0;
    initFL(gr);
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
                max = 0.;
                for(j=0;j < gr->noeuds[node].nbAttacked;j++)
                {
                    tmp = gr->noeuds[gr->noeuds[node].attacked[j]].score[step-1];
                    if (tmp > max)
                        max = tmp;
                }
                score = 0.5 * ( 1 + score - max );
                gr->noeuds[node].score[step] = score;
            }
            if(fabs(gr->noeuds[node].score[step] - gr->noeuds[node].score[step-1]) < DIFF)
                pointFixe++;
        }
        step++;
    }
    for (node=0; node < gr->nbNoeuds ; node++)
    {
        if(gr->noeuds[node].score[step-1]>0.99)
            gr->noeuds[node].score[step] = 1;
        if(gr->noeuds[node].score[step-1]<0.01)
            gr->noeuds[node].score[step] = 0;
        if(gr->noeuds[node].score[step-1]>0.49 && gr->noeuds[node].score[step-1]<0.61)
            gr->noeuds[node].score[step] = 0.5;
    }
    return step;
}

void fl(graph *gr, char **argv)
{
    int laststep;
    const char *fichierEntree = argv[2];
    gr = load_graph(fichierEntree);
    laststep = computeScoreFL(gr);
    bubbleSortScoreFL(gr,laststep);
    displayRankingFL(gr,laststep);
}
