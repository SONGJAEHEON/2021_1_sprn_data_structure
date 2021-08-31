#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INT_MAX 2147483647
#define INT_MIN -2147483648

FILE* fin;
FILE* fout;

typedef struct _Queue{
    int* key;
    int first, rear, qsize, max_queue_size;
}Queue;

typedef struct _Graph{
    int size;
    int* node;
    int** matrix;
}Graph;

Graph* CreateGraph(int* nodes, int n);
void InsertEdge(Graph* G, int a, int b);
void PrintGraph(Graph* G);      // In matrix form.
void DeleteGraph(Graph* G);
void Topsort(Graph* G);         // Use the topological sort.
int IsInPermanent(int* permanent, int size, int X);     // Check whether the vertex is in the permanent list or not.
Queue* MakeNewQueue(int X);
int IsEmpty(Queue* Q);          // Return 1 if the queue is empty.
int Dequeue(Queue* Q);
void Enqueue(Queue* Q, int X);
void DeleteQueue(Queue* Q, int X);
void countInput(int* n, char* str);     // Count the number of nodes.
void parseInput(int* arr, char* str, int n);      // Parse the input.

int main(int argc, char* argv[]){
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    char numbers[100], inputs[150];
    fgets(numbers, 100, fin);
    int n =0;
    countInput(&n, numbers);
    int* nodes = (int*)malloc(sizeof(int)*n);
    parseInput(nodes, numbers, n);

    Graph* G = CreateGraph(nodes, n);

    fgets(inputs, 100, fin);
    int len = strlen(inputs), i, j;
    for(i = 0; i < len; i += 4){
        int a = inputs[i] - '0', b = inputs[i+2] - '0';
        InsertEdge(G, a, b);
    }

    PrintGraph(G);
    Topsort(G);
    DeleteGraph(G);

    fclose(fin);
    fclose(fout);

    return 0;
}

Graph* CreateGraph(int* nodes, int n){
    Graph* g = (Graph*)malloc(sizeof(Graph));
    g->size = n;
    g->node = (int*)malloc(n*sizeof(int));
    g->matrix = (int**)malloc(n*sizeof(int*));      // Dynamic allocation of 2D array.
    for(int i = 0; i < n; i++){
        g->matrix[i] = (int*)malloc(n*sizeof(int*));
    }
    
    int min, minidx, tmp;
    for(int i = 0; i < n; i++){     // Sort the vertexes.
        min = INT_MAX;
        for(int j = i; j < n; j++){
            if(nodes[j] < min){
                min = nodes[j];
                minidx = j;
            }
        }
        tmp = nodes[i];
        nodes[i] = min;
        nodes[minidx] = tmp;
    }

    for(int i = 0; i < n; i++){
        g->node[i] = nodes[i];
    }

    return g;
}

void InsertEdge(Graph* G, int a, int b){
    int aidx, bidx;
    for(int i = 0; i < G->size; i++){
        if(G->node[i] == a){
            aidx = i;
            break;
        }
    }
    for(int i = 0; i < G->size; i++){
        if(G->node[i] == b){
            bidx = i;
            break;
        }
    }
    G->matrix[aidx][bidx] = 1;

    return;
}

void PrintGraph(Graph* G){
    // Print the first line.
    fprintf(fout, " ");
    for(int i = 0; i < G->size; i++)
        fprintf(fout, "  %d", G->node[i]);
    fprintf(fout, "\n");
    
    // Print from the second line.
    for(int i = 0; i < G->size; i++){
        fprintf(fout, "%d", G->node[i]);
        for(int j = 0; j < G->size; j++){
            fprintf(fout, "  %d", G->matrix[i][j]);
        }
        fprintf(fout, "\n");
    }
    fprintf(fout, "\n");

    return;
}

void DeleteGraph(Graph* G){
    for(int i = 0; i < G->size; i++){
        free(G->matrix[i]);
    }
    free(G->matrix);
    free(G->node);
    free(G);

    return;
}

void Topsort(Graph* G){
    Queue* q = MakeNewQueue(G->size);
    
    // 2 methods to find the cycle. (1: No End. 2: Terminal vertex is in the permanent list.)
    int i, j, cnt = 0, permanentidx = 0, permanentcnt = 0, dq;
    int* permanent = (int*)malloc((G->size)*(sizeof(int)));
    for(int k = 0; k < G->size; k++){
        permanent[k] = INT_MIN;
    }
    while(permanentcnt != G->size){         // Rotate the matrix 1 time to find the vertex which has the indegree '0'.
        if(permanent[G->size-1] == INT_MIN){
            for(i = 0; i < G->size; i++){
                for(j = 0; j < G->size; j++){
                    if(G->matrix[j][i] == 1)
                        cnt++;
                }
                if(cnt == 0){
                    if(!IsInPermanent(permanent, G->size, G->node[i])){
                        Enqueue(q, i);
                        permanent[permanentidx++] = G->node[i];
                    }
                }
                cnt = 0;
            }
        }
        dq = Dequeue(q);        // Dequeue return '-1' if the queue is empty.
        if(dq == INT_MIN){
            fprintf(fout, "Sorting Error: Cycle!\n");
            return;
        }
        permanentcnt++;
        for(int k = 0; k < G->size; k++){
            if(G->matrix[dq][k] == 1){
                if(IsInPermanent(permanent, G->size, G->node[k])){       // Cycle case 2.
                    fprintf(fout, "Sorting Error: Cycle!\n");
                    return;
                }else
                    G->matrix[dq][k] = 0;
            }
        }
    }
    for(int k = 0; k < G->size; k++){
        fprintf(fout, "%d ", permanent[k]);
    }
    fprintf(fout, "\n");

    return;
}

int IsInPermanent(int* permanent, int size, int X){
    for(int i = 0; i < size; i++){
        if(permanent[i] == X)
            return 1;
    }
    
    return 0;
}

Queue* MakeNewQueue(int X){
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->key = (int*)malloc(X*sizeof(int));
    q->first = 0, q->rear = -1, q->qsize = 0, q->max_queue_size = X;

    return q;
}

int IsEmpty(Queue* Q){
    if(Q->qsize == 0)
        return 1;
    else
        return 0;
}

int Dequeue(Queue* Q){
    if(IsEmpty(Q))
        return INT_MIN;
    int rtn = Q->key[Q->first];
    Q->first = (Q->first+1)%(Q->max_queue_size);
    Q->qsize--;

    return rtn;
}

void Enqueue(Queue* Q, int X){
    if(Q->qsize != Q->max_queue_size){
        Q->rear = (Q->rear+1)%(Q->max_queue_size);
        Q->key[Q->rear] = X;
        Q->qsize++;
    }

    return;
}

void DeleteQueue(Queue* Q, int X){
    free(Q->key);
    free(Q);

    return;
}

void countInput(int* n, char* str){
    int len = strlen(str), i;
    for(i = 0; i < len; i++){
        if(0 <= str[i] - '0' && str[i] - '0' < 10)
            (*n)++;
    }

    return;
}

void parseInput(int* arr, char* str, int n){
    int len = strlen(str), i;
    int cnt = 0;
    for(i = 0; i < len; i++){
        if(0 <= str[i] - '0' && str[i] - '0' < 10)
            arr[cnt++] = str[i] - '0';
    }

    return;
}