#include <stdio.h>
#include <stdlib.h>
#include <time.h>

FILE* fin;
FILE* fout;

typedef struct _DisjointSet{
    int size_maze;
    int* ptr_arr;
}DisjointSets;

void init(DisjointSets* sets, DisjointSets* maze_print, int num);
void Union(DisjointSets* sets, DisjointSets* maze_print, int i, int j);
int find(DisjointSets* sets, int i);
void createMaze(DisjointSets* sets, DisjointSets* maze_print, int num);
void printMaze(DisjointSets* maze_print, int num);
void freeMaze(DisjointSets* sets, DisjointSets* maze_print);
int isFar(DisjointSets* sets, int num1, int num2);          // Check whether two nodes are adjacent to each other or not.
int rotateAll(DisjointSets* sets);          // Check whether all nodes' root is '1' or not.

int main(int argc, char* argv[]){
    srand((unsigned int)time(NULL));

    int num;
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    DisjointSets *sets, *maze_print;
    fscanf(fin, "%d", &num);

    sets = (DisjointSets*)malloc(sizeof(DisjointSets));
    maze_print = (DisjointSets*)malloc(sizeof(DisjointSets));

    init(sets, maze_print, num);
    createMaze(sets, maze_print, num);
    printMaze(maze_print, num);
    freeMaze(sets, maze_print);

    fclose(fin);
    fclose(fout);

    return 0;
}

void init(DisjointSets* sets, DisjointSets* maze_print, int num){
    sets->ptr_arr = (int*)malloc(sizeof(int)*(num*num+1));
    sets->size_maze = num;
    maze_print->ptr_arr = (int*)malloc(sizeof(int)*(2*num*(2*num-1)));
    maze_print->size_maze = num;

    // Arrays initiation.
    for(int i = 0; i < (num*num+1); i++)
        sets->ptr_arr[i] = 0;
    for(int i = 0; i < (2*num*(2*num-1)); i++)
        maze_print->ptr_arr[i] = 1;

    return;
}

void Union(DisjointSets* sets, DisjointSets* maze_print, int i, int j){
    // Change the parent node.
    if(find(sets, i) == 1)
        sets->ptr_arr[find(sets, j)] = i;
    else if(find(sets, j) == 1)
        sets->ptr_arr[find(sets, i)] = j;
    else
        sets->ptr_arr[find(sets, i)] = j;

    // Remove the wall.
    if(i == j+1)
        maze_print->ptr_arr[(j/maze_print->size_maze)*(4*maze_print->size_maze)+(2*(j%maze_print->size_maze))] = 0;
    else if(j == i+1)
        maze_print->ptr_arr[(i/maze_print->size_maze)*(4*maze_print->size_maze)+(2*(i%maze_print->size_maze))] = 0;
    else if(i == j+sets->size_maze)
        maze_print->ptr_arr[((j-1)/maze_print->size_maze)*(4*maze_print->size_maze)+(2*maze_print->size_maze)+(2*((j-1)%maze_print->size_maze+1)-1)] = 0;
    else      // j == i+sets->size_maze case.
        maze_print->ptr_arr[((i-1)/maze_print->size_maze)*(4*maze_print->size_maze)+(2*maze_print->size_maze)+(2*((i-1)%maze_print->size_maze+1)-1)] = 0;

    return;
}

int find(DisjointSets* sets, int i){        // Find the root node.
    while(sets->ptr_arr[i] > 0)
        i = sets->ptr_arr[i];

    return i;
}

void createMaze(DisjointSets* sets, DisjointSets* maze_print, int num){
    int blck1 = 0, blck2 = 0;
    while(!rotateAll(sets)){
        // Find 2 blocks to be unioned.
        while(isFar(sets, blck1, blck2) || find(sets, blck1)==find(sets, blck2)){    // Short circuit evaluation.
            blck1 = (rand())%(num*num)+1;
            blck2 = (rand())%(num*num)+1;
        }
        Union(sets, maze_print, blck1, blck2);
    }

    return;
}

void printMaze(DisjointSets* maze_print, int num){
    // Upper boundary.
    for(int i = 0; i < num; i++){
       fprintf(fout, "+---");
    }
    fprintf(fout, "+\n");
    // Inside the boundary.
    for(int i = 1; i < 2*num*(2*num-1)-1; i++){
        // 1st column of each row.
        if(i%(2*num) == 1){
            if(i == 1)
                fprintf(fout, " ");
            else if(i/(2*num)%2 == 0)
                fprintf(fout, "|");
            else
                fprintf(fout, "+");
        }
        if(i%(2*num) == 0){
            // Last column of each row.
            if((i/(2*num))%2 == 1)
                fprintf(fout, "|\n");
            else
                fprintf(fout, "+\n");
        }else{
            if((i/(2*num))%2 == 0){     // Rows consist of blocks.
                if(i%2 == 1)
                    fprintf(fout, "   ");      // 3 blacks.
                else{
                    if(maze_print->ptr_arr[i] == 1)
                        fprintf(fout, "|");
                    else
                        fprintf(fout, " ");
                }
            }else{      // Rows consist of lines.
                if(i%2 == 0)
                    fprintf(fout, "+");
                else{
                    if(maze_print->ptr_arr[i] == 1)
                        fprintf(fout, "---");
                    else
                        fprintf(fout, "   ");
                }
            }
        }
    }
    fprintf(fout, " \n");

    // Below boundary.
    for(int i = 0; i < num; i++){
       fprintf(fout, "+---");
    }
    fprintf(fout, "+\n");

    return;
}

void freeMaze(DisjointSets* sets, DisjointSets* maze_print){
    int* tmp;

    // Free sets.
    tmp = sets->ptr_arr;
    sets->ptr_arr = NULL;
    free(tmp);
    free(sets);

    // Free maze_print.
    tmp = maze_print->ptr_arr;
    maze_print->ptr_arr = NULL;
    free(tmp);
    free(maze_print);

    return;
}

int isFar(DisjointSets* sets, int num1, int num2){      // Check whether two nodes are adjacent to each other or not.
    if((num2 == num1+1) && (num1%(sets->size_maze)!=0))       return 0;
    else if(num2 == num1+(sets->size_maze))      return 0;
    else if((num1 == num2+1) && (num2%(sets->size_maze)!=0))  return 0;
    else if(num1 == num2+(sets->size_maze))      return 0;
    else    return 1;
}

int rotateAll(DisjointSets* sets){          // Check whether all nodes' root is '1' or not.
    for(int i = 1; i <= (sets->size_maze * sets->size_maze); i++){
        if(find(sets, i) != 1)  return 0;
    }

    return 1;
}