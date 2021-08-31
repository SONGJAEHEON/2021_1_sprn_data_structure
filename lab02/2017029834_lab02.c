#include <stdio.h>
#include <stdlib.h>

typedef struct Node * PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;
typedef int ElementType;

struct Node{
    ElementType element;
    Position next;
};

List MakeEmpty(List L);
int IsEmpty(List L);
int IsLast(Position P, List L);
void Insert(ElementType X, List L, Position P);
void PrintList(List L);
void Delete(ElementType X, List L);
Position Find(ElementType X, List L);
Position FindPrevious(ElementType X, List L);
void DeleteList(List L);

FILE * input;
FILE * output;

int main(int argc, char * argv[]){
    char command;
    int key1, key2;
    FILE * input;
    Position header = NULL;
    Position tmp = NULL;
    if(argc == 1){
        input = fopen("input.txt", "r");
        output = fopen("output.txt", "w");
    }else{
        input = fopen(argv[1], "r");
        output = fopen(argv[2], "w");
    }

    header = MakeEmpty(header);
    while(1){
        command = fgetc(input);
        if(feof(input)) break;
        switch(command){
            case 'i':
                fscanf(input, "%d %d", &key1, &key2);
                //duplication check
                tmp = Find(key1, header);
                if(tmp != NULL){
                    fprintf(output, "There already is an element with key %d. Insertion failed\n", key1);
                    break;
                }
                tmp = Find(key2, header);
                Insert(key1, header, tmp);
                break;
            case 'd':
                fscanf(input, "%d", &key1);
                Delete(key1, header);
                break;
            case 'f':
                fscanf(input, "%d", &key1);
                tmp = FindPrevious(key1, header);
                if(IsLast(tmp, header))
                    fprintf(output, "Finding(%d) Failed: Element %d is not in the list\n", key1, key1);
                else{
                    if(tmp->element > 0)    //if tmp is not the header
                        fprintf(output, "Key of the previous node of %d is %d\n", key1, tmp->element);
                    else                    //if tmp is the header(because the element of header is -1)
                        fprintf(output, "Key of the previous node of %d is head\n", key1);
                }
                break;
            case 'p':
                PrintList(header);
                break;
            default:
                break;
        }
    }
    DeleteList(header);
    fclose(input);
    fclose(output);
    return 0;
}

List MakeEmpty(List L){     // initialize the List
    L = (List)malloc(sizeof(struct Node));
    L->element = -1;        // header's element : -1
    L->next = NULL;

    return L;
}

int IsEmpty(List L){        // empty : return 1 / else :return 0
    if(L == NULL){          // NULL pointer error
        printf("Handed List is NULL in IsEmpty\n");
        exit(-1);
    }

    return L->next == NULL;
}

int IsLast(Position P, List L){     // LastNode : return 1 / else : return 0
    if(P == NULL){
        printf("Handed List is NULL in IsLast\n");
        exit(-1);
    }

    return P->next == NULL;
}

void Insert(ElementType X, List L, Position P){     // insert the Node which contains the element X right after the P Node
    if(L == NULL){      // NULL pointer error
        printf("Handed List is NULL in Insert\n");
        exit(-1);
    }
    if(P == NULL){      // NULL pointer error
        fprintf(output, "Insertion(%d) Failed: cannot find the location to be inserted\n", X);

        return;
    }
    PtrToNode newNode = (PtrToNode)malloc(sizeof(struct Node)); // dynamic allocation
    newNode->element = X;
    newNode->next = P->next;
    P->next = newNode;

    return;
}

void PrintList(List L){
    if(L == NULL){
        printf("Handed List is NULL in PrintList\n");
        exit(-1);
    }
    if(IsEmpty(L)){                         // if the list is empty
        fprintf(output, "Empty List!\n");

        return;
    }else{
        Position tmp = L->next;
        fprintf(output, "key: %d", tmp->element);
        while(tmp->next != NULL){
            tmp = tmp->next;
            fprintf(output, "\tkey: %d", tmp->element);    // tab and space
        }
    }
    fprintf(output, "\n");

    return;
}

void Delete(ElementType X, List L){
    if(L == NULL){      // NULL pointer error
        printf("Handed List is NULL in Delete\n");
        exit(-1);
    }
    Position p, tmp;
    p = Find(X, L);     // X exists in L : return the position / X doesn't exists in L : return NULL
    if(p == NULL){      // if X doesn't exists in L
        fprintf(output, "Deletion(%d) Failed: Element %d is not in the list\n", X, X);

        return;
    }
    if(!IsLast(p, L)){       // if X is not the last Node
        p = FindPrevious(X, L);
        tmp = p->next;
        p->next = tmp->next;
        tmp->next = NULL;
        free(tmp);
    }else{                  // if X is the last Node
        free(p);
    }

    return;
}

Position Find(ElementType X, List L){       // X exists in L : return the position / X doesn't exists in L : return NULL
    if(L == NULL){      // NULL pointer error
        printf("Handed List is NULL in Find\n");
        exit(-1);
    }
    if(X == -1){
        return L;
    }else{
        Position p = L, tmp = NULL;
        if(!IsEmpty(L)){
            do{
                p = p->next;
                if(X == p->element){
                    tmp = p;
                    break;
                }
            }while(p->next != NULL);
        }

        return tmp;
    }
}

Position FindPrevious(ElementType X, List L){       // list is empty : return the head / cannot find the X : return the last node / finding success : return the previoud node
    if(L == NULL){
        printf("Handed List is NULL in FindPrevious\n");
        exit(-1);
    }
    Position p = L;
    if(!IsEmpty(L)){        // if the list is not empty
        do{
            if(X == p->next->element)
                return p;
            p = p->next;
        }while(p->next != NULL);
    }

    return p;
}

void DeleteList(List L){
    if(L == NULL){
        printf("Handed List is NULL in DeleteList\n");
        exit(-1);
    }
    Position tmp;
    do{             // free the node one by one
        tmp = L->next;
        L->next = NULL;
        free(L);
        L = tmp;
    }while(L != NULL);

    return;
}
