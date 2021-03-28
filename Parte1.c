#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Parte1.h"


/*
 *  Data Type: node
 *
 *  Description: Structure with:
 *      1) Vertice
 *      2) Custo da aresta
 *      3) Ponteiro para o próximo vertice adjacente
 */
struct P1_node {int v; double custo ; P1_link *next;};

/*
 *  Data Type: graph
 *
 *  Description: Structure with:
 *      1) Numero total de vertices
 *      2) Numero total de arestas
 *      3) Ponteiro para tabela de listas de adjacencias
 *      4) Ponteiro para tabela de ligações a um vertice pedido
 *      5) Vertice pedido a que corresponde a tabela de ligações
 */
struct P1_graph{int V; int E; P1_link **adj; char *linked; int vertice;};

/*
 *  Data Type: edge
 *
 *  Description: Structure with:
 *      1) Vertice 1 da aresta
 *      2) Vertice 2 da aresta
 *      3) Custo da aresta
 */
struct P1_edge {int v; int w; double custo;};

/*
 *  Function:
 *    f_A0
 *
 *  Description:
 *    Procura o grau de um certo vertice
 *
 *  Arguments:
 *      -> Ficheiro de entrada
 *      -> Vertice
 *      -> Numero total de arestas
 *
 *  Return value:
 *      -> Grau do vertice
 */
int P1_f_A0(FILE *fp, int vertice, int E)
{
    int v, w;
    double aux;
    int grau=0;
 
    for (int i = 0; i < E; i++) {
        if (fscanf(fp, "%d %d %lf", &v, &w, &aux) != 3)     // Lemos cada aresta
            exit(0);
        v--;   
        w--;

        if ((v == vertice) || (w == vertice)) { // Se encontarmos uma aresta em que o vertice esteja la entao aumentamos o grau
            grau ++;
        }
    }
    return grau;
}

/*
 *  Function:
 *    f_B0
 *
 *  Description:
 *    Procura se 2 vertices sao adjacentes
 *
 *  Arguments:
 *      -> Ficheiro de entrada
 *      -> Vertice 1
 *      -> Vertice 2
 *      -> Numero total de arestas
 *
 *  Return value:
 *      -> -1 se nao forem adjacentes ou então o custo da aresta que une os vertices
 */
double P1_f_B0(FILE *fp, int v1, int v2, int E)
{
    int v, w;
    double custo, returnc = -1;
    for (int i = 0; i < E; i++) {
        if (fscanf(fp, "%d %d %lf", &v, &w, &custo) != 3)       // Lemos a aresta do ficheiro
            exit(0);
        v--;
        w--;
        if (((v == v1) && (w == v2)) ||((w == v1) && (v == v2)))    // Se encontrarmos uma aresta com os 2 vertices entao e porque sao adjacentes
            returnc = custo;
    }

    if (returnc > -1)
        return returnc;
    return -1;
}

/*
 *  Function:
 *    f_C0
 *
 *  Description:
 *    Procura se o  vertice esta numa clique de 3
 *
 *  Arguments:
 *      -> Grafo
 *      -> Vertice 
 *      
 *
 *  Return value:
 *      -> 1 se pertence ou 0 se nao pertence
 */
int P1_f_C0(P1_Graph * G, int vertice)
{
    P1_link * aux1 = G->adj[vertice];
    P1_link * aux2 = NULL;


    while(aux1!=NULL)   // Procura-se todos os vertices adjacentes do vertice pedido
    {
        aux2 = G->adj[aux1->v];

        while(aux2!=NULL)   //Procura-se todos os vertices adjacentes do (vertice adjacente do vertice pedido)
        {
            if(aux2->v == vertice)  // Se for igual ao vertice inicialmente pedido ignoramos
            {
                aux2 = aux2->next;
                continue;
            }

            if(G->linked[aux2->v])  // Caso o vertice inicial seja adjacente deste segundo vertice encontrado entao encontramos um clique de 3 (O(1) tal como nas tabela de adjacencias)
                return 1;

            aux2= aux2->next;
        }
        aux1 = aux1->next;
    }

    return 0;

}

/*
 *  Function:
 *    f_D0
 *
 *  Description:
 *    Procura em quantas cliques de 3 esta um vertice
 *
 *  Arguments:
 *      -> Grafo
 *      -> Vertice
 *  
 *
 *  Return value:
 *      -> numero de cliques de 3 em que vertice faz parte
 */
