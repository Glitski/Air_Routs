#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcs.h"

/*
 *  Data Type: node
 *
 *  Description: 
 *  Elemento da lista
 * 
 *  Structure with:
 *      1) Vertice
 *      2) Custo da aresta
 *      3) Ponteiro para o próximo vertice adjacente
 */
struct node {int v; double custo ; link *next;};

/*
 *  Data Type: graph
 *  
 * 
 *  Description: 
 *      Estrutura que representa um grafo geral (no nosso caso representa o backbone)
 * 
 * 
 * Structure with:
 *      1) Numero total de vertices
 *      2) Numero total de arestas
 *      3) Ponteiro para tabela de listas de adjacencias
 *      4) Numero total de conexoes do backbone
 *      5) Custo total do backbone
 */
struct graph{int V; int E; link **adj; int conn; double cusTotal;};

/*
 *  Data Type: edge
 *
 *  Description: 
 *      Estrutura que representa uma aresta
 * 
 * 
 * Structure with:
 *      1) Vertice 1 da aresta
 *      2) Vertice 2 da aresta
 *      3) Custo da aresta
 */
struct edge {int v; int w; double custo;};

/*
 *  Data Type: backbone
 *
 *  Description: 
 *      Estrutura que representa a conectividade no backbone
 * 
 * Structure with:
 *      1) Tabela de id
 *      2) Tabela de size
 */
struct backbone {int *id ; int *sz;};


/*
 *  Function:
 *    funcao_A1
 *
 *  Description:
 *    Funcao que calcula o backbone de um grafo usando o algoritmo de Kruskal
 *
 *  Arguments:
 *      -> Grafo
 *      -> Vetor de arestas
 *      -> Ficheiro de saida
 *      -> flag que indica se tem de dar print de -1 (util para ser reutilizada nos outros modos)
 *
 *  Return value:
 *      --------------
 */
void funcao_A1(Graph * G, Edge **arestas, FILE *fout,int flag)
{
    int i,k;
    int * id=NULL,*sz=NULL,r1,r2;
    
    quicksort((Item *) arestas,0,G->E-1,LessCusto);     // Ordena vetor de arestas por custos

    CWQUinit(G->V,&id,&sz);     // Cria e inicializa as tabelas de conectividade

    for(i=0,k=0;i<G->E && k<G->V-1;i++)     // Até esgotarmos o vetor de arestas ou ate o backbone estar completo
    {
        if(!CWQUfind(arestas[i]->v,arestas[i]->w,id,&r1,&r2))   // Se 2 vertices nao estiverem conectados
        {
            CWQUunion(arestas[i]->v,arestas[i]->w,id,sz,r1,r2); // Queremos conecta-los e adicionar essa aresta ao backbone
            GraphInsertE(G, arestas[i]);

            arestas[i]->custo=-1;       // Indicador que a aresta faz parte do backbone
            k++;
        }
    }
    free(id);
    free(sz);

    if (!flag)      // Modo A1
        fprintf(fout, "%d %.2lf\n", G->conn, G->cusTotal);
    else    // Se o A1 foi chamado por outro modo
        fprintf(fout, "%d %.2lf -1\n", G->conn, G->cusTotal);
    
    printBackbone(G,fout);

    return;

}


/*
 *  Function:
 *    funcao_B1
 *
 *  Description:
 *    Funcao que calcula a aresta substituda de uma que tenha sido eliminada num bacbkone
 *
 *  Arguments:
 *      -> Grafo
 *      -> Vetor de arestas
 *      -> V1 da aresta
 *      -> V2 da aresta
 *      -> ficheiro de saida
 *
 *  Return value:
 *      --------------
 */
