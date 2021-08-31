#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DELETED -2147483648         // INT_MIN for the deleted blanks.

FILE* fin;
FILE* fout;

typedef int ElementType;
typedef ElementType List;
typedef struct HashTbl* HashTable;

typedef struct HashTbl{
    int TableSize;
    List* TheLists;
}HashTbl;

HashTable createTable(int TableSize);
void Insert(HashTable H, ElementType Key, int solution);
void Delete(HashTable H, ElementType Key, int solution);
int Find(HashTable H, ElementType Key, int solution);
void printTable(HashTable H);
void deleteTable(HashTable H);
int IsFull(HashTable H, ElementType Key, int solution);

int main(int argc, char* argv[]){
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    char solution_str[20];
    int solution, TableSize;

    fscanf(fin, "%s", solution_str);
    if(!strcmp(solution_str, "Linear"))
        solution = 1;
    else if(!strcmp(solution_str, "Quadratic"))
        solution = 2;
    else{
        fprintf(fout, "Error: Invalid hashing solution!");
        return 0;
    }

    fscanf(fin, "%d", &TableSize);

    HashTable H = createTable(TableSize);

    char cv;
    int key;
    while(!feof(fin)){
        fscanf(fin, "%c", &cv);
        switch(cv){
            case 'i':
                fscanf(fin, "%d", &key);
                Insert(H, key, solution);
                break;
            case 'f':
                fscanf(fin, "%d", &key);
                int result = Find(H, key, solution);
                if(result)
                    fprintf(fout, "%d is in the table\n", key);
                else
                    fprintf(fout, "%d is not in the table\n", key);
                break;
            case 'd':
                fscanf(fin, "%d", &key);
                Delete(H, key, solution);
                break;
            case 'p':
                printTable(H);
                break;
        }
    }

    deleteTable(H);

    fclose(fin);
    fclose(fout);

    return 0;
}

HashTable createTable(int TableSize){
    HashTbl* h = (HashTbl*)malloc(sizeof(HashTbl));
    h->TableSize = TableSize;
    h->TheLists = (List*)malloc((TableSize)*(sizeof(int)));

    for(int i = 0; i < TableSize; i++){
        h->TheLists[i] = 0;
    }

    return h;
}

void Insert(HashTable H, ElementType Key, int solution){        // What if the quadratic searching cannot find the element? -> it is out of condition.
    int findRtn = Find(H, Key, solution);
    if(findRtn != 0)         // Key already exists in the table.
        fprintf(fout, "Insertion Error: %d already exists at address %d\n", Key, findRtn-Key);
    else if(IsFull(H, Key, solution)){      // Table is full.
        fprintf(fout, "Insertion Error: table is full\n");
    }else{          // Key insertion is possible.
        int idx = Key % H->TableSize;
        if(solution == 1){
            while(1){
                if(H->TheLists[idx] > 0)
                    idx = (idx+1)%(H->TableSize);
                else{
                    H->TheLists[idx] = Key;
                    break;
                }
            }
        }else{
            int base = 1;
            while(1){
                if(H->TheLists[idx] > 0){
                    idx = (idx+(base*base))%(H->TableSize);
                    base++;
                }else{
                    H->TheLists[idx] = Key;
                    break;
                }
            }
        }
        fprintf(fout, "Insert %d into address %d\n", Key, idx);
    }

    return;
}

void Delete(HashTable H, ElementType Key, int solution){
    int findRtn = Find(H, Key, solution);
    if(findRtn == 0)            // Key is not found.
        fprintf(fout, "Deletion Error: %d is not in the table\n", Key);
    else{
        H->TheLists[findRtn-Key] = DELETED;
        fprintf(fout, "Delete %d\n", Key);
    }

    return;
}

int Find(HashTable H, ElementType Key, int solution){
    int idx = Key % H->TableSize, startIdx = idx;

    if(solution == 1){      // Collision solution type: linear
        while(1){
            if(H->TheLists[idx] == Key)
                return Key+idx;
            else
                idx = (idx+1) % H->TableSize;

            if(idx == startIdx)
                break;
        }
    }else{                  // Collision solution type: quadratic
        int base = 1;
        while(1){
            if(H->TheLists[idx] == Key)
                return Key+idx;
            else{
                idx = (idx+base*base) % (H->TableSize);
                base++;
            }

            if(base > H->TableSize)
                break;
        }
    }

    return 0;
}

void printTable(HashTable H){
    for(int i = 0; i < H->TableSize; i++){
        if(H->TheLists[i] == DELETED)
            fprintf(fout, "0 ");
        else
            fprintf(fout, "%d ", H->TheLists[i]);
    }

    return;
}

void deleteTable(HashTable H){
    free(H->TheLists);
    free(H);

    return;
}

int IsFull(HashTable H, ElementType Key, int solution){        // The list is full even though there is a unreachable blank.
    int chck = 1;
    if(solution == 1){          // Collision solution type: linear
        for(int i = 0; i < H->TableSize; i++){
            if(H->TheLists[i] <= 0)
                return 0;
        }
    }else{                      // Collision solution type: quadratic
        int idx = Key % H->TableSize;
        int base = 1; 
        for(int i = 0; i < H->TableSize; i++){
            if(H->TheLists[idx] <= 0)
                return 0;
            else{
                idx = (idx+base*base)%(H->TableSize);
                base++;
            }
        }
    }
}