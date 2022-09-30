#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include "propa.h"

#define NBSEM 9


void init1(graph *gr, double epsilon)
{
    int i;

    for (i=0 ; i < gr->nbNoeuds ; i++)
    {
        if (gr->noeuds[i].nbAttacked)
        {
            gr->noeuds[i].score[0] = epsilon;
            gr->noeuds[i].scoreNull[0] = 0;
        }
        else
        {
            gr->noeuds[i].score[0] = 1.;
            gr->noeuds[i].scoreNull[0] = 1.;
        }

        gr->noeuds[i].branchLengthN[i] = 1;
    }
}

int belongTo(int tab[], int valeur, int size)
{
    int i;
    for(i=0;i<size;i++)
    {
        if (tab[i] == valeur)
            return i;
    }
    return -1;
}

int minimalArg(graph *gr, int node)
{
    int *dejaPasser = (int *)calloc(gr->nbNoeuds,sizeof(int));
    int *aVisiter = (int *)calloc(gr->nbNoeuds,sizeof(int));
    int val,j,res=0;
    aVisiter[node] = 1;
    while((val = belongTo(aVisiter,1,gr->nbNoeuds)) != -1)
    {
        aVisiter[val] = 0;
        if(!dejaPasser[val])
        {
        	if (res)  dejaPasser[val] = 1;
            res++;
            for(j=0;j < gr->noeuds[val].nbAttacked;j++)
            {
                if(!dejaPasser[gr->noeuds[val].attacked[j]])
                    aVisiter[gr->noeuds[val].attacked[j]] = 1;
            }
        }
    }
    free(dejaPasser);
    free(aVisiter);
    return res;
}

int seuilMinimal(graph *gr)
{
    int seuil = 0;
    int new = 0, i;

    for(i=0 ; i < gr->nbNoeuds ; i++)
    {
        new = minimalArg(gr,i);
        if (new > seuil)
            seuil = new;
    }
    return seuil;
}

void nbBranchSet(graph *gr, int node, int size, int set)
{
    int i, j;
	unsigned long int nb = 0;
    unsigned long int *tmp = (unsigned long int *)calloc(gr->nbNoeuds,sizeof(unsigned long int));
    for ( i=0; i<gr->nbNoeuds ; i++)
    {
        if(set)
            if (gr->noeuds[node].branchLengthN[i] == 0) continue;
        j=0;
        while (j < gr->noeuds[i].nbAttacked)
        {
            nb = gr->noeuds[node].branchLengthN[i];

            if(set)
            {
                /*if(!tmp[gr->noeuds[i].attacked[j]])*/
                    tmp[gr->noeuds[i].attacked[j]] |= 1; /* ++ */
            }
            else
            {
                tmp[gr->noeuds[i].attacked[j]] += nb;
            }
            j++;
        }
    }
    for (i=0; i<gr->nbNoeuds ;i++)
    {
        gr->noeuds[node].branchLengthN[i] = tmp[i];
    }
    free(tmp);
}




int unattacked(graph *gr, int node)
{
    int i;
    for(i=0;i<gr->nbNoeuds;i++)
    {
        if (gr->noeuds[node].branchLengthN[i] != 0)
            return 0;
    }
    return 1;
}

void calculScoreLexico(graph *gr, float epsilon, float delta, int stepmax, int set)
{
    int step,node,i;
    double score = 0, score2 = 0;

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

                if (step%2)
                {
                    for( i=0 ; i<gr->nbNoeuds ; i++)
                    {
                        score -= gr->noeuds[node].branchLengthN[i] * ((gr->noeuds[i].score[0])*pow(delta,step));
                        score2 -= gr->noeuds[node].branchLengthN[i] * ((gr->noeuds[i].scoreNull[0])*pow(delta,step));
                    }
                }
                else
                {
                    for( i=0 ; i<gr->nbNoeuds ; i++)
                    {
                        score += gr->noeuds[node].branchLengthN[i] * ((gr->noeuds[i].score[0])*pow(delta,step));
                        score2 += gr->noeuds[node].branchLengthN[i] * ((gr->noeuds[i].scoreNull[0])*pow(delta,step));
                    }
                }

                gr->noeuds[node].score[step] = score;
                gr->noeuds[node].scoreNull[step] = score2;
            }
        }
    }
}



int main(int argc, char **argv)
{
    int i;
    graph *gr = NULL;
    char *sem[NBSEM] = {"CAT","BBS","DBS","FL","PROPA1","PROPA2","PROPA3","alphaBBS","CS"};

    void(*fsem[NBSEM])(graph*,char**) = {bh,bbs,dbs,fl,p1lex,p2lex,p3lex,alphaBBS,cs};

    for(i=0;i<NBSEM;i++)
    {
        if(!strcmp(argv[1],sem[i]))
            fsem[i](gr,argv);
    }
    return 0;
}
