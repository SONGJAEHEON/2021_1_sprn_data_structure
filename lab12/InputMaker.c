#include <stdio.h>
#include <stdlib.h>
#include <time.h>

FILE* fout;

int main(int argc, char** args){
    srand((unsigned int)time(NULL));

    int input = atoi(args[1]);
    fout = fopen(args[2], "w");

    fprintf(fout, "%d\n", input);

    int cnt = 0, from, to, dist;
    while(1){
        from = rand() % input;
        to = rand() % (input-from)+from+1;
        dist = rand() % (3*input);
        fprintf(fout, "%d-%d-%d", from, to, dist);
        if(cnt < (input*input))
            fprintf(fout, " ");
        else
            break;
        cnt++;
    }
    fprintf(fout, "\n");

    fclose(fout);

    return 0;
}