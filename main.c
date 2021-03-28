#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcs.h"
#include "Parte1.h"


int main(int argc, char *argv[])
{   
    if (argc ==2){

        if (strcmp(strrchr(argv[1], '.'), ".routes0") == 0)     // Se for a primeira parte do projeto
        {
            P1_main(argc,argv);
            return 0;
        }
    }
    
    FILE *fin, *fout;
    char cabecalho[3];
    int V, E, v1, v2, k;
    Graph *G;
    Edge **arestas = NULL;
    

    verify(argc, argv);     // Verifica-se erros de invocação


    fin = fopen(argv[1], "r");       // Abre-se ficheiro de entrada e saída
    fout = openFile(argv[1]);

    while (!feof(fin))
    {
        k = validateHeader(fin, fout, &v1, &v2, &V, &E, cabecalho);   //Verificamos se o cabecalho possui um vertice invalido
        if (k==-1)
            break;  // Caso nao consiga ler os vertices do ficheiro

        G = GraphInit(V, E);            
        arestas = Le_arestas(E,fin);

        if(strcmp(cabecalho,"A1")==0)       // Funcoes que realizam os diferentes modos
        {
            funcao_A1(G, arestas,fout,0);    
        }
        else if(strcmp(cabecalho,"B1")==0)
        {
            funcao_B1(G,arestas,v1,v2,fout);
        }
        else if(strcmp(cabecalho,"C1")==0)
        {
            funcao_C1(G, arestas,fout,v1,v2);
        }
        else if(strcmp(cabecalho,"D1")==0)
        {
            funcao_D1(G,arestas,v1,fout);
        }
        else if(strcmp(cabecalho,"E1")==0)
        {
            funcao_E1(G,arestas,fout);
        }
       

        fprintf(fout, "\n");
        freeArestas(arestas,E);
        freegraph(G);
        
        
    }
    fclose(fin);
    fclose(fout);
    

    return 0;
}