int P1_f_D0(P1_Graph * G, int vertice)
{
    P1_link * aux1 = G->adj[vertice];
    P1_link * aux2 = NULL;
    int cliques = 0;

    while(aux1!=NULL)   // Procura-se todos os vertices adjacentes do vertice pedido
    {
        aux2 = G->adj[aux1->v];

        while(aux2!=NULL)   //Procura-se todos os vertices adjacentes do (vertice adjacente do vertice pedido)
        {

            if(aux2->v == vertice || aux2->v >= aux1->v)    // Se for igual ao vertice inicialmente pedido ignoramos
            {                                               // ou entao se for maior que o outro vertice
                aux2= aux2->next;                           // isto para evitar percorrer o mesmo clique 2x (em 2 vertices necessariamente um e maior ou igual que o outro)
                continue;
            }

            if(G->linked[aux2->v])      // Caso o vertice inicial seja adjacente deste segundo vertice encontrado entao encontramos um clique de 3 (O(1) tal como nas tabela de adjacencias)
                cliques++;

            aux2= aux2->next;
        }
        aux1 = aux1->next;
    }

    return cliques;

}

/*
 *  Function:
 *    verify
 *
 *  Description:
 *    Verifica erros de invocação do programa
 *
 *  Arguments:
 *      -> Numero de argumentos do programa
 *      -> Argumentos
 *  
 *
 *  Return value:
 *      None
 */

void P1_verify(int argc, char *argv[])
{
    if (argc != 2)      // Caso nao tenhamos so 2 argumentos
    {
        //printf("Invalid number of agruments\n");
        exit(0);
    }
    if (strcmp(strrchr(argv[1], '.'), ".routes0") != 0)     // Caso a extensão não seja .routes0
    {
        //printf("Invalid format (should be *.routes0)\n");
        exit(0);
    }
    return;
}

/*
 *  Function:
 *    openFile
 *
 *  Description:
 *    Cria o ficheiro de output mudando-lhe a extensão
 *
 *  Arguments:
 *      -> Nome do ficheiro de entrada
 *     
 *  
 *
 *  Return value:
 *      -> Apontador para o ficheiro aberto
 */
FILE *P1_openFile(char *filename)
{
    char *word;
    FILE *fp;

    filename[strlen(filename) - strlen(".routes0")] = '\0'; // Para o strcpy so copiar até este \0 (ignorando a extensão)

    word = (char *)malloc((strlen(filename) + strlen(".queries") + 1) * sizeof(char));

    if (word== NULL) // Erro de alocacao
      exit(0);

    if (word == NULL)
    {
        fprintf(stderr, "ERROR: allocation of output filename. Not enough memory.\n");
        exit(0);
    }

    strcpy(word, filename);     // Cria-se o ficheiro de saida com a extensão correta
    strcat(word, ".queries");
    fp = fopen(word, "w");
    free(word);
    return fp;
}

/*
 *  Function:
 *    validateHeader
 *
 *  Description:
 *    Verifica de o cabecalho tem um vertice invalido
 *
 *  Arguments:
 *      -> Ficheiro de entrada
 *      -> Ficheiro de saida
 *      -> Vertice 1
 *      -> Vertice 2
 *      -> Numero total de vertices
 *      -> Numero total de arestas
 *      -> Cabecalho do grafo
 *      -> Valor do custo de uma aresta
 *      
 *  
 *
 *  Return value:
 *      -> Int que vai dizer se o main tem de dar break ou continue
 */
int P1_validateHeader(FILE *fp, FILE *fout, int *v1, int *v2, int *V, int *E, char *cabecalho, double *temp)
{
    int k;
    
    if (fscanf(fp, "%d %d %s", V, E, cabecalho) != 3)       // Caso nao consiga ler o cabecalho e porque o ficheiro acabou
        return -1;

    fprintf(fout, "%d %d %s ", *V, *E, cabecalho);      //Output

    if (strcmp(cabecalho, "B0") == 0)       // Se for modo B0 quermemos ler 2 vertices para o problema
    {
        
        if (fscanf(fp, "%d %d", v1, v2) != 2)
            exit(0);
        fprintf(fout, "%d %d ", *v1, *v2);

        if (!((*v1 > 0) & (*v1 <= *V) & (*v2 > 0) & (*v2 <= *V)))   // Verifica se ambos os vertices lidos sao validos
        {
            fprintf(fout, "-1\n\n");
            for (k = 0; k < *E; k++)        // Descarta do ficheiro o resto das arestas
            {
                if (fscanf(fp, "%d %d %lf", v1, v2, temp) != 3)
                    exit(0);
            }
            return 1;
        }
        (*v2)--;
    }
    else    // Se for outro modo apenas queremos ler 1
    {
        if (fscanf(fp, "%d", v1) != 1)
            exit(0);

        fprintf(fout, "%d ", *v1);  // Output

        if (!((*v1 > 0) & (*v1 <= *V)))     // Verifica se o vertice é valido
        {
            fprintf(fout, "-1\n\n");
            for (k = 0; k < *E; k++)     // Descarta do ficheiro o resto das arestas
            {
                if (fscanf(fp, "%d %d %lf", v1, v2, temp) != 3)
                    exit(0);
            }
            return 1;
        }
    }

    (*v1)--;  // Porque para indexar tabelas é de 0 a V-1

    return 0;
}

