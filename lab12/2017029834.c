// InputMaker.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* fin;
FILE* fout;

typedef struct Node{
    int vertex;
    int dist;
    int prev;
}Node;

typedef struct Graph{
    int size;
    int** vertices;
    Node* nodes;
}Graph;

typedef struct Heap{
    int Capacity;
    int Size;
    Node* Element;
}Heap;

Graph* createGraph(int size);
void deleteGraph(Graph* g);
void printShortestPath(Graph* g);       // Run dijkstra's algorithm.
void PrintRoute(Graph* g, int vertex);  // Print the path.
Heap* createMinHeap(int heapSize);
void deleteMinHeap(Heap* minHeap);
int RtnChild(Heap* minHeap, int idx);   // Return the child index to be moved.
void Swap(Heap* minHeap, int a, int b); // Swap the nodes.
void insertToMinHeap(Heap* minHeap, int vertex, int distance, int prev);
Node deleteMin(Heap* minHeap);

void main(int argc, char* argv[]){
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");
    Graph* g;
    int size;
    fscanf(fin, "%d\n", &size);
    g = createGraph(size+1);

    char tmp = 0;
    while(tmp != '\n' && tmp != EOF){
        int node1, node2, weight;
        fscanf(fin, "%d-%d-%d", &node1, &node2, &weight);
        g->vertices[node1][node2] = weight;
        tmp = fgetc(fin);
    }
    printShortestPath(g);

    deleteGraph(g);

    fclose(fin);
    fclose(fout);

    return;
}

Graph* createGraph(int size){
    Graph* g = (Graph*)malloc(sizeof(Graph));
    g->size = size-1;
    g->vertices = (int**)malloc((size)*(sizeof(int*)));
    for(int i = 0; i < size; i++)
        g->vertices[i]  = (int*)malloc((size)*(sizeof(int)));
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            g->vertices[i][j] = 0;
        }
    }
    g->nodes = (Node*)malloc((size)*(sizeof(Node)));
    for(int i = 0; i < size; i++){
        g->nodes[i].vertex = i;
        g->nodes[i].dist = __INT_MAX__;         // Initial distance is the INT_MAX for the comparison.
        g->nodes[i].prev = 0;
    }

    return g;
}

void deleteGraph(Graph* g){
    free(g->nodes);
    for(int i = 0; i < g->size; i++)
        free(g->vertices[i]);

    free(g->vertices);
    free(g);

    return;
}

void printShortestPath(Graph* g){
    Heap* minHeap = createMinHeap((g->size)*(g->size));     // Heap size with (number of edge)^2.

    // Handle outgoing edges form the node 1 first.
    g->nodes[1].vertex = 1;
    g->nodes[1].dist = 0;
    g->nodes[1].prev = 1;
    for(int i = 2; i <= g->size; i++){
        if(g->vertices[1][i] != 0){
            insertToMinHeap(minHeap, i, g->vertices[1][i], 1);
            g->vertices[1][i] = 0;
        }
    }

    // Handle rest of the nodes.
    Node tmp;
    int cnt = 0;
    while(cnt < g->size*g->size){
        tmp = deleteMin(minHeap);
        if(tmp.vertex == 0)
            break;
        if(g->nodes[tmp.prev].dist + tmp.dist < g->nodes[tmp.vertex].dist){
            g->nodes[tmp.vertex].dist = g->nodes[tmp.prev].dist + tmp.dist;
            g->nodes[tmp.vertex].prev = tmp.prev;
        }
        for(int i = 1; i <= g->size; i++){
            if(g->vertices[tmp.vertex][i] != 0){
                insertToMinHeap(minHeap, i, g->vertices[tmp.vertex][i], tmp.vertex);
                g->vertices[tmp.vertex][i] = 0;
            }
        }
        cnt++;
    }

    // Print the paths.
    for(int i = 2; i <= g->size; i++){
        if(g->nodes[i].dist == __INT_MAX__){
            fprintf(fout, "Cannot reach to node %d.\n", i);
        }else{
            PrintRoute(g, i);       // Use a recursive function.
            fprintf(fout, " (cost: %d)\n", g->nodes[i].dist);
        }
    }

    return;
}

void PrintRoute(Graph* g, int vertex){      // Recursive fuction.
    if(vertex == 1){
        fprintf(fout, "1");
    }else{
        PrintRoute(g, g->nodes[vertex].prev);
        fprintf(fout, "->%d", vertex);
    }

    return;
}

Heap* createMinHeap(int heapSize){
    Heap* heap = (Heap*)malloc(sizeof(Heap));
    heap->Capacity = heapSize+1;
    heap->Size = 0;
    heap->Element = (Node*)malloc((heap->Capacity)*(sizeof(Node)));

    return heap;
}

void deleteMinHeap(Heap* minHeap){
    free(minHeap->Element);
    free(minHeap);

    return;
}

void insertToMinHeap(Heap* minHeap, int vertex, int distance, int prev){  // prev는?
    int tmp = ++minHeap->Size;
    while(tmp > 1 && distance < minHeap->Element[tmp/2].dist){
        minHeap->Element[tmp].vertex = minHeap->Element[tmp/2].vertex;
        minHeap->Element[tmp].dist = minHeap->Element[tmp/2].dist;
        minHeap->Element[tmp].prev = minHeap->Element[tmp/2].prev;
        tmp /= 2;
    }
    minHeap->Element[tmp].vertex = vertex;
    minHeap->Element[tmp].dist = distance;
    minHeap->Element[tmp].prev = prev;

    return;
}

Node deleteMin(Heap* minHeap){
    Node node, tmp;
    
    if(minHeap->Size == 0){     // Empty heap case.
        node.vertex = 0;
        node.dist = 0;
        node.prev = 0;
    }else{                      // Return the node with 'index 1' and heapify the heap.
        node.vertex = minHeap->Element[1].vertex;
        node.dist = minHeap->Element[1].dist;
        node.prev = minHeap->Element[1].prev;

        minHeap->Element[1].vertex = minHeap->Element[minHeap->Size].vertex;
        minHeap->Element[1].dist = minHeap->Element[minHeap->Size].dist;
        minHeap->Element[1].prev = minHeap->Element[minHeap->Size].prev;
        minHeap->Size--;
        
        int tmp = 1, chng;
        while(2*tmp <= minHeap->Size){
            chng = RtnChild(minHeap, tmp);
            if(minHeap->Element[tmp].dist > minHeap->Element[chng].dist){
                Swap(minHeap, tmp, chng);
            }else{
                break;
            }
            tmp = chng;
        }
    }

    return node;
}

int RtnChild(Heap* minHeap, int idx){
    if(2*idx == minHeap->Capacity){
        return 2*idx;
    }else{
        if(minHeap->Element[2*idx].dist < minHeap->Element[2*idx+1].dist){
            return 2*idx; 
        }else{
            return 2*idx+1;
        }
    }
}

void Swap(Heap* minHeap, int a, int b){
    Node tmp;
    tmp.vertex = minHeap->Element[a].vertex;
    tmp.dist = minHeap->Element[a].dist;
    tmp.prev = minHeap->Element[a].prev;
    minHeap->Element[a].vertex = minHeap->Element[b].vertex;
    minHeap->Element[a].dist = minHeap->Element[b].dist;
    minHeap->Element[a].prev = minHeap->Element[b].prev;
    minHeap->Element[b].vertex = tmp.vertex;
    minHeap->Element[b].dist = tmp.dist;
    minHeap->Element[b].prev = tmp.prev;

    return;
}