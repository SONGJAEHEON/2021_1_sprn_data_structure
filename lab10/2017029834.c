#include <stdio.h>
#include <stdlib.h>

#define INT_MIN -2147483648

FILE* fin;
FILE* fout;

typedef struct BNode* BNodePtr;

struct BNode{
    int order;      // m value;
    int size;       // number of children
    BNodePtr* child;// children pointers
    int* key;       // keys
    int is_leaf;
}BNode;

BNodePtr CreateTree(int order);
BNodePtr CreateNodePtr(int* arr, int order, int length);
void Insert(BNodePtr root, int key);            // not yet!
void InsertAtLeaf(BNodePtr root, int key);
int Find(BNodePtr root, int key);
BNodePtr FindLoc(BNodePtr root, int key);
BNodePtr FindParentLoc(BNodePtr root, int key);
int CountKey(BNodePtr root);
int GetMax(BNodePtr root, int key);     // 최댓값 리턴하고 내부 정렬
int GetMin(BNodePtr root, int key);     // 최솟값 리턴하고 내부 정렬
BNodePtr SplitRoot(BNodePtr root);      // not yet!
BNodePtr Split(BNodePtr parent, BNodePtr oneself, int std, int key, int cnt);  // not yet!
void PrintTree(BNodePtr root, int isRoot);
void DeleteTree(BNodePtr root);
int Ceil(double input);

void main(int argc, char* argv[]){
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    int order;
    fscanf(fin, "%d", &order);
    BNodePtr root = CreateTree(order);

    char cv;
    int key;
    while(!feof(fin)){
        fscanf(fin, "%c", &cv);
        switch(cv){
            case 'i':
                fscanf(fin, "%d", &key);
                if(Find(root, key))
                    fprintf(fout, "Insert Error: key %d is already in the tree!\n", key);
                else
                    Insert(root, key);
                break;
            case 'f':
                fscanf(fin, "%d", &key);
                if(Find(root, key))
                    fprintf(fout, "Find key %d\n", key);
                else
                    fprintf(fout, "Finding Error: key %d is not in the tree!\n", key);
                break;
            case 'p':
                PrintTree(root, 1);
                break;
        }
    }
    
    DeleteTree(root);

    return 0;
}

BNodePtr CreateTree(int order){
    BNodePtr ptr = (BNodePtr)malloc(sizeof(BNode));
    ptr->order = order;
    ptr->size = 0;
    ptr->child = (BNodePtr*)malloc((order)*sizeof(BNode));
    ptr->key = (int*)malloc(order*sizeof(int));
    for(int i = 0; i < order; i++){
        ptr->key[i] = INT_MIN;
    }
    ptr->is_leaf = 1;

    return ptr;
}

BNodePtr CreateNodePtr(int* arr, int order, int length){
    BNodePtr ptr = (BNodePtr)malloc(sizeof(BNode));
    ptr->order = order;
    ptr->size = 0;          // 리턴 받아서 할당해줘야함.
    ptr->child = (BNodePtr*)malloc((order)*sizeof(BNode));
    ptr->key = (int*)malloc(order*sizeof(int));
    for(int i = 0; i < length; i++){
        ptr->key[i] = arr[i];
    }
    for(int i = length; i < order; i++){
        ptr->key[i] = INT_MIN;
    }
    // ptr->is_leaf = 1;

    return ptr;
}

