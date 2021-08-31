#include <stdio.h>
#include <stdlib.h>
#include <time.h>

FILE* fout;

int IsExists(int* array, int size, int random);
void Add(int* array, int random);

int main(int argc, char* args[]){
    srand((unsigned int)time(NULL));

    fout = fopen(args[1], "w");
    int input = atoi(args[2]);

    fprintf(fout, "%d\n", input);

    int* arr = (int*)malloc((input)*sizeof(int));
    for(int i = 0; i < input; i++){
        arr[i] = -1;
    }

    int random;
    for(int i = 0; i < input; i++){
        //      Codes for blocking duplicated insertion.
        // do{
        //     random = rand() % 100;
        // }while(IsExists(arr, input, random));
        random = rand() % 100;
        Add(arr, random);
        fprintf(fout, "%d ", random);
    }
    fprintf(fout, "\n");

    return 0;
}

int IsExists(int* arr, int size, int rand){
    int chck = 0, i = 0;
    while(arr[i] >= 0 && i < size){
        if(arr[i] == rand){
            chck = 1;
            break;
        }
        i++;
    }

    return chck;
}

void Add(int* arr, int rand){
    int i = 0;
    while(arr[i] >= 0)
        i++;
    arr[i] = rand;

    return;
}