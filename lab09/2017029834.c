#include <stdio.h>
#include <stdlib.h>
#define INF 1e9

FILE* fin;
FILE* fout;

typedef struct HeapStruct{
    int Capacity;
    int Size;
    int* Elements;
}Heap;

Heap* CreateHeap(int heapSize);
void Insert(Heap* heap, int value);
int Find(Heap* heap, int value);
int DeleteMax(Heap* heap);
void PrintHeap(Heap* heap);
int IsFull(Heap* heap);
int getBiggerChild(Heap* heap, int index);

int main(int argc, char* argv[]){
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");  

    char cv;
    Heap* maxHeap;
    int heapSize, key, max_element;

    while(!feof(fin)){
        fscanf(fin, "%c", &cv);
        switch(cv){
            case 'n':
                fscanf(fin, "%d", &heapSize);
                maxHeap = CreateHeap(heapSize);
                break;
            case 'i':
                fscanf(fin, "%d", &key);
                Insert(maxHeap, key);
                break;
            case 'd':
                max_element = DeleteMax(maxHeap);
                if(max_element != -INF){
                    fprintf(fout, "Max Element: %d deleted\n", max_element);
                }
                break;
            case 'p':
                PrintHeap(maxHeap);
                break;
            case 'f':
                fscanf(fin, "%d", &key);
                if(Find(maxHeap, key)) fprintf(fout, "%d is in the heap\n", key);
                else fprintf(fout, "Finding Error: %d is not in the heap\n", key);
                break;
        }
    }

    return 0;
}

Heap* CreateHeap(int heapSize){
    Heap* heap = malloc(sizeof(Heap));
    heap->Elements = (int*)malloc((heapSize+1) * sizeof(Heap));
    heap->Size = heapSize;
    heap->Capacity = 0;

    return heap;
}

void Insert(Heap* heap, int value){     // Priority: isFull > duplicate > insert.
    if(IsFull(heap)){
        fprintf(fout, "Insert Error: heap is full\n");
    }else if(Find(heap, value)){
        fprintf(fout, "Insert Error: %d is already in the heap\n", value);
    }else{
        int tmp = ++heap->Capacity;
        while(tmp > 1 && heap->Elements[tmp/2] < value)         // Find the location to be inserted.
        {
            heap->Elements[tmp] = heap->Elements[tmp/2];
            tmp /= 2;
        }
        heap->Elements[tmp] = value;
        fprintf(fout, "insert %d\n", value);
    }

    return;
}

int Find(Heap* heap, int value){            // Return 1 if the value exists.
    for(int i = 1; i <= heap->Capacity; i++){
        if(heap->Elements[i] == value)
            return 1;
    }

    return 0;
}

int DeleteMax(Heap* heap){
    if(heap->Capacity == 0){
        fprintf(fout, "Delete Error: heap is empty\n");

        return -INF;
    }
    int max = heap->Elements[1], tmpIndex = 1, tmpChildIndex = getBiggerChild(heap, tmpIndex);
    while(heap->Elements[heap->Capacity] < heap->Elements[tmpChildIndex] && tmpChildIndex != 0){        // Rearrange the array.
        heap->Elements[tmpIndex] = heap->Elements[tmpChildIndex];
        tmpIndex = tmpChildIndex;
        tmpChildIndex = getBiggerChild(heap, tmpIndex);
    }
    heap->Elements[tmpIndex] = heap->Elements[heap->Capacity--];

    return max;
}

void PrintHeap(Heap* heap){
    int tmp = heap->Capacity;
    if(tmp == 0){           // Heap is empty.
        fprintf(fout, "Print Error: heap is empty\n");
    }else{
        while(tmp--)
            fprintf(fout, "%d ", heap->Elements[heap->Capacity-tmp]);
        fprintf(fout, "\n");
    }

    return;
}

int IsFull(Heap* heap){         // Return 1 if the heap is full.
    return heap->Size == heap->Capacity;
}

int getBiggerChild(Heap* heap, int index){      // Return the adequate child node index.
    if(index*2 > heap->Capacity){               // If the parent has no child.
        return 0;
    }else if(index*2+1 > heap->Capacity){       // If the parent has only left child.
        return index*2;
    }else{
        if(heap->Elements[index*2] > heap->Elements[index*2+1]){
            return index*2;
        }else{
            return index*2+1;
        }
    }
}