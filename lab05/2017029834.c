#include <stdio.h>
#include <stdlib.h>

FILE* fin;
FILE* fout;

typedef struct BST* Tree;
typedef struct BST{
    int value;
    struct BST* left;
    struct BST* right;
}BST;

Tree insertNode(Tree root, int key);
Tree deleteNode(Tree root, int key);
int findNode(Tree root, int key);
void printInorder(Tree root, int check);
void deleteTree(Tree root);
Tree findMin(Tree root);

void main(int argc, char* argv[]){
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");
    char cv;
    int key, check = 1;

    Tree root = NULL;
    while(!feof(fin)){
        fscanf(fin, "%c", &cv);
        switch(cv){
            case 'i':
                fscanf(fin, "%d", &key);
                root = insertNode(root, key);
                break;
            case 'f':
                fscanf(fin, "%d", &key);
                if(findNode(root, key))       // Key is found.
                    fprintf(fout, "%d is in the tree\n", key);
                else                          // Key is not found.
                    fprintf(fout, "Finding Error: %d is not in the tree\n", key);
                break;
            case 'd':
                fscanf(fin, "%d", &key);
                if(deleteNode(root, key))
                    fprintf(fout, "delete %d\n", key);
                else
                    fprintf(fout, "Deletion Error: %d is not in the tree\n", key);
                break;
            case 'p':
                fscanf(fin, "%c", &cv);
                if(cv == 'i'){
                    if(root == NULL)
                        fprintf(fout, "Tree is empty");
                    else
                        printInorder(root, check);
                }
                fprintf(fout, "\n");
                break;
        }
    }
    deleteTree(root);

    fclose (fin);
    fclose (fout);
}

Tree insertNode(Tree root, int key){
    Tree newNode = (Tree)malloc(sizeof(BST));
    if(newNode == NULL){                    // Dynamic allocation error.
        printf("Dynamic allocation error\n");

        return root;
    }
    newNode->value = key;
    newNode->left = NULL;
    newNode->right = NULL;
    if(root == NULL){       // Empty tree.
        root = newNode;
    }else{
        Tree tmp = root;
        while(1){           // Find the location to be inserted.
            if(tmp->value > key){
                if(!tmp->left) break;
                tmp = tmp->left;
            }else if(tmp->value < key){
                if(!tmp->right) break;
                tmp = tmp->right;
            }else{      // Overlapped case.
                fprintf(fout, "Insertion Error: %d is already in the tree\n", key);
                
                return root;
            }
        }
        if(tmp->value > key)
            tmp->left = newNode;
        if(tmp->value < key)
            tmp->right = newNode;
    }
    fprintf(fout, "insert %d\n", key);

    return root;
}

Tree deleteNode(Tree root, int key){
    Tree tmp = NULL;
    if(root == NULL){               // Cannot find the key within the tree.
        return NULL;
    }else if(root->value > key){
        root = deleteNode(root->left, key);
    }else if(root->value < key){
        root = deleteNode(root->right, key);
    }else if(root->left && root->right){        // Having both left child and right child.
        tmp = findMin(root->right);
        root->value = tmp->value;
        root->right = deleteNode(root->right, root->value);
    }else{
        tmp = root;
        if(root->left == NULL){     // Having only right child.
            root = root->right;
        }else{                      // Having only left child.
            root = root->left;
        }
        tmp = NULL;
        free (tmp);
    }

    return root;
}

int findNode(Tree root, int key){       // Return 1 for existing, 0 for non-existing.
    Tree tmp = root;
    while(tmp){           // Find the node.
        if(tmp->value > key){
            if(!tmp->left) break;
            tmp = tmp->left;
        }else if(tmp->value < key){
            if(!tmp->right) break;
            tmp = tmp->right;
        }else{      // Key is found.
            
            return 1;
        }
    }

    return 0;       // Key is not found.
}

void printInorder(Tree root, int check){
    if(root->left){                         // Leftside.
        printInorder(root->left, 0);
    }
    if(!check || root->right != NULL)        // Oneself.
        fprintf(fout, "%d ", root->value);
    else                                    // The last node.
        fprintf(fout, "%d", root->value);
    if(root->right){                        // Rightside.
        if(check)
            printInorder(root->right, 1);
        else
            printInorder(root->right, 0);
    }

    return;
}

void deleteTree(Tree root){
    if(root == NULL) return;        // Empty tree.

    if(root->left)                 // Free the nodes recursively.
        deleteTree(root->left);
    if(root->right)
        deleteTree(root->right);
    free(root);

    return;
}

Tree findMin(Tree root){            // Find the leftmost node.
    if(root == NULL) return NULL;
    
    Tree tmp = root;
    while(tmp->left){
        tmp = tmp->left;
    }

    return tmp;
}
