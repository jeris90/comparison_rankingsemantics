#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "propa.h"



/****
 Function allowing us to load the graph from the input file (in the ASPARTIX format) into the structure.
 @param s name of the file (path)
 @return return the graph
 */
graph* load_graph(const char *s)
{
    int i,num=0,n;
    char c;
    graph *gr = (graph *)malloc(sizeof(graph));
    gr = parser_ASPARTIX(s,gr);
    return gr;
}

/****
 Function that allows to free the memory space once the work with the graph is finished
 @param gr the graph
 */
void freeGraph(graph *gr)
{
    int i;
    free(gr->ordre);
    for (i=0 ; i<gr->nbNoeuds ; i++)
    {
        free(gr->noeuds[i].nom);
        free(gr->noeuds[i].attacker);
        free(gr->noeuds[i].attacked);
        free(gr->noeuds[i].score);
        free(gr->noeuds[i].scoreNull);
        free(gr->noeuds[i].branchLengthN);
    }
    free(gr->noeuds);
    free(gr);
}

/****
Function that displays the graph with, for each node, the display of its direct attackers and the arguments it attacks.
 @param gr the graph
 */
void displayAF(graph *gr)
{
    int i=0,j,k;
    int tmp = -1;
    for(;i<gr->nbNoeuds;i++)
    {
        printf("ARGUMENT %s\n",gr->noeuds[i].nom);
        printf("\t -> : ");
        for(j=0;j<gr->noeuds[i].nbAttacker;j++){
            tmp = gr->noeuds[i].attacker[j];
            printf("%s ",gr->noeuds[tmp].nom);
        }
        printf("\n");

        printf("\t <- : ");
        for(k=0;k<gr->noeuds[i].nbAttacked;k++){
            tmp = gr->noeuds[i].attacked[k];
            printf("%s ",gr->noeuds[tmp].nom);
        }
        printf("\n");
    }
}

