//Rohit Kulkarni, 5/30/19, rnkulkar, pa5
//Graph.c, uses List.c to create the Graph ADT, the main method is BFS.

#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"




#define WHITE 0
#define BLACK 1
#define GRAY  2


struct GraphObj {
  List *adjArray;
  int *color;
  int *parent;
  int *discover;
  int *finish;
  int order; 
  int size; 
  int source; 
};




Graph newGraph(int n){ 
  Graph G = malloc(sizeof(struct GraphObj)); 
  G->adjArray = calloc (n + 1, sizeof(List)); 
  G->color = calloc(n + 1, sizeof(int)); 
  G->parent = calloc(n + 1, sizeof(int)); 
  G->discover = calloc(n+1, sizeof(int));
  G->finish = calloc(n + 1, sizeof(int));
  G->source = NIL;
  G->order = n; 
  G->size = 0; 
  for (int i = 1; i < n + 1; i++){
    G->adjArray[i] = newList();
    G->color[i] = WHITE;
    G->parent[i] = NIL; 
  }
  return G;
} 
 
void freeGraph(Graph* pG){
  Graph G = *pG; 
  for(int i = 0; i < (G->order) + 1; i++){
    freeList(&(G->adjArray[i]));
  }
  free(G->adjArray); 
  free(G->color); 
  free(G->parent); 
  free(*pG);
  *pG = NULL; 
} 

 

/*** Access functions ***/

int getOrder(Graph G){
  return G->order;
}

int getSize(Graph G){
  return G->size;
}

int getSource(Graph G){
  return G->source;
}

int getParent(Graph G, int u){
  if(u < 1 || u > getOrder(G)){
    printf("getParent: invalid u parameter");
    return 0;
  }
  return G->parent[u];
}



int getDiscover(Graph G, int u)
{
    if(u < 1 || u > getOrder(G)){
        printf("getDiscover: invalid u parameter");
        return 0;
    }
    return G->discover[u];
}

int getFinish(Graph G, int u)
{
    if(u < 1 || u > getOrder(G)){
        printf("getFinish: invalid u parameter");
        return 0;
    }
    return G->finish[u];
}



/*** Manipulation procedures ***/ 


static void sortedInsert(List L, int u){
  if (length(L) == 0){
    append(L, u);
    return;
  }
  for(moveFront(L); index(L) >= 0; moveNext(L)){
    int n = get(L);
    if(u < n){
      insertBefore(L, u);
      break;
    }
  }
  if(index(L) == -1){
    append(L, u);
  }
}



void addEdge(Graph G, int u, int v){
  if(u < 1 || u > getOrder(G) || v < 1 || v > getOrder(G))
  {
        printf("Verticies are not defined on Graph\n");
        return;
  }
  sortedInsert(G->adjArray[v], u);
  sortedInsert(G->adjArray[u], v);
  
  G->size++;
}



 

void addArc(Graph G, int u, int v){
  if(u < 1 || u > getOrder(G) || v < 1 || v > getOrder(G)){
    printf("invalid verticies as input");
  }
  List U = G->adjArray[u];
  sortedInsert(U, v);
  G->size++;
}
 

void Visit(Graph G, List Q, int vertex, int* t)
{
    G->discover[vertex] = ++(*t);
    G->color[vertex] = GRAY;

    moveFront(G->adjArray[vertex]);
    while(index(G->adjArray[vertex]) >= 0)
    {
        int n = get(G->adjArray[vertex]);
        if(G->color[n] == WHITE){
            G->parent[n] = vertex;
            Visit(G, Q, n, t);
        }
        moveNext(G->adjArray[vertex]);
    }
    G->color[vertex] = BLACK;
    G->finish[vertex] = ++(*t);   
    append(Q, vertex);
}

void DFS(Graph G, List S){
  if(length(S) != getOrder(G))
  {
      printf ("invalid S parameter as input");
  }
  int v;
    for(v=1; v<=getOrder(G); v++){
        G->color[v] = WHITE;
        G->parent[v] = NIL;
    }
    List Q = newList();
    int i, time = 0;
    moveFront(S);
    //prepend(S, -1);
    for(i = 1; i <= getOrder(G); i++){
        // reusing var vertex from previous loop.
        int vertex = get(S);
        if(G->color[vertex] == WHITE){
            Visit(G, Q, vertex, &time);
        }
        moveNext(S); 
    }
    clear(S);
    append(Q, -1);
    moveFront(Q);
    while(get(Q) != -1){
        prepend(S, get(Q));
        moveNext(Q);
    }
    freeList(&Q);  
} 

/*** Other operations ***/ 

Graph transpose(Graph G){
    int n = getOrder(G);
    Graph trans = newGraph(n);
    int i;   // vertex
    for(i = 1; i <= n; i++)
    {
        moveFront(G->adjArray[i]);
        while(index(G->adjArray[i]) >= 0)
        {
            int n = get(G->adjArray[i]);
            addArc(trans, n, i);
            moveNext(G->adjArray[i]);
        }
    }
    return trans;
}

Graph copyGraph(Graph G){
    int n = getOrder(G);
    Graph G_copy = newGraph(n);
    int i;   // vertex
    for(i = 1; i <= n; i++){
        moveFront(G->adjArray[i]);
        while(index(G->adjArray[i]) >= 0)
        {
            int n = get(G->adjArray[i]);
            addArc(G_copy, i, n);
            moveNext(G->adjArray[i]);
        }
    }
    return G_copy;
}

void printGraph(FILE* out, Graph G){
  if(out == NULL || G == NULL)
  {
    printf("Passed in NUll parameters");
    return;
  }

  for (int i = 1; i < getOrder(G) + 1; i++)
  {
    fprintf(out, "%d: ", i);
    printList(out, G->adjArray[i]);
    fprintf(out, "\n");
  }
}
