#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "propa.h"


int valueToNormalize(graph *gr)
{
    int i,j,tmp,max=0;
    for(i=0; i<gr->nbNoeuds ; i++)
    {
        tmp = gr->noeuds[i].nbAttacked;
        if(tmp > max)
            max = tmp;
    }
    return max;
}

/***********
 * MATRICE *
 ***********/

static void init(double **matrice, int nbLigne, int nbColonne)
{
    int i;
    for(i=0 ; i<nbLigne ; i++)
        matrice[i] = (double *)calloc(nbColonne,sizeof(double));
}

void multiplicationTwoSquareMatrix(double **m1, double **m2, double **res, int taille)
{
    int i,j,k;
    for(i=0;i<taille;i++)
    {
        for(j=0;j<taille;j++)
        {
            res[i][j] = 0;
            for(k=0;k<taille;k++)
            {
                res[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
}

void multiplicationMatrixAndDouble(double **m, double k, int taille)
{
    int i,j;
    for(i=0;i<taille;i++)
    {
        for(j=0;j<taille;j++)
            m[i][j] = m[i][j] * k;
    }
}

void divisionMatrixAndDouble(double **m, double k, int taille)
{
    int i,j;
    if(k)
    {
        for(i=0;i<taille;i++)
        {
            for(j=0;j<taille;j++)
                m[i][j] = m[i][j] / k;
        }
    }
}

void multiplicationMatrixAndTable(double **m, graph *gr, int step)
{
    int i,j;
    for(j=0;j<gr->nbNoeuds;j++)
    {
        gr->noeuds[j].score[step] = gr->noeuds[j].score[step-1];
        for(i=0;i<gr->nbNoeuds;i++)
        {
            if (step%2)
                gr->noeuds[j].score[step] -= m[j][i] * gr->noeuds[i].score[0];
            else
                gr->noeuds[j].score[step] += m[j][i] * gr->noeuds[i].score[0];
        }
    }
}

int isEmptyMatrix(double **m, int taille)
{
    int i,j;
    for(i=0;i<taille;i++)
    {
        for(j=0;j<taille;j++)
        {
            if(m[i][j])
                return 0;
        }
    }
    return 1;
}

void copyMatrice(double **m, double **res, int taille)
{
    int i,j;
    for(i=0;i<taille;i++)
    {
        for(j=0;j<taille;j++)
        {
            res[i][j] = m[i][j];
        }
    }
}



/***
    node1 plus petit que node2?
 */
static int lt(graph *gr, int node1, int node2, int max)
{
    if (gr->noeuds[node1].score[max] < gr->noeuds[node2].score[max])
        return 1;
    if (gr->noeuds[node1].score[max] > gr->noeuds[node2].score[max])
        return 0;
    return 2;
}


void bubbleSortScoreCS(graph *gr, int max)
{
    int i,tmp,size = (gr->nbNoeuds)-1,init = 0;
    int fin = 1;

    while ((size > 0) && fin)
    {
        fin = 0;
        for (i = init; i<size ; i++)
        {
            if (lt(gr,gr->ordre[i],gr->ordre[i+1],max) == 1)
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

static void displayRankingCS(graph *gr,int max)
{
    int j;
    printf("%s",gr->noeuds[gr->ordre[0]].nom);
    for (j = 1 ; j < gr->nbNoeuds ; j++)
    {
        if (lt(gr,gr->ordre[j-1],gr->ordre[j],max) == 2)
            printf(" %s",gr->noeuds[gr->ordre[j]].nom);
        else
            printf(">%s",gr->noeuds[gr->ordre[j]].nom);
    }
    printf("\n");
}

void initRes(graph *gr)
{
    int i;
    for(i=0;i<gr->nbNoeuds;i++)
    {
        gr->noeuds[i].score[0] = 1;
    }
 
}




/*void afficheScore(graph *gr, int step)
{
    int i;
    for(i=0;i<gr->nbNoeuds;i++)
        printf("%.5f   ",gr->noeuds[i].score[step]);
    printf("\n\n");
}*/

static void completeMatrice(double **matrice, graph *gr)
{
    int arg,att,nbAtt;
    
    for (arg=0 ; arg<gr->nbNoeuds ; arg++)
    {
        nbAtt = gr->noeuds[arg].nbAttacked;
        if(nbAtt)
        {
            for (att=0 ; att<nbAtt ; att++)
            {
                matrice[arg][gr->noeuds[arg].attacked[att]] = 1;
            }
        }
    }
}

int end(graph *gr, int step)
{
    int node;
    int nbStabilize = 0;
    for(node=0;node<gr->nbNoeuds;node++)
    {
        if(fabs(gr->noeuds[node].score[step] - gr->noeuds[node].score[step-1]) < DIFF)
            nbStabilize++;
    }
    return nbStabilize;
}

int iterationProcess(graph *gr, double **matriceInit, double deltaInit)
{
    int pointfixe = 0;
    int node;
    int step = 1;
    double delta = deltaInit;
    double **mat1 = (double **)calloc(gr->nbNoeuds,sizeof(double *));
    double **mat2 = (double **)calloc(gr->nbNoeuds,sizeof(double *));
    init(mat1,gr->nbNoeuds,gr->nbNoeuds);
    init(mat2,gr->nbNoeuds,gr->nbNoeuds);

    copyMatrice(matriceInit,mat1,gr->nbNoeuds);
    copyMatrice(matriceInit,mat2,gr->nbNoeuds);
    while(pointfixe != gr->nbNoeuds)
    {

        if(isEmptyMatrix(mat1,gr->nbNoeuds))
            break;

        if(step!=1)
        {
            multiplicationTwoSquareMatrix(mat1,matriceInit,mat2,gr->nbNoeuds);
        }
        
        multiplicationMatrixAndDouble(mat2,delta,gr->nbNoeuds);
        copyMatrice(mat2,mat1,gr->nbNoeuds);
        
        multiplicationMatrixAndTable(mat2,gr,step);

        pointfixe = end(gr,step);
        delta *= deltaInit;
        step++;
    }
    return step-1;
}


void cs(graph *gr, char **argv)
{
    int laststep;
    int norm;
    double **matriceInit;
    double delta  = (double)atof(argv[2]);
    const char *fichierEntree = argv[3];
    gr = load_graph(fichierEntree);

    matriceInit = (double **)calloc(gr->nbNoeuds,sizeof(double *));
    init(matriceInit,gr->nbNoeuds,gr->nbNoeuds);
    completeMatrice(matriceInit,gr);
    initRes(gr);
    norm = valueToNormalize(gr);
    divisionMatrixAndDouble(matriceInit,norm,gr->nbNoeuds);
    laststep = iterationProcess(gr,matriceInit,delta);
    bubbleSortScoreCS(gr,laststep);
    displayRankingCS(gr,laststep);
}