void Insert(BNodePtr root, int key){
                                    // // 1. leaf node까지 내려가 insert
                                    // // 2. full인 경우 1) rotation 2) split
                                    // int std = 0, tmp = 0, cnt = 0;
                                    // if(!root->is_leaf){
                                    //     while(root->key[std] < key && std <= key)
                                    //         std++;
                                    //     Insert(root->child[std], key);
                                    // }
                                    // // case 1
                                    // if(root->size < root->order-1){
                                    //     while(root->key[std] < key && std <= root->size)
                                    //         std++;
                                    //     cnt = root->size - std;
                                    //     for(int i = 0; i < cnt; i++){
                                    //         root->key[root->size-i+1] = root->key[root->size-i];
                                    //     }
                                    //     root->key[std] = key;
                                    //     root->size++;
                                    // }
                                    // // case 2-1

                                    // // case 2-2
                        // BNodePtr tmp = root, tmp2 = NULL;       // tmp2는 tmp의 parent
                        // int std = 0, cnt = 0;
                        // while(tmp->is_leaf){            // leaf node까지 내려가기
                        //     while(tmp->key[std] != 0){ // 내려갈 child 번호를 찾기
                        //         if(tmp->key[std] < key) std++;
                        //     }
                        //     tmp2 = tmp;
                        //     tmp = tmp->child[std];
                        // }
                        // // for(int i = 0; i < tmp->order; i++){
                        // //     if(tmp->key[i] != 0) cnt++;
                        // // }
                        // cnt = CountKey(tmp);
                        // if(tmp->order-cnt > 1)         // leaf node가 not full
                        //     InsertAtLeaf(tmp, key);
                        // else{               // key rotation 또는 node split
                        //     if(tmp2 != tmp){        // tmp가 root가 아닌 경우
                        //         if(std == 0){       // 제일 왼쪽
                        //             if(CountKey(tmp2->child[std+1]) < tmp2->order-1){
                        //                 InsertAtLeaf(tmp2->child[std+1], tmp2->key[std]);
                        //                 tmp2->key[std] = GetMax(tmp, key);
                        //             }
                        //         }else if(std == tmp2->size){        // 제일 오른쪽
                        //             if(CountKey(tmp2->child[std-1]) < tmp2->order-1){
                        //                 InsertAtLeaf(tmp2->child[std-1], tmp2->key[std-1]);
                        //                 tmp2->key[std-1] = GetMin(tmp, key);
                        //             }
                        //         }else if(CountKey(tmp2->child[std+1]) < tmp2->order-1){
                        //                 InsertAtLeaf(tmp2->child[std+1], tmp2->key[std]);
                        //                 tmp2->key[std] = GetMax(tmp, key);
                        //         }else if(CountKey(tmp2->child[std-1]) < tmp2->order-1){
                        //                 InsertAtLeaf(tmp2->child[std-1], tmp2->key[std-1]);
                        //                 tmp2->key[std-1] = GetMin(tmp, key);
                        //         }else{      // 노드 스플릿

                        //         }
                        //     }else{          // 노드 스플릿
                        //         root = SplitRoot(tmp);
                        //     }
                        // }
    // 1. Leaf node까지 내려가서 insert
    // 2. rotate
    // 3. split
    BNodePtr tmp = FindLoc(root, key), tmp2 = FindParentLoc(root, key);
    // Leaf node에 insert
    if(CountKey(tmp)<root->order-1){
        InsertAtLeaf(tmp, key);
    }else if(tmp == tmp2){
        int i = 0, j = 0;
        int* arr = (int*)malloc((root->order)*sizeof(int));
        while(i < root->order){
            if(tmp->key[j] < key)
                arr[i] =  tmp->key[j++];
            else
                arr[i] = key;
            i++;
        }
        for(int i = 0; i < root->order; i++){
            tmp->key[i] = arr[i];
        }
        free(arr);
        root = SplitRoot(tmp);
        root->child[0]->size = 0;
        root->child[1]->size = 0;
    }else{
        if(tmp = tmp2->child[0]){
            if(CountKey(tmp2->child[1]) < root->order-1){
                InsertAtLeaf(tmp2->child[1], tmp2->key[0]);
                tmp2->key[0] = GetMax(tmp, key);
            }
        }else if(tmp == tmp2->child[tmp->size-1]){
            if(CountKey(tmp2->child[tmp2->size-2]) < root->order-1){
                InsertAtLeaf(tmp2->child[tmp2->size-2], tmp2->key[size-2]);
                tmp2->key[size-2] = GetMin(tmp, key);
            }
        }else{
            int check, cnt = 0;
            do{
                check = 0;
                int std = 0;
                while(std < root->order){
                    if(key > tmp2->key[std])
                        std++;
                }
                if(CountKey(tmp2->child[std+1]) < root->order-1){
                    InsertAtLeaf(tmp2->child[std+1], tmp2->key[std]);
                    tmp2->key[std] = GetMax(tmp, key);
                }else if(CountKey(tmp2->child[std-1]) < root->order-1){
                    InsertAtLeaf(tmp2->child[std-1], tmp2->key[std-1]);
                    tmp2->key[std-1] = GetMin(tmp, key);
                }else{  // Split 해야하는 경우
                    Split(tmp2, tmp, std, key, cnt);
                    if(CountKey(tmp2) > root->order-1){
                        check = 1;
                        cnt++;
                        tmp = tmp2;
                        tmp2 = FindParentLoc(tmp2);
                    }
                }
            }while(check);
        }
    }
}

void InsertAtLeaf(BNodePtr root, int key){
    int cnt = CountKey(root);       // 기존 Key 갯수
    int* arr = (int*)malloc(cnt*sizeof(int));
    int j = 0;
    for(int i = 0; i < (cnt+1); i++){
        if(root->key[i] < key)
            arr[j] = root->key[i];
        else
            arr[j] = key;
        j++;
    }
    for(int i = 0; i < (cnt+1); i++)
        root->key[i] = arr[i];
    free(arr);

    return root;
}

int Find(BNodePtr root, int key){
    int std = 0;
    while(root->key[std] < key && std < root->order){
        std++;
    }
    if(root->key[std] == key)
        return 1;
    else if(root->is_leaf != 0)   // is_leaf
        return Find(root->child[std], key);
    
    return 0;
}

BNodePtr FindLoc(BNodePtr root, int key){
    BNodePtr tmp = root;
    while(1){
        int std = 0;
        while(tmp->key[std] < key && tmp < tmp->order){
            std++;
        }
        if(tmp->key[std] == key || tmp->is_leaf == 1)
            break;
        tmp = tmp->child[std];
    }
    
    return tmp;
}