/*
 *  Function:
 *    Le_arestas
 *
 *  Description:
 *    Le do ficheiro totas as arestas de um grafo e mete em memoria
 *
 *  Arguments:
 *      -> Vertice 1
 *      -> Vertice 2
 *      -> Custo aresta
 *      -> Numero total de arestas
 *      -> Aresta
 *      -> Grafo
 *      -> Ficheiro de entrada
 *  
 *
 *  Return value:
 *      None
 */
void P1_Le_arestas(int E, P1_Edge *e, P1_Graph *G, FILE * fp)
{
    int i, v1, v2;
    double temp;
    for (i = 0; i < E; i++)        // Le todas as arestas
    {
        if (fscanf(fp, "%d %d %lf", &v1, &v2, &temp) != 3)
            exit(0);
        v1--;
        v2--;
        e = P1_edgeChange(e, v1, v2, temp);
        P1_GraphInsertE(G, e); // E insere no grafo
    }
}

/*
 *  Function:
 *    New
 *
 *  Description:
 *    Cria o no de uma lista
 *
 *  Arguments:
 *      -> Vertice a inserir
 *      -> Custo
 *      -> Ponteiro para o proximo no
 *
 *  Return value:
 *      -> Retorna um ponteiro para o no criado
 */
P1_link *P1_New(int v, double custo, P1_link *next) {

    P1_link *x = (P1_link *) malloc(sizeof(P1_link));    // Cria no da lista

    if (x == NULL) // Erro de alocacao
      exit(0);
    x->v = v;
    x->next = next;
    x->custo = custo;
    return x;
}

/*
 *  Function:
 *    GraphInit
 *
 *  Description:
 *    Inicia o grafo
 *
 *  Arguments:
 *      -> Ficheiro de entrada
 *      -> Numero total de vertices
 *      -> Numero total de arestas
 *      -> Vertice que vai ter a tabela de adjacencias
 *
 *  Return value:
 *      -> Pointer para o grafo criado
 */
P1_Graph *P1_GraphInit(FILE *fp,int V,int E,int vertice) {

    P1_Graph *G = (P1_Graph*) malloc(sizeof(struct P1_graph));  

    if (G == NULL) // Erro de alocacao
        exit(0); 

    G->V = V;
    G->E = E;
    G->adj = (P1_link **) malloc((G->V) * sizeof(P1_link *)); // Cria a tabela da lista de adjacências

    if (G->adj == NULL) // Erro de alocacao
        exit(0);

    G->linked = (char *) calloc(G->V, sizeof(char));     // Cria tabela auxiliar de adjacências do vertice pedido 

    if (G->linked == NULL) // Erro de alocacao
        exit(0);

    G->vertice = vertice;
    
    for (int i = 0; i < G->V; i++) {       // Inicializa as listas a NULL
        G->adj[i] = NULL;
    }


    return G;
}

/*
 *  Function:
 *    GraphInsertE
 *
 *  Description:
 *    Introduz uma aresta no grafo
 *
 *  Arguments:
 *      -> Grafo
 *      -> Aresta a inserir
 *   
 *
 *  Return value:
 *      -------
 */

void P1_GraphInsertE(P1_Graph *G, P1_Edge *e) {
    G->adj[e->v] = P1_New(e->w, e->custo, G->adj[e->v]);       // Cria ambos os nos a serem inseridos
    G->adj[e->w] = P1_New(e->v, e->custo, G->adj[e->w]);
    
    if (G->linked != NULL) {        // Registamos as adjacências do vertice pedido no problema na tabela auxiliar 
        if (e->v == G->vertice)     // (para que depois o acesso seja O(1) tal como na tabela de adjacencias)
            G->linked[e->w] ++;
        if (e->w == G->vertice)
            G->linked[e->v] ++;  
    }
    return;
}

