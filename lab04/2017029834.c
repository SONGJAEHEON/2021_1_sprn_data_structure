#include <stdio.h>
#include <stdlib.h>

FILE* fin;
FILE* fout;

typedef struct ThreadedTree* ThreadedPtr;
typedef int ElementType;

typedef struct ThreadedTree{
    int left_thread;    // Flag if ptr is thread.
    ThreadedPtr left_child;
    ElementType data;
    ThreadedPtr right_child;
    int right_thread;   // Flag if ptr is thread.
}ThreadedTree;

ThreadedPtr CreateTree();
int Insert(ThreadedPtr root, int root_idx, ElementType data, int idx);      // Return 0 in error cases.
void printInorder(ThreadedPtr root);
void DeleteTree(ThreadedPtr root);

int main(int argc, char* argv[]){
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    ThreadedPtr root = CreateTree();

    int NUM_NODES;
    fscanf(fin, "%d", &NUM_NODES);

    int root_idx = 0, idx = 0;

    while(++idx <= NUM_NODES){
        ElementType data;
        fscanf(fin, "%d", &data);

        if(Insert(root, root_idx, data, idx) == 0){     // Error case.
            return 0;
        }
    }

    printInorder(root);
    DeleteTree(root);

    fclose(fin);
    fclose(fout);

    return 0;
}

ThreadedPtr CreateTree(){
    ThreadedPtr tree = NULL;
    tree = (ThreadedPtr)malloc(sizeof(ThreadedTree));       // Dynamic allocation.
    if(tree == NULL){
        fprintf(fout, "Out of Space!\n");
        return NULL;
    }
    tree->data = -1;        // Root node characteristics.
    tree->left_thread = 1;
    tree->left_child = tree;
    tree->right_thread = 1;
    tree->right_child = tree;

    return tree;
}

int Insert(ThreadedPtr root, int root_idx, ElementType data, int idx){      // Return 0 in error cases.
    if(root == NULL){
        printf("Handed tree is NULL in Insert\n");
        
        return 0;
    }
    ThreadedPtr tmpPtr = root;
    int tmp = 0, idxtmp = idx;

    ThreadedPtr newNode = (ThreadedPtr)malloc(sizeof(ThreadedTree));        // Dynamic allocation.
    if(newNode == NULL){
        fprintf(fout, "Out of Space!\n");

        return 0;       // Error.
    }
    newNode->data = data;
    
    while(tmp < idxtmp/2){      // Find the location to be inserted.
        while(idxtmp > 2*tmp+1){
            idxtmp /= 2;
        }
        if(idxtmp == 2*tmp){
            tmpPtr = tmpPtr->left_child;
            tmp = 2*tmp;
        }
        else{
            tmpPtr = tmpPtr->right_child;
            tmp = 2*tmp+1;
        }
        idxtmp = idx;
    }

    if(idxtmp == 2*tmp){        // Left child insertion.
        newNode->left_thread = 1;
        newNode->left_child = tmpPtr->left_child;
        newNode->right_thread = 1;
        newNode->right_child = tmpPtr;
        tmpPtr->left_thread = 0;
        tmpPtr->left_child = newNode;
    }else{                      // Right child insertion.
        newNode->left_thread = 1;
        newNode->left_child = tmpPtr;
        newNode->right_thread = 1;
        newNode->right_child = tmpPtr->right_child;
        tmpPtr->right_thread = 0;
        tmpPtr->right_child = newNode;
    }

    return 1;       // No error.
}

void printInorder(ThreadedPtr root){
    ThreadedPtr tmp = root, tmp2 = NULL;
    
    if(root == NULL){       // NULL tree case.
        return;
    }else{                  // Not NULL case.
        tmp = tmp->right_child;
        while(!tmp->left_thread){       // Move to the leftmost node.
            tmp = tmp->left_child;
        }
    }

    fprintf(fout, "Inorder Traversal :");
    while(1){
        fprintf(fout, " %d", tmp->data);
        tmp2 = tmp;
        tmp = tmp->right_child;

        if(!tmp2->right_thread){        // Condition whether move to the leftmost node or not.
            while(!tmp->left_thread){
                tmp2 = tmp;
                tmp = tmp->left_child;
            }
        }
        
        if(tmp == root) break;      // After the traversal, finish.
    }

    return;
}

void DeleteTree(ThreadedPtr root){      // Free the tree recursively.
    if(root == NULL) return;

    if(!root->left_thread)
        DeleteTree(root->left_child);
    if(!root->right_thread)
        DeleteTree(root->right_child);

    free(root);
    
    return;
}
