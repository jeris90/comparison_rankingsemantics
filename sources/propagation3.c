#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "propa.h"


void calculScoreLexicoMax(graph *gr, float epsilon, float delta, int stepmax, int set)
{
    int step,node,i;
    double score = 0, score2 = 0,valmax = 0,valmaxNull=0,tmp=0,tmpNull=0;

    init1(gr,epsilon);
    for (step = 1; step < stepmax ; step ++)
    {
        for (node=0 ; node < gr->nbNoeuds ; node++)
        {
            score = gr->noeuds[node].score[step-1];
            score2 = gr->noeuds[node].scoreNull[step-1];

            nbBranchSet(gr,node,step,set);
            if (unattacked(gr,node))
            {
                gr->noeuds[node].score[step] = score;
                gr->noeuds[node].scoreNull[step] = score2;
            }
            else
            {
                valmax=0;
                valmaxNull=0;
                if (step%2)
                {
                    for( i=0 ; i<gr->nbNoeuds ; i++)
                    {
                        if(gr->noeuds[node].branchLengthN[i])
                        {
                            tmp = (gr->noeuds[i].score[0])*pow(delta,step);
                            if (tmp > valmax)
                                valmax = tmp;
                            tmpNull = (gr->noeuds[i].scoreNull[0])*pow(delta,step);
                            if (tmpNull > valmaxNull)
                                valmaxNull = tmpNull;
                        }
                    }
                    score -= valmax;
                    score2 -= valmaxNull;
                }
                else
                {
                    for( i=0 ; i<gr->nbNoeuds ; i++)
                    {
                        if(gr->noeuds[node].branchLengthN[i])
                        {
                            tmp = (gr->noeuds[i].score[0])*pow(delta,step);
                            if (tmp > valmax)
                                valmax = tmp;
                            tmpNull = (gr->noeuds[i].scoreNull[0])*pow(delta,step);
                            if (tmpNull > valmaxNull)
                                valmaxNull = tmpNull;
                        }
                    }
                    score += valmax;
                    score2 += valmaxNull;
                }

                gr->noeuds[node].score[step] = score;
                gr->noeuds[node].scoreNull[step] = score2;
            }
        }
    }
}
    

static int lt3(graph *gr, int node1, int node2, int limite)
{
    int i;
    for( i=0 ; i<limite; i++) /* for( i=0 ; i<((gr->nbNoeuds)*(gr->nbNoeuds)+1) ; i++) */
    {
        if (gr->noeuds[node1].scoreNull[i] < gr->noeuds[node2].scoreNull[i])
        {
            return 1;
        }
        if (gr->noeuds[node1].scoreNull[i] > gr->noeuds[node2].scoreNull[i])
        {
            return 0;
        }
    }
    for( i=0 ; i<limite; i++) /* for( i=0 ; i<((gr->nbNoeuds)*(gr->nbNoeuds)+1) ; i++) */
    {
        if (gr->noeuds[node1].score[i] < gr->noeuds[node2].score[i])
        {
            return 1;
        }
        if (gr->noeuds[node1].score[i] > gr->noeuds[node2].score[i])
        {
            return 0;
        }
    }
    return 2;
}

void bubbleSortScorePropa3(graph *gr)
{
    int i,tmp,size = (gr->nbNoeuds)-1;
    int fin = 1;
    while ((size > 0) && fin)
    {
        fin = 0;
        for (i = 0; i<size ; i++)
        {
            if (lt3(gr,gr->ordre[i],gr->ordre[i+1],(gr->nbNoeuds)*(gr->nbNoeuds)+1) == 1)
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


void displayRankingPropa3(graph *gr,int seuil)
{
    int j;
    printf("%s",gr->noeuds[gr->ordre[0]].nom);
    for (j = 1 ; j < gr->nbNoeuds ; j++)
    {
        if (lt3(gr,gr->ordre[j-1],gr->ordre[j],seuil) == 2)
            printf(" %s",gr->noeuds[gr->ordre[j]].nom);
        else
            printf(">%s",gr->noeuds[gr->ordre[j]].nom);
    }
    printf("\n");
}

void p3lex(graph *gr, char **argv)
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
    bubbleSortScorePropa3(gr);
    displayRankingPropa3(gr,seuil);

}
