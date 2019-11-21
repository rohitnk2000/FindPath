//Rohit Kulkarni, pa5, 6/7/19, rnkulkar
//Uses Graph ADT to take input file, read in a directed graph, run DFS on this graph, 
//and print out all of the strongly connected components of the graph.

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"List.h"
#include"Graph.h"
#define MAX_LEN 255


int main(int argc, char* argv[]) {
	int n = 0;
	FILE *in, *out;
	char line[MAX_LEN];

	//checks to see if two files are chosen as arguemnts
	if(argc != 3) {
		fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
		exit(1);
	}

	in = fopen(argv[1], "r");
	out = fopen(argv[2], "w");

	if(in == NULL) {
		fprintf(stderr, "Unable to open file %s for reading\n", argv[1]);
		exit(1);
	}
	if(out == NULL) {
		fprintf(stderr, "Unable to open file %s for writingn\n", argv[2]);
		exit(1);
	}

	fgets(line, MAX_LEN, in);
	sscanf(line, "%d", &n);


	List verticies = newList();
	for(int i = 1; i <= n; i++){
		append(verticies, i);
	}

	Graph G = newGraph(n);

	while(fgets(line, MAX_LEN, in) != NULL){
		int x = 0;
		int y = 0;
		sscanf(line, "%d %d", &x, &y);
		if (x == 0 && y == 0){
			break;
		}
		addArc(G, x, y);
	}

	fprintf(out, "Adjacency list representation of G:\n");
	printGraph(out, G);
	fprintf(out, "\n");

	DFS(G, verticies);

	Graph T = transpose(G);
	DFS(T, verticies);

	int numSCC = 0;
	for(int i = 1; i <= getOrder(T); i++){
		if(getParent(T, i) == NIL) numSCC++;
	}

	fprintf(out, "G contains %d strongly connected components:\n", numSCC);

	List *L = calloc(numSCC + 1, sizeof(List));
	for(int i = 1; i <= numSCC; i++) {
		L[i] = newList();
	}
	int count = 1;
	int listLength = length(verticies);
	int vertex = front(verticies);
	for(int j = 1; j <= listLength - 1; j++) {
		int tempVertex = back(verticies);
		deleteBack(verticies);
		prepend(L[count], tempVertex);
		if (getParent(T, tempVertex) == NIL){
			count++;
		}
	}
	prepend(L[count], vertex);
int x = 1;
	while( x <= numSCC) {
		fprintf(out, "Component %d: ", x);
		printList(out, L[x]); fprintf(out, "\n");
		x++;
	}

	freeGraph(&G);
	freeGraph(&T);
	for(count = 0; count <= numSCC; count++) {
		freeList(&L[count]);
	}
	freeList(&verticies);
	free(L);
	fclose(in); fclose(out);
	return(0);
}