void funcao_B1(Graph * G,Edge **arestas, int v1,int v2, FILE *fout)
{
    int i,k, flag = 0;   //flag indica se a aresta a remover pertence ao backbone;
    int * id=NULL,*sz=NULL,r1,r2;
    Backbone * B;

    if (!verifyEdge(arestas, v1, v2, G))        // Caso a aresta seja invalida entao e o modo A1 apenas
    {
        funcao_A1(G,arestas,fout,1);
        return;
    }

    B = BackboneInit(G->V);             // Inicializa as tabelas de conectividade do backbone
    
    quicksort((Item *) arestas,0,G->E-1,LessCusto);


    CWQUinit(G->V,&id,&sz); // Inicializa as tabelas de conectividade que servem para criar o backbone

    for(i=0,k=0;i<G->E && k<G->V-1;i++)
    {
        if(!CWQUfind(arestas[i]->v,arestas[i]->w,id,&r1,&r2))   // Caso os 2 vertices da aresta nao estejam conectados
        {
            CWQUunion(arestas[i]->v,arestas[i]->w,id,sz,r1,r2); // Une-se os 2 e adiciona-se a aresta ao backbone
            GraphInsertE(G, arestas[i]);

            arestas[i]->custo=-1;
            k++;
            
            // A aresta a remover pertence ao backbone original
            if ((arestas[i]->v == v1 && arestas[i]->w == v2) || (arestas[i]->v == v2 && arestas[i]->w == v1)) {
                flag = 1;
                continue;
            }

            CWQUfind(arestas[i]->v,arestas[i]->w,B->id,&r1,&r2);        // Representa-se a conectividade do backbone (excluindo a aresta a remover)
            CWQUunion(arestas[i]->v,arestas[i]->w,B->id,B->sz,r1,r2);
            
        }
    }

    if (flag) {     // Se a aresta a remover pertencer ao backbone vamos descobrir a que susbtitui

        for (i = 0; i < G->E; i++) {    // Vamos percorrer o vetor de arestas e analisar as arestas que nao foram introduzidas no backbone
            if (arestas[i]->custo == -1)    
                continue;
            if(!CWQUfind(arestas[i]->v,arestas[i]->w,B->id,&r1,&r2)) {  // Se esta aresta unir 2 vertices que nao estao conectados, entao e a aresta que substitui a que foi removida
                break;      
            }
        }
        if (i == G->E) {    // Caso tenha percorrido o vetor de arestas sem encontrar a que substitui
            fprintf(fout, "%d %.2lf -1\n", G->conn, G->cusTotal);
            printBackbone(G, fout);
        }
        else {      // Se encontrou
            fprintf(fout, "%d %.2lf 1\n", G->conn, G->cusTotal);
            printBackbone(G, fout);

            if (arestas[i]->v < arestas[i]->w)
                fprintf(fout, "%d %d %.2lf\n", arestas[i]->v + 1, arestas[i]->w + 1, arestas[i]->custo);
            else
                fprintf(fout, "%d %d %.2lf\n", arestas[i]->w + 1, arestas[i]->v + 1, arestas[i]->custo);
        }
        
    }
    else {      // Se não pertencer
        
        fprintf(fout, "%d %.2lf 0\n", G->conn, G->cusTotal);
        printBackbone(G, fout);
    }
    free(id);
    free(sz);
    freeBackbone(B);
    return;
}

/*
 *  Function:
 *    funcao_C1
 *
 *  Description:
 *    Funcao que calcula o backbone e a sua variante quando se retira uma aresta no grafo inicial
 *
 *  Arguments:
 *      -> Grafo
 *      -> Vetor de arestas
 *      -> V1 da aresta
 *      -> V2 da aresta
 *      -> ficheiro de saida
 *
 *  Return value:
 *      --------------
 */

void funcao_C1(Graph * G, Edge ** arestas, FILE *fout,int v1,int v2)
{
    int i,k,j,flag=0;   // flag indica quando foi adicionada a aresta a remover ao backbone 
    int * id=NULL,*sz=NULL,r1,r2;
    Backbone * B=NULL;
    Graph * G2=NULL;

    if (!verifyEdge(arestas, v1, v2, G))     // Caso a aresta seja invalida entao e o modo A1 apenas
    {
        funcao_A1(G,arestas,fout,1);
        return;
    }
   
    quicksort((Item *) arestas,0,G->E-1,LessCusto); 
  

    CWQUinit(G->V,&id,&sz); 

    B = BackboneInit(G->V);     // Inicializa as tabelas de conectividade do backbone


    G2 = GraphInit(G->V,G->E);  // Cria-se outro grafo que representa o backbone variante
   
        

    for(i=0,k=0,j=0;i<G->E && k<G->V-1;i++)
    {
        if (j < G->V-1) //Backbone completo (ja não é necessario testar se as arestas ligam dois vertices previamente desconectados)

            if(!CWQUfind(arestas[i]->v,arestas[i]->w,id,&r1,&r2)) 
            {
                j++;    // Conta as arestas inseridas no backbone original

                CWQUunion(arestas[i]->v,arestas[i]->w,id,sz,r1,r2);   // Construcao do backbone original
                GraphInsertE(G, arestas[i]);

                // Quando inserirmos a aresta a remover
                if((arestas[i]->v == v1 && arestas[i]->w == v2) || (arestas[i]->v == v2 && arestas[i]->w == v1))
                {
                    flag=1;
                    continue;
                }

                if(!flag)       // Enquanto ainda nao foi inserida a aresta a remover, entao os 2 backbones sao iguais (raizes iguais)
                {
                    CWQUunion(arestas[i]->v,arestas[i]->w,B->id,B->sz,r1,r2);
                    GraphInsertE(G2,arestas[i]);
                    k++;
                }
            }
        
        if(flag)        // A partir do momento em que os backbones diferem, entao passa a ser necessario fazer o find do backbone variante (isto é, ver se 2 vertices estao ligados)
        {
            if(!CWQUfind(arestas[i]->v,arestas[i]->w,B->id,&r1,&r2))
            {
                CWQUunion(arestas[i]->v,arestas[i]->w,B->id,B->sz,r1,r2);
                GraphInsertE(G2, arestas[i]);            
                k++;    // Conta as arestas inseridas no backbone variante        
            } 
        }
    }

    free(id);
    free(sz);
    fprintf(fout, "%d %.2lf ", G->conn, G->cusTotal);

    
    if(!flag)
        fprintf(fout, "-1\n");
    else
        fprintf(fout, "%d %.2lf\n", G2->conn, G2->cusTotal);

    printBackbone(G,fout);

    if(flag)
        printBackbone(G2,fout);


    freegraph(G2);
    freeBackbone(B);

    return;
}

/*
 *  Function:
 *    funcao_D1
 *
 *  Description:
 *    Funcao que calcula o backbone e as rotas alternativas quando se retira um vertice
 *
 *  Arguments:
 *      -> Grafo
 *      -> Vetor de arestas
 *      -> Vertice retirado
 *      -> ficheiro de saida
 *
 *  Return value:
 *      --------------
 */
void funcao_D1(Graph * G, Edge **arestas, int v, FILE *fout)
{
    int i,k;
    int * id=NULL,*sz=NULL,r1,r2, count = 0, count2 = 0;
    Backbone * B;
    Edge **e;

    if(!(v>=0 && v<=G->V-1))    // Se for um vertice invalido entao e semelhante ao modo A1
    {
        funcao_A1(G,arestas,fout,1);
        return;
    }
    B = BackboneInit(G->V);
    
    quicksort((Item *) arestas,0,G->E-1,LessCusto);
        

    CWQUinit(G->V,&id,&sz);

    for(i=0,k=0;i<G->E && k<G->V-1;i++)     // Calcula-se o backbone de forma semelhante ao A1
    {
        if(!CWQUfind(arestas[i]->v,arestas[i]->w,id,&r1,&r2))
        {
            CWQUunion(arestas[i]->v,arestas[i]->w,id,sz,r1,r2);
            GraphInsertE(G, arestas[i]);

            arestas[i]->custo=-1;
            k++;

            if (arestas[i]->v == v || arestas[i]->w == v) {
                count ++;       // Contamos quantas arestas que possuem o vertice a retirar fazem parte do backbone
                continue;
            }

            CWQUfind(arestas[i]->v,arestas[i]->w,B->id,&r1,&r2);         // Representa-se a conectividade do backbone (excluindo as arestas a remover)
            CWQUunion(arestas[i]->v,arestas[i]->w,B->id,B->sz,r1,r2);
        }
    }

    e =(Edge **) malloc(count * sizeof(Edge *));    // Guarda as arestas substitutas (valor maximo count = arestas que foram removidas)

    if (e == NULL) // Erro de alocacao
      exit(0);

    for (int i = 0; i < G->E && count2 < count; i++) {

        if(arestas[i]->custo==-1)   // Aresta no backbone
            continue;

        if (arestas[i]->v == v || arestas[i]->w == v)   // Aresta que possui o vertice a remover
            continue;

        if(!CWQUfind(arestas[i]->v,arestas[i]->w,B->id,&r1,&r2)) {  // Se a aresta unir 2 vertices que nao estao conectados, entao substitui umas das removidas

            CWQUunion(arestas[i]->v,arestas[i]->w,B->id,B->sz,r1,r2);

            e[count2] = (Edge *) malloc(sizeof(Edge));  // Count 2 -> numero de arestas encontradas que subsituem

            if (e[count2] == NULL) // Erro de alocacao
                exit(0);

            if(arestas[i]->v < arestas[i]->w){  // Ordem das arestas
                e[count2]->v = arestas[i]->v;
                e[count2]->w = arestas[i]->w;
            }
            else{
                e[count2]->v = arestas[i]->w;
                e[count2]->w = arestas[i]->v;
            }

            e[count2++]->custo = arestas[i]->custo;
        }
    }
    fprintf (fout, "%d %.2lf %d\n", G->conn, G->cusTotal, count2);
    printBackbone(G, fout);

    quicksort((Item *) e,0,count2-1,LessAresta);    // Ordenar o vetor de arestas pela ordem desejada para dar print

    for (int i = 0; i < count2; i++) {
        fprintf(fout, "%d %d %.2lf\n", e[i]->v + 1, e[i]->w + 1, e[i]->custo);
    }
    freeArestas(e,count2);
    free(id);
    free(sz);
    freeBackbone(B);
    return;
}

/*
 *  Function:
 *    funcao_E1
 *
 *  Description:
 *    Funcao que calcula o backbone e a rota substituta para cada aresta no mesmo
 *
 *  Arguments:
 *      -> Grafo
 *      -> Vetor de arestas
 *      -> ficheiro de saida
 *
 *  Return value:
 *      --------------
 */
void funcao_E1(Graph * G, Edge **arestas, FILE *fout)
{
    int i,k,j;
    int * id=NULL,*sz=NULL,r1,r2;
    link *aux;
    Backbone * B;

    B = (Backbone *) malloc(sizeof(Backbone));  // Aloca-se diretamente o backbone para evitar inicializar os vetores da conectividade (uma vez que vai ser feito mais adiante pela funcao ResetConnect)

    if (B == NULL) // Erro de alocacao
        exit(0);

    B->id= (int *) malloc(G->V*sizeof(int));

    if (B->id == NULL) // Erro de alocacao
        exit(0);

    B->sz= (int *) malloc(G->V*sizeof(int));

    if (B->sz == NULL) // Erro de alocacao
        exit(0);


    quicksort((Item *) arestas,0,G->E-1,LessCusto);

    CWQUinit(G->V,&id,&sz);

    for(i=0,k=0;i<G->E && k<G->V-1;i++)     // Calcular o backbone como se fosse em A1
    {
        if(!CWQUfind(arestas[i]->v,arestas[i]->w,id,&r1,&r2))
        {
            CWQUunion(arestas[i]->v,arestas[i]->w,id,sz,r1,r2);
            GraphInsertE(G, arestas[i]);

            arestas[i]->custo=-1;
            k++;
        }
    }
    fprintf(fout, "%d %.2lf\n", G->conn, G->cusTotal);
    
    for (i = 0; i < G->V; i++) {    // Para cada aresta do backbone
        aux = G->adj[i];
        while (aux != NULL) {
            fprintf(fout, "%d %d %.2lf ", i + 1, aux->v + 1, aux->custo);

            ResetConnect(B,G->V);   // Reiniciamos a conectividade do backbone 
            InitConect(B,G,i,aux->v);   //e voltamos a defini-la excluindo a aresta que queremos remover

            for (j = 0; j < G->E; j++) {    // Depois vamos procurar no vetor de arestas

                if (arestas[j]->custo == -1)
                    continue;

                if(!CWQUfind(arestas[j]->v,arestas[j]->w,B->id,&r1,&r2))    // Se encontrarmos uma aresta que une 2 vertices que nao estao conectados entao e essa a aresta que substitui
                    break;
            }

            if (j == G->E)  // Caso tenhamos percorrido o vetor todo (nao ha aresta substituta)
            {
                fprintf(fout,"-1\n");
            }
            else
            {
                if(arestas[j]->v < arestas[j]->w)
                        fprintf(fout, "%d %d %.2lf\n", arestas[j]->v + 1, arestas[j]->w + 1, arestas[j]->custo);
                    else     
                        fprintf(fout, "%d %d %.2lf\n", arestas[j]->w + 1, arestas[j]->v + 1, arestas[j]->custo);
            }

            aux = aux->next;
        }
    }

    free(id);
    free(sz);
    freeBackbone(B);

    return;

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

void verify(int argc, char *argv[])
{
    if (argc != 2)      // Caso nao tenhamos so 2 argumentos
    {
        //printf("Invalid number of agruments\n");
        exit(0);
    }
    if (strcmp(strrchr(argv[1], '.'), ".routes") != 0)     // Caso a extensão não seja .routes
    {
        //printf("Invalid format (should be *.routes)\n");
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
FILE *openFile(char *filename)
{
    char *word;
    FILE *fp;

    filename[strlen(filename) - strlen(".routes")] = '\0'; // Para o strcpy so copiar até este \0 (ignorando a extensão)

    word = (char *)malloc((strlen(filename) + strlen(".bbones") + 1) * sizeof(char));

    if (word == NULL) // Erro de alocacao
        exit(0);

    if (word == NULL)
    {
        fprintf(stderr, "ERROR: allocation of output filename. Not enough memory.\n");
        exit(0);
    }

    strcpy(word, filename);     // Cria-se o ficheiro de saida com a extensão correta
    strcat(word, ".bbones");
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
 *      
 *  
 *
 *  Return value:
 *      -> Int que vai dizer se o main tem de dar break ou continue
 */
int validateHeader(FILE *fp, FILE *fout, int *v1, int *v2, int *V, int *E, char *cabecalho)
{    
    if (fscanf(fp, "%d %d %s", V, E, cabecalho) != 3)       // Caso nao consiga ler o cabecalho e porque o ficheiro acabou
        return -1;

    fprintf(fout, "%d %d %s ", *V, *E, cabecalho);      //Output

    if ((strcmp(cabecalho, "B1") == 0)||(strcmp(cabecalho, "C1") == 0))       // Se for modo B1 ou C1 quermemos ler 2 vertices para o problema
    {
        
        if (fscanf(fp, "%d %d", v1, v2) != 2)
            exit(0);

       
        fprintf(fout, "%d %d ", *v1, *v2);
        (*v2)--;
        (*v1)--;
    }
    else if(strcmp(cabecalho, "D1") == 0)    // Se for outro modo apenas queremos ler 1
    {
        if (fscanf(fp, "%d", v1) != 1)
            exit(0);
            
        
        fprintf(fout, "%d ", *v1);  // Output

        (*v1)--;
    }

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
 *      -> Ficheiro de entrada
 *  
 *
 *  Return value:
 *      Vetor de arestas
 */
Edge ** Le_arestas(int E, FILE * fp)
{
    int i, v1, v2;
    double temp;
    Edge ** arestas;
    arestas = (Edge **) malloc(E*(sizeof(Edge *)));

    if (arestas == NULL) // Erro de alocacao
        exit(0);

    for (i = 0; i < E; i++)        // Le todas as arestas
    {
        if (fscanf(fp, "%d %d %lf", &v1, &v2, &temp) != 3)
            exit(0);
        v1--;
        v2--;
        arestas[i] = (Edge *) malloc(sizeof(Edge ));

        if (arestas[i] == NULL) // Erro de alocacao
            exit(0);

        arestas[i]->v = v1;
        arestas[i]->w = v2;
        arestas[i]->custo = temp;

    
    }
    return arestas;
}

/*
 *  Function:
 *    InitConect
 *
 *  Description:
 *    Representa a conectividade de um backbone ignorando uma aresta
 *
 *  Arguments:
 *      -> Backbone
 *      -> Grafo
 *      -> V1 da aresta
 *      -> V2 da aresta
 *  
 *
 *  Return value:
 *    ---------------
 */
void InitConect(Backbone * B,Graph * G, int v1,int v2)
{
    link * aux;
    int i;
    int r1,r2;

    for(i=0;i<G->V;i++)     // Para cada aresta do backbone
    {
        aux=G->adj[i];
        while(aux!=NULL)
        {
            if((i == v1 && aux->v == v2) || (i == v2 && aux->v == v1))  // Se for a aresta que queremos ignorar, passamos a frente
            {
                aux =aux->next;
                continue;
            }
               
            
            if(!CWQUfind(i,aux->v,B->id,&r1,&r2))   // Representamos a sua conectividade
                CWQUunion(i,aux->v,B->id,B->sz,r1,r2);

            aux =aux->next;
        }
    }   

    return;
}

/*
 *  Function:
 *    ResetConnect
 *
 *  Description:
 *    Reinicia o vetor da conectividade do backbone
 *
 *  Arguments:
 *      -> Backbone
 *      -> Numero total de vertices
 *  
 *
 *  Return value:
 *    ---------------
 */
void ResetConnect(Backbone * B, int V)
{
    for(int i=0;i<V;i++)
    {
        B->id[i]=i;
        B->sz[i]=1;
    }
}
