#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "propa.h"

/****
 Function that extracts the argument arg(arg)
 */
static void extractArgument(char *string, char *arg1)
{
    int i,j=0;

    for(i=4 ; string[i]!=')' ; i++)
    {
        arg1[j++] = string[i];
    }
    arg1[j] = '\0';
}

/****
 Function to add an argument to the graph
 */
static void new_argumentAPX(int num, graph *gr, char *string)
{
    char *arg = (char *)calloc(TAILLE_MAX,sizeof(char));
    extractArgument(string,arg);
    strcpy(gr->noeuds[num].nom,arg);
    free(arg);
}

/****
 Function that initializes the components of an argument
 */
static void init_argument(graph *gr)
{
    int num;
    gr->ordre = (int *)malloc(sizeof(int)*MAX);
    for (num=0 ; num<gr->nbNoeuds ; num++)
    {
        gr->ordre[num] = num;
        gr->noeuds[num].nbAttacker = 0;
        gr->noeuds[num].nbAttacked = 0;
        gr->noeuds[num].attacker = (int *)malloc(sizeof(int)*gr->nbNoeuds);
        gr->noeuds[num].attacked = (int *)malloc(sizeof(int)*gr->nbNoeuds);
        gr->noeuds[num].branchLengthN = (unsigned long int *)calloc((gr->nbNoeuds),sizeof(unsigned long int));
        gr->noeuds[num].score = (double *)calloc(SIZE,sizeof(double));
        gr->noeuds[num].scoreNull = (double *)calloc(SIZE,sizeof(double));
    }
}

/****
 Function that extracts the two arguments involved in the attack att(arg1,arg2)
 */
static void extractAttack(char *chaine, char *arg1, char *arg2)
{
    int i,j=0;

    for(i=4 ; chaine[i]!=',' ; i++)
    {
        arg1[j++] = chaine[i];
    }
    arg1[j] = '\0';
    i++; /* skip the comma */
    j=0;
    for(;chaine[i]!=')';i++)
        arg2[j++] = chaine[i];
    arg2[j] = '\0';
}

static void new_attackAPX(char *chaine, graph *gr)
{
    char *comp = (char *)calloc(TAILLE_MAX,sizeof(char));
    char *arg1 = (char *)calloc(TAILLE_MAX,sizeof(char));
    char *arg2 = (char *)calloc(TAILLE_MAX,sizeof(char));
    int i,a1=-1,a2=-1;
    extractAttack(chaine,arg1,arg2);

    for( i=0 ; i<gr->nbNoeuds ; i++)
    {
        strcpy(comp,gr->noeuds[i].nom);
        if (!strcmp(comp,arg1))
            a1 = i;
        if (!strcmp(comp,arg2))
            a2 = i;
    }
    if((a1 == -1) || (a2 == -1))
    {
        printf("%d (%s) %d (%s)\n",a1,arg1,a2,arg2);
        perror("Nonexistent Argument in the set of attacks"),exit(1);
    }
    gr->noeuds[a1].attacker[gr->noeuds[a1].nbAttacker] = a2;
    gr->noeuds[a2].attacked[gr->noeuds[a2].nbAttacked] = a1;

    gr->noeuds[a1].nbAttacker++;
    gr->noeuds[a2].nbAttacked++;
    free(comp);
    free(arg1);
    free(arg2);
}





/****
 Function to load the graph described in the text file into the structure
 @s Name of the text file containing the graph
 @type returns 1 if the lexicographical order is used, 0 if the fixed point method is used.
 #gr returns the graph with the filled structure
 */
graph* parser_ASPARTIX(const char *s, graph *gr)
{
    int i;
    int existAttack = 0;
    char c;
    FILE *f;
    char *chaine = (char *)calloc(TAILLE_MAX,sizeof(char));

    if(!(f=fopen(s,"r")))
        perror("Unreadable file: "),exit(1);
    gr->nbNoeuds = 0;
    gr->noeuds=(noeud *)calloc(MAX,sizeof(noeud));
    while (fgets(chaine, TAILLE_MAX, f) != NULL)
    {
        if(strstr(chaine,"arg") != NULL)
        {
            gr->noeuds[gr->nbNoeuds].nom = (char *)calloc(TAILLE_MAX,sizeof(char));
            new_argumentAPX(gr->nbNoeuds,gr,chaine);
            gr->nbNoeuds++;
        }
        else
        {
            existAttack = 1;
            break;
        }
    }
    init_argument(gr);
    if(existAttack)
    {
        new_attackAPX(chaine,gr);
        while (fgets(chaine, TAILLE_MAX, f) != NULL)
        {
            if(strstr(chaine,"att") != NULL)
            {
                new_attackAPX(chaine,gr);
            }
            else
            {
            	break;
            }
        }
    }

    fclose(f);
    free(chaine);
    return gr;
}


