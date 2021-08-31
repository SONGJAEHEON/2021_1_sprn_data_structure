#include <stdio.h>
#include <stdlib.h>

FILE* fin;
FILE* fout;

#define MAX(a, b) (((a) >= (b)) ? (a) : (b))

struct AVLNode;
typedef struct AVLNode* Position;
typedef struct AVLNode* AVLTree;
typedef int ElementType;

typedef struct AVLNode{
    ElementType element;
    AVLTree left, right;
    int height;
}AVLNode;

AVLTree Insert(ElementType X, AVLTree T);
AVLTree Delete(ElementType X, AVLTree T);
Position SingleRotateWithLeft(Position node);
Position SingleRotateWithRight(Position node);
Position DoubleRotateWithLeft(Position node);
Position DoubleRotateWithRight(Position node);
void PrintInorder(AVLTree T);
void DeleteTree(AVLTree T);
int CalcHeight(Position P);     // Calculate the height of the P node and its descendants.
int GetHeight(Position P);      // Return the height of the P node.
int FindLeftmost(Position P);

int main(int argc, char* argv[]){
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");
    AVLTree Tree = NULL;
    char cv;
    int key;

    while(!feof(fin)){
        fscanf(fin, "%c", &cv);
        switch(cv){
            case 'i':
                fscanf(fin, "%d\n", &key);
                Tree = Insert(key, Tree);
                break;
            case 'd':
                fscanf(fin, "%d\n", &key);
                Tree = Delete(key, Tree);
                Tree->height = CalcHeight(Tree);
                break;
        }
        PrintInorder(Tree);
        fprintf(fout, "\n");
    }

    DeleteTree(Tree);
    fclose(fin);
    fclose(fout);

    return 0;
}

AVLTree Insert(ElementType X, AVLTree T){
    if(T == NULL){          // Node insertion at the right spot.
        AVLNode* newNode = (AVLNode*)malloc(sizeof(AVLNode));
        if(newNode == NULL){            // Dynamic allocation error.
            fprintf(fout, "Memory is full!\n");
        }
        newNode->element = X;
        newNode->left = NULL;
        newNode->right = NULL;
        newNode->height = 0;
        T = newNode;
    }else if(X < T->element){
        T->left = Insert(X, T->left);
        T->height = CalcHeight(T);
        if(GetHeight(T->left) - GetHeight(T->right) >= 2){
            if(X < T->left->element)
                T = SingleRotateWithLeft(T);        // LL case.
            else
                T = DoubleRotateWithLeft(T);       // LR case.
        }
    }else if(X > T->element){
        T->right = Insert(X, T->right);
        T->height = CalcHeight(T);
        if(GetHeight(T->right) - GetHeight(T->left) >= 2){
            if(X > T->right->element)
                T = SingleRotateWithRight(T);       // RR case.
            else
                T = DoubleRotateWithRight(T);        // RL case.
        }
    }else      // Overlapped case.
        fprintf(fout, "Insertion Error: %d is already in the tree!\n", X);
    T->height = CalcHeight(T);  // Update the height of the node and its child nodes.

    return T;
}

AVLTree Delete(ElementType X, AVLTree T){
    if(T == NULL){           // Cannot find the node.
        fprintf(fout, "Deletion Error: %d is not in the tree!\n", X);

        return T;
    }else if(X < T->element){
        T->left = Delete(X, T->left);
    }else if(X > T->element){
        T->right = Delete(X, T->right);
    }else{
        if(T->left == NULL && T->right == NULL){
            free(T);

            return NULL;
        }else if(T->left == NULL){
            Position tmp = T->right;
            T->right = NULL;
            free(T);

            return tmp;
        }else if(T->right == NULL){
            Position tmp = T->left;
            T->left = NULL;
            free(T);

            return tmp;
        }else{
            T->element = FindLeftmost(T->right);
            T->right = Delete(T->element, T->right);

            return T;
        }
    }
    T->height = CalcHeight(T);
    if(CalcHeight(T->left) - CalcHeight(T->right) >= 2){
        if(GetHeight(T->left->left) >= GetHeight(T->left->right))               // LL case.
            T = SingleRotateWithLeft(T);
        else                            // LR case.
            T = DoubleRotateWithLeft(T);
    }else if(CalcHeight(T->right) - CalcHeight(T->left) >= 2){
        if(GetHeight(T->right->right) >= GetHeight(T->right->left))             // RR case.
            T = SingleRotateWithRight(T);
        else                            // RL case.
            T = DoubleRotateWithRight(T);
    }

    return T;
}

Position SingleRotateWithLeft(Position node){       // LL case.
    Position tmp1 = node, tmp2 = node->left;
    tmp1->left = tmp2->right;
    tmp2->right = tmp1;
    tmp1->height = CalcHeight(tmp1);
    tmp2->height = CalcHeight(tmp2);

    return tmp2;
}

Position SingleRotateWithRight(Position node){      // RR case.
    Position tmp1 = node, tmp2 = node->right;
    tmp1->right = tmp2->left;
    tmp2->left = tmp1;
    tmp1->height = CalcHeight(tmp1);
    tmp2->height = CalcHeight(tmp2);

    return tmp2;
}

Position DoubleRotateWithLeft(Position node){       // LR case.
    node->left = SingleRotateWithRight(node->left);
    
    return SingleRotateWithLeft(node);
}

Position DoubleRotateWithRight(Position node){      // RL case.
    node->right = SingleRotateWithLeft(node->right);

    return SingleRotateWithRight(node);
}

void PrintInorder(AVLTree T){       // Print the tree with inorder traversal.
    if(T != NULL){
        if(T->left)
            PrintInorder(T->left);
        fprintf(fout, "%d(%d) ", T->element, T->height);
        if(T->right)
            PrintInorder(T->right);
    }

    return;
}

void DeleteTree(AVLTree T){
    if(T != NULL){
        DeleteTree(T->left);
        DeleteTree(T->right);
        free(T);
    }

    return;
}

int CalcHeight(Position p){          // Rearrange the descendible nodes' height.
    if(p == NULL)       // Height of the NULL node is -1;
        return -1;
    else if(p->left == NULL && p->right == NULL)
        return 0;
    else if(p->left == NULL){
        p->height = CalcHeight(p->right)+1;

        return p->height;
    }
    else if(p->right == NULL){
        p->height = CalcHeight(p->left)+1;

        return p->height;
    }
    else{
        p->height = MAX(CalcHeight(p->left), CalcHeight(p->right))+1;

        return p->height;
    }
}

int GetHeight(Position P){        // Return the height of the P node.
    if(P == NULL)
        return -1;
    else
        return P->height;
}

int FindLeftmost(Position P){       // Return the leftmost node's element.
    while(P->left){
        P = P->left;
    }

    return P->element;
}