BNodePtr FindParentLoc(BNodePtr root, int key){
    BNodePtr oneself = root, parent = root;
    while(1){
        int std = 0;
        while(oneself->key[std] < key && oneself < oneself->order){
            std++;
        }
        if(oneself->key[std] == key || oneself->is_leaf == 1)
            break;
        parent = oneself;
        oneself = oneself->child[std];
    }
    if(oneself == parent)
        return NULL;        // 루트 노드인 경우
    else
        return parent;
}

int CountKey(BNodePtr root){
    int cnt = 0;
    for(int i = 0; i < root->order; i++){
        if(root->key[i] != INT_MIN) cnt++;
    }

    return cnt;
}

int GetMax(BNodePtr root, int key){
    int cnt = CountKey(root)+1, i = 0, j = 0, max;
    int* arr = (int*)malloc(cnt*sizeof(int));
    while(i < cnt){
        if(root->key[j] < key){
            arr[i] = root->key[j];
            j++;
        }else{
            arr[i] = key;
        }
        i++;
    }
    max = arr[cnt-1];
    for(int l = 0; l < cnt-1; l++){
        root->key[l] = arr[l];
    }
    free(arr);

    return max;
}

int GetMin(BNodePtr root, int key){
    int cnt = CountKey(root)+1, i = 0, j = 0, min;
    int* arr = (int*)malloc(cnt*sizeof(int));
    while(i < cnt){
        if(root->key[j] < key){
            arr[i] = root->key[j];
            j++;
        }else{
            arr[i] = key;
        }
        i++;
    }
    min = arr[0];
    for(int l = 0; l < cnt-1; l++){
        root->key[l] = arr[l+1];
    }
    free(arr);

    return min;
}

BNodePtr SplitRoot(BNodePtr root){
    BNodePtr tmp1, tmp2, tmp3;
    int* arr1, *arr2, *arr3;
    int mid = root->order/2;      // order3->1(2번째), order4->2(3번째), order5->2, order6->3
    arr1 = (int*)malloc(sizeof(int));
    arr2 = (int*)malloc((mid)*sizeof(int));
    arr3 = (int*)malloc((root->order-mid-1)*sizeof(int));
    // BNode 3개 만들어서 구현한 후에 root는 free시키기
    arr1[0] = root->key[mid];
    for(int i = 0; i < mid; i++){
        arr2[i] = root->key[i];
    }
    for(int i = 0; i < (root->order-mid-1); i++){
        arr3[i] = root->key[i+mid+1];
    }
    tmp1 = CreateNodePtr(arr1, root->order, 1);
    tmp1->is_leaf = 0;
    tmp1->size = 2;
    tmp2 = CreateNodePtr(arr2, root->order, mid);   // is_leaf 생각해줘야함
    tmp2->size = mid+1;
    tmp3 = CreateNodePtr(arr3, root->order, root->order-mid-1);
    tmp3->size = root->order-mid;
    tmp1->child[0] = tmp2;
    tmp1->child[1] = tmp3;

    return tmp1;
}

BNodePtr Split(BNodePtr parent, BNodePtr oneself, int std, int key, int cnt){
    if(cnt == 0){
        int i = 0, j = 0;
        int* arr = (int*)malloc((root->order)*sizeof(int));
        while(i < root->order){
            if(tmp->key[j] < key)
                arr[i] =  tmp->key[j++];
            else
                arr[i] = key;
            i++;
        }
    }
    
    BNodePtr tmp, tmp1, tmp2;
    int* arr1, *arr2;
    int mid = root->order/2;
    int midnum = oneself->key[mid];
    arr1 = (int*)malloc((mid)*sizeof(int));
    arr2 = (int*)malloc((root->order-mid-1)*sizeof(int));
    for(int i = 0; i < mid; i++){
        arr1[i] = oneself->key[i];
    }
    for(int i = 0; i < (root->order-mid-1); i++){
        arr2[i] = root->key[i+mid+1];
    }
    tmp1 = CreateNodePtr(arr1, parent->order, mid);
    tmp1->size = mid+1;
    for(int i = 0; i < mid+1; i++){
        tmp1->child[i] = tmp1->key[i];
    }
    tmp2 = CreateNodePtr(arr2, parent->order, parent->order-mid-1);
    tmp2->size = root->order-mid;
    for(int i = 0; i < order-mid;)
}

void PrintTree(BNodePtr root, int isRoot){
    int cnt = 0;
    if(isRoot){
        if(root->key[0] == 0)
            fprintf(fout, "Print Error: tree is empty!");
    }else{
        while(root->child[cnt]){        // is_leaf
            PrintTree(root->child[cnt], 0);
            if(root->key[cnt] != 0)
                fprintf(fout, "%d ", root->key[cnt]);
            cnt++;
        }
    }
    fprintf(fout, "\n");

    return;
}

void DeleteTree(BNodePtr root){
    int cnt = 0;
    while(root->child[cnt] != NULL){        // is_leaf
        DeleteTree(root->child[cnt]);
        cnt++;
    }
    free(root->child);
    free(root->key);
    free(root);

    return;
}

int Ceil(double input){
    int tmp = (int)input;
    if(input == (double)tmp)
        return tmp;
    else
        return tmp+1;
}