#include <stdio.h>
#include <stdlib.h>
typedef struct pair{
    int price, expire_day;
}Pair;
typedef struct treenode{
    int name;
    struct treenode *child, *sibling;
    Pair *store;
    int store_num;
    int maxmelon;
    long long mincost;
}TreeNode;
int n, m, c;
void swap(Pair* a, Pair* b){
    Pair c=*a;
    *a=*b;
    *b=c;
}
void min_heapify(TreeNode* company, int i){
    Pair* A=company->store;
    int A_heapsize=company->store_num;
    int left=2*i, right=2*i+1, smallest;
    if (left<=A_heapsize&&A[left].price<A[i].price) smallest=left;
    else smallest=i;
    if (right<=A_heapsize&&A[right].price<A[smallest].price) smallest=right;
    if (smallest!=i){
        swap(&A[i], &A[smallest]);
        min_heapify(company, smallest);
    }
}
void insert_heap(TreeNode* company, Pair newmelon){
    Pair* A=company->store;
    company->store_num++;
    A[company->store_num]=newmelon;
    int cursor=company->store_num, smaller;
    while (cursor!=1){
        smaller=(A[cursor].price<=A[cursor/2].price)?cursor:cursor/2;
        if (smaller==cursor/2) break;
        else{
            swap(&A[cursor], &A[cursor/2]);
            cursor=cursor/2;
        }
    }
}
void remove_smallest(TreeNode* company){
    company->store[1]=company->store[company->store_num];
    company->store_num--;
    min_heapify(company, 1);
}
Pair peek_smallest(TreeNode* company){
    return company->store[1];
}
int TrueMaxMelon;
void findbest(TreeNode* root){
    if (root!=NULL){
        int MaxMelon=0;
        long long MinCost=0;
        TreeNode* cursor=root->child;
        while (cursor!=NULL){
            findbest(cursor);
            MaxMelon+=(cursor->maxmelon);
            MinCost+=(cursor->mincost);
            cursor=cursor->sibling;
        }
        if (root->store_num>0){
            Pair nowmelon=peek_smallest(root);
            MaxMelon+=1;
            MinCost+=nowmelon.price;
        }
        root->maxmelon=MaxMelon;
        root->mincost=MinCost;
        if (root->maxmelon>TrueMaxMelon&&root->mincost<=c) TrueMaxMelon=root->maxmelon;
    }
}
int main(){
    if (scanf("%d %d %d", &n, &m, &c)){};
    TreeNode** nametoptr=(TreeNode**)malloc((n+1)*sizeof(TreeNode*));
    for (int i=1;i<=n;i++){
        TreeNode* new=(TreeNode*)malloc(sizeof(TreeNode));
        new->name=i;
        new->child=NULL, new->sibling=NULL;
        new->store=(Pair*)malloc((m+1)*sizeof(Pair));
        new->store_num=0;
        nametoptr[i]=new;
    }
    for (int i=2;i<=n;i++){
        int scan_parent;
        if (scanf("%d", &scan_parent)){};
        TreeNode *Parent=nametoptr[scan_parent], *Child=nametoptr[i];
        if (Parent->child==NULL) Parent->child=Child;
        else{
            TreeNode *cursor=Parent->child, *prev=NULL;
            while (cursor!=NULL){
                prev=cursor;
                cursor=cursor->sibling;
            }
            prev->sibling=Child;
        }
    }
    for (int d=0;d<m;d++){
        for (int i=1;i<=n;i++){
            Pair newmelon;
            if (scanf("%d %d", &newmelon.price, &newmelon.expire_day));
            newmelon.expire_day+=d;
            insert_heap(nametoptr[i], newmelon);
            while (1){
                if (nametoptr[i]->store_num>0){
                    if (peek_smallest(nametoptr[i]).expire_day<d) remove_smallest(nametoptr[i]);
                    else break;
                }else break;
            }
        }
        TrueMaxMelon=0;
        findbest(nametoptr[1]);
        printf("%d\n", TrueMaxMelon);
    }
    return 0;
}
