#ifndef P1_INCLUDED
#define P1_INCLUDED

/* 
    Apesar de algumas estruras e funções serem iguais/parecidas às utilizadas na segunda parte do projeto, visto que
    alteramos as estruturas usadas na 2º parte e para mais fácil separação das 2 partes adiciona-se P1_ antes de cada
    função e estrutura para as distinguir das utilizadas na 2ºfase
*/

// Descrição das funções e das estruturas no ficheiro .c

typedef struct P1_node P1_link;
typedef struct P1_edge P1_Edge;
typedef struct P1_graph P1_Graph;

P1_link *P1_New(int, double, P1_link *);
P1_Graph *P1_GraphInit(FILE *,int,int,int);
void P1_GraphInsertE (P1_Graph *, P1_Edge *);
void P1_printgraph(P1_Graph *);
void P1_freegraph(P1_Graph *);
P1_Edge * P1_edgeInit(P1_Edge * );
P1_Edge * P1_edgeChange(P1_Edge *,int ,int , double );

int P1_f_A0(FILE *, int, int); 
double P1_f_B0(FILE *, int, int, int);
int P1_f_C0(P1_Graph *, int);
int P1_f_D0(P1_Graph *, int);
int P1_validateHeader(FILE *, FILE *, int *, int *, int *, int *, char *, double *);
FILE *P1_openFile(char *);
void P1_verify(int, char **);
void P1_Le_arestas(int, P1_Edge *, P1_Graph *, FILE *);

void P1_main(int argc, char *argv[]);

#endif // P1_INCLUDED