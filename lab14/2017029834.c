#include <stdio.h>
#include <stdlib.h>

FILE* fin;
FILE* fout;

typedef struct Array Array;

struct Array{
    int size;
    int* values;
};

Array* CreateArray(int size);
void QuickSort(Array* array, int left, int right);
int Partition(Array* array, int left, int right);
void Swap(Array* array, int left, int right);
void PrintArray(Array* array, int left, int right);
void DeleteArray(Array* array);

int main(int argc, char* argv[]){
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    int size, i;
    Array* array;

    fscanf(fin, "%d", &size);
    array = CreateArray(size);

    for(i = 0; i < size; i++){
        fscanf(fin, "%d", &array->values[i]);
    }

    QuickSort(array, 0, size-1);
    fprintf(fout, "\nSorting Result: \n");
    PrintArray(array, 0, size-1);

    DeleteArray(array);

    fclose(fin);
    fclose(fout);

    return 0;
}

Array* CreateArray(int size){
    Array* arr = (Array*)malloc(sizeof(Array));
    arr->size = size;
    arr->values = (int*)malloc((size)*(sizeof(int)));

    return arr;
}

void QuickSort(Array* array, int left, int right){
    if(left < right){
        int pivot = Partition(array, left, right);
        QuickSort(array, left, pivot-1);
        QuickSort(array, pivot+1, right);
    }

    return;
}

int Partition(Array* array, int left, int right){
    int i = left, j = right-1, pivot = array->values[right], rtn;
    int swapCnt = 0;
    fprintf(fout, "Left:%3d, Right:%3d, Pivot:%3d\n", left, right, pivot);
    PrintArray(array, left, right);
    while(1){
        while(array->values[i] < pivot && i <= right-1)     // '<' for the better result.
            i++;
        while (array->values[j] >= pivot && j >= left)
            j--;

        if(i < j){
            Swap(array, i, j);
            swapCnt++;
            PrintArray(array, left, right);
        }else{              // Edge cases.
            if(swapCnt > 0){
                Swap(array, i, right);
                PrintArray(array, left, right);
                rtn = i;
            }else if(j < left){
                Swap(array, left, right);
                PrintArray(array, left, right);
                rtn = left;
            }else if(j == left){
                Swap(array, i, right);
                PrintArray(array, left, right);
                rtn = i;
            }else if(i == right-1){
                Swap(array, right-1, right);
                PrintArray(array, left, right);
                rtn = right-1;
            }else{          // 'i == right' with no swapCnt after rotation.
                rtn = right;
            }
            break;
        }

    }

    return rtn;
}

void Swap(Array* arr, int l, int r){        // Swap the elements.
    int tmp = arr->values[l];
    arr->values[l] = arr->values[r];
    arr->values[r] = tmp;

    return;
}

void PrintArray(Array* array, int left, int right){         // Print the array as the form.
    char* c = "-";
    for(int i = 0; i < array->size; i++){
        if(left <= i && i <= right)
            fprintf(fout, "%-3d", array->values[i]);
        else
            fprintf(fout, "%-3s", c);
    }
    fprintf(fout, "\n");

    return;
}

void DeleteArray(Array* array){         // Free the array.
    free(array->values);
    free(array);

    return;
}