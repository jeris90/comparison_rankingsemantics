#define SIZE 1000000
#define DIFF 0.0000001

#define MAX 100000
#define TAILLE_MAX 1000

/* Argument */
typedef struct node
{
    char *nom;

    int nbAttacker; /* Number of arguments which are directly attacked by this argument */
    int nbAttacked; /* Number of arguments which directly attack this argument */
    int *attacker; /* Set of direct attackers */
    int *attacked; /* Set of direct attackees */
    double *score; /* Score of this score step by step */
    double *scoreNull; /* Score of this score when epsilon is 0 step by step (only for propagation)*/
    unsigned long int *branchLengthN; /* Number of times the argument at index i is present at the end of the branch of length N */
}noeud;


/* AF */
typedef struct graphe
{
    int nbNoeuds; /* Number of arguments */
    noeud *noeuds; /* List of arguments */
    int *ordre; /* Ranking between arguments */
}graph;


typedef struct tab
{
    int nbNoeuds; /* Number of non-attacked arguments in the AF */
    int *tabANA;  /* List of non-attacked arguments in the AF */
}tableau;


graph* load_graph(const char *s);
graph* parser_ASPARTIX(const char *s, graph *gr);

void displayAF(graph *gr);
void freeGraph(graph *gr);
void new_noeud(int num, graph *gr);
void new_attack(int atter, int atted, graph *gr);
void remove_attack(int atter, int atted, graph *gr);

void init1(graph *gr, double epsilon);
int unattacked(graph *gr, int node);
void nbBranchSet(graph *gr, int node, int size, int set);
int estPresent(tableau *t, int node);
void calculScoreLexico(graph *gr, float epsilon, float delta, int stepmax, int set);
void calculScoreLexicoMax(graph *gr, float epsilon, float delta, int stepmax, int set);

/* Propagation_{\epsilon} */
void p1lex(graph *gr, char **argv);

/* Propagation_{1+\epsilon} */
void p2lex(graph *gr, char **argv);

/* Propagation_{1 -> \epsilon} */
void p3lex(graph *gr, char **argv);

/* h-Categorizer */
void bh(graph *gr, char **argv);

/* Burden-based Semantics */
void bbs(graph *gr, char **argv);

/* Discussion-based Semantics */
void dbs(graph *gr, char **argv);

/* Fuzzy Label */
void fl(graph *gr, char **argv);

/* Alpha Burden-based Semantics */
void alphaBBS(graph *gr, char **argv);

/* Counting Semantics */
void cs(graph *gr, char **argv);