/*
 *  Function:
 *    printgraph
 *
 *  Description:
 *    Da print do grafo no terminal (usado para debug)
 *
 *  Arguments:
 *      -> Grafo a imprimir
 *
 *
 *  Return value:
 *      -> Pointer para o grafo criado
 */
void P1_printgraph(P1_Graph *G) {
    P1_link *aux;
    int count = 0;
    for (int i = 0; i < G->V; i++) {    // Percorremos a tabela de lista de adjacêncais
        count = 0;
        aux = G->adj[i];            
        printf("%d: ", i + 1);
        while (aux != NULL) 
        {   
            if (count != 0)
                printf(" -> ");
            printf("%d", aux->v + 1);
            aux = aux->next;
            count ++;
        }
        printf("\n");
    }
}

/*
 *  Function:
 *    freegraph
 *
 *  Description:
 *    Da free da memoria alocada associada ao grafo
 *
 *  Arguments:
 *      -> Grafo a dar free
 *      
 *
 *  Return value:
 *      ---------------
 */
void P1_freegraph(P1_Graph *G) {
    P1_link *aux, *aux2;
    for (int i = 0; i < G->V; i++) {    // Libertar cada lista
        aux = G->adj[i];
        while (aux != NULL) // Libertar cada nó 
        {
            //printf("2\n");
            aux2 = aux->next;
            free (aux);
            aux = aux2;
        }
    }
    free(G->adj);       // Tabela de listas
    free(G->linked);    // Tabela auxiliar
    free(G);
}

/*
 *  Function:
 *    edgeInit
 *
 *  Description:
 *    Cria uma aresta
 *
 *  Arguments:
 *      -> Pointer que vai ter a aresta
 *
 *  Return value:
 *      -> Pointer para aresta
 */
P1_Edge * P1_edgeInit(P1_Edge * e)
{
    P1_Edge * a = malloc(sizeof(P1_Edge));

    if (a == NULL) // Erro de alocacao
        exit(0);
        
    return a;
}

/*
 *  Function:
 *    edgeChange
 *
 *  Description:
 *    Altera o conteúdo da aresta a inserir
 *
 *  Arguments:
 *      -> Aresta
 *      -> Vertice 1
 *      -> Vertice 2
 *      -> Custo da aresta
 *
 *  Return value:
 *      -> Pointer para aresta
 */
P1_Edge * P1_edgeChange(P1_Edge *e,int v1,int v2, double custo)
{
    e->v=v1;
    e->w=v2;
    e->custo=custo;
    return e;
}



void P1_main(int argc, char *argv[])
{
    FILE *fp, *fout;
    char cabecalho[3];
    int V, E, v1, v2, k;
    double temp;
    P1_Graph *G;
    P1_Edge *e = NULL;

    P1_verify(argc, argv);     // Verifica-se erros de invocação

    e = P1_edgeInit(e);        // Inicia-se aresta

    fp = fopen(argv[1], "r");       // Abre-se ficheiro de entrada e saída
    fout = P1_openFile(argv[1]);

    while (!feof(fp))
    {
        k = P1_validateHeader(fp, fout, &v1, &v2, &V, &E, cabecalho, &temp);   //Verificamos se o cabecalho possui um vertice invalido
        if (k==-1)
            break;  // Caso nao consiga ler os vertices do ficheiro
        else if(k==1)
            continue;   // Caso o vertice seja invalido

        if (strcmp(cabecalho, "A0") == 0)       // Se for modo A0
        {
            fprintf(fout, "%d\n\n", P1_f_A0(fp, v1, E));

            continue;
        }  
        else if (strcmp(cabecalho, "B0") == 0)  // Se for modo B0
        {
            temp = P1_f_B0(fp, v1, v2, E);

            if (temp == -1)
                fprintf(fout, "-1\n\n");
            else
                fprintf(fout, "%.2lf\n\n", temp);
            
            continue;
        }

        G = P1_GraphInit(fp, V, E, v1);             // Se for C0 ou D0 temos de ler o grafo para memória para o analisar
        P1_Le_arestas(E, e, G, fp);

        if (strcmp(cabecalho, "C0") == 0)       // Se for C0
            fprintf(fout, "%d\n", P1_f_C0(G, v1));

        else if (strcmp(cabecalho, "D0") == 0)      // Se for D0
            fprintf(fout, "%d\n", P1_f_D0(G, v1));


        fprintf(fout, "\n");
        P1_freegraph(G);
    }

    free(e);
    fclose(fp);
    fclose(fout);

    return;
}