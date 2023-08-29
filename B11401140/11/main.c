#include <stdio.h>
#include <stdlib.h>
typedef struct rbnode{
    long long key;
    struct rbnode *parent, *left, *right;
    int color;// 0 for black, 1 for red
    long long size;
    long long oddsum, evensum;
    long long base;
}RBNode;
RBNode* NIL;
long long basiccost;
RBNode* createNode(long long key, long long base) {
    RBNode* newNode=(RBNode*)malloc(sizeof(RBNode));
    newNode->key=key;
    newNode->parent=NIL;
    newNode->left=NIL;
    newNode->right=NIL;
    newNode->color=1;
    newNode->size=1;
    newNode->oddsum=key;
    newNode->evensum=0;
    newNode->base=base;
    return newNode;
}
long long getsize(RBNode* x){
    if (x!=NIL) return x->size;
    else return 0;
}
long long getoddsum(RBNode* x){
    if (x!=NIL) return x->oddsum;
    else return 0;
}
long long getevensum(RBNode* x){
    if (x!=NIL) return x->evensum;
    else return 0;
}
void pull(RBNode* x){
    if (x!=NIL){
        x->size=getsize(x->left)+1+getsize(x->right);
        if (getsize(x->left)%2==0){
            x->oddsum=getoddsum(x->left)+x->key+getevensum(x->right);
            x->evensum=getevensum(x->left)+getoddsum(x->right);
        }else{
            x->oddsum=getoddsum(x->left)+getoddsum(x->right);
            x->evensum=getevensum(x->left)+x->key+getevensum(x->right);
        }
    }
}
void continuepull(RBNode* cursor){
    while (cursor!=NIL){
        pull(cursor);
        cursor=cursor->parent;
    }
}
void leftRotate(RBNode** root, RBNode* x) {
    RBNode*y=x->right;
    x->right=y->left;
    if (y->left!=NIL) y->left->parent=x;
    y->parent=x->parent;
    if (x->parent==NIL) *root=y;
    else if (x==x->parent->left) x->parent->left = y;
    else x->parent->right=y;
    y->left=x;
    x->parent=y;
    pull(x);
    pull(y);
}
void rightRotate(RBNode** root, RBNode* y) {
    RBNode* x=y->left;
    y->left=x->right;
    if (x->right!=NIL) x->right->parent=y;
    x->parent=y->parent;
    if (y->parent==NIL) *root=x;
    else if (y==y->parent->left) y->parent->left=x;
    else y->parent->right=x;
    x->right=y;
    y->parent=x;
    pull(y);
    pull(x);
}
void RB_insert_fixup(RBNode** root, RBNode* z) {
    while (z != *root && z->parent->color == 1) {
        if (z->parent == z->parent->parent->left) {
            RBNode* y = z->parent->parent->right;
            if (y != NIL && y->color == 1) {
                z->parent->color = 0;
                y->color = 0;
                z->parent->parent->color = 1;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(root, z);
                }
                z->parent->color = 0;
                z->parent->parent->color = 1;
                rightRotate(root, z->parent->parent);
            }
        } else {
            RBNode* y = z->parent->parent->left;
            if (y != NIL) {
                if (y->color == 1){
                    z->parent->color = 0;
                    y->color = 0;
                    z->parent->parent->color = 1;
                    z = z->parent->parent;
                }else{
                    if (z == z->parent->left) {
                        z = z->parent;
                        rightRotate(root, z);
                    }
                    z->parent->color = 0;
                    z->parent->parent->color = 1;
                    leftRotate(root, z->parent->parent);
                }
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(root, z);
                }
                z->parent->color = 0;
                z->parent->parent->color = 1;
                leftRotate(root, z->parent->parent);
            }
        }
    }
    (*root)->color = 0;
}
RBNode* RB_insert(RBNode** root, long long key, long long base) {
    RBNode* z=createNode(key, base);
    RBNode* y=NIL;
    RBNode* x=*root;
    while (x!=NIL){
        y=x;
        if (z->key>x->key) x=x->left;
        else x=x->right;
    }
    z->parent = y;
    if (y==NIL) *root=z;
    else if (z->key>y->key) y->left=z;
    else y->right=z;
    z->left=NIL;
    z->right=NIL;
    z->color=1;
    continuepull(z);
    RB_insert_fixup(root, z);
    basiccost+=base;
    return z;
}
RBNode* tree_maximum(RBNode* node) {
    while (node->left!=NIL) node=node->left;
    return node;
}
// Function to transplant a subtree in place of another subtree
void RB_transplant(RBNode** root, RBNode* u, RBNode* v) {
    if (u->parent==NIL) *root=v;
    else if (u==u->parent->left) u->parent->left=v;
    else u->parent->right=v;
    if (v!=NIL) v->parent=u->parent;
}
// Function to fix the red-black tree after node deletion
void RB_delete_fixup(RBNode** root, RBNode* x){
    if (x==NIL) return;
    while (x!=(*root)&&(x->color==0)){
        if (x==x->parent->left){
            RBNode* w=x->parent->right;
            if (w->color==1){
                w->color=0;
                x->parent->color=1;
                leftRotate(root, x->parent);
                w=x->parent->right;
            }
            if (w==NIL) break;
            if ((w->left==NIL||w->left->color==0)&&(w->right==NIL||w->right->color==0)){
                w->color=1;
                x=x->parent;
            }else{
                if (w->right==NIL||w->right->color==0){
                    if (w->left!=NIL) w->left->color=0;
                    w->color=1;
                    rightRotate(root, w);
                    w=x->parent->right;
                }
                w->color=x->parent->color;
                x->parent->color=0;
                if (w->right!=NIL) w->right->color=0;
                leftRotate(root, x->parent);
                x=*root;
                break;
            }
        }else{
            RBNode* w=x->parent->left;
            if (w->color==1){
                w->color=0;
                x->parent->color=1;
                rightRotate(root, x->parent);
                w=x->parent->left;
            }
            if (w==NIL) break;
            if ((w->right->color==0)&&(w->left->color==0)){
                w->color=1;
                x=x->parent;
            }else{
                if (w->left==NIL||w->left->color==0){
                    if (w->right!=NIL) w->right->color=0;
                    w->color=1;
                    leftRotate(root, w);
                    w=x->parent->left;
                }
                w->color=x->parent->color;
                x->parent->color=0;
                if (w->left!=NULL)
                    w->left->color=0;
                rightRotate(root, x->parent);
                x=*root;
                break;
            }
        }
    }
    if (x!=NIL) x->color=0;
}
void RB_delete(RBNode** root, RBNode* z) {
    RBNode* x;
    RBNode* y=z;
    int y_original_color=y->color;
    if (z->left==NIL){
        x=z->right;
        RB_transplant(root, z, z->right);
        continuepull(z->parent);
    }else if (z->right==NIL){
        x=z->left;
        RB_transplant(root, z, z->left);
        continuepull(z->parent);
    }else{
        RBNode* debug;
        y=tree_maximum(z->right);
        y_original_color=y->color;
        x=y->right;
        if (y->parent==z){
            debug=y;
        }else{
            debug=y->parent;
            RB_transplant(root, y, y->right);
            y->right=z->right;
            if (y->right!=NIL) y->right->parent=y;
        }
        RB_transplant(root, z, y);
        y->left=z->left;
        y->left->parent=y;
        y->color=z->color;
        continuepull(debug);
    }
    if (y_original_color==0) RB_delete_fixup(root, x);
    basiccost-=z->base;
    free(z);
}
/*
// Function to print binary tree in 2D
// It does reverse inorder traversal
void print2DUtil(RBNode* root, int space){
    if (root == NULL) return;
    // Increase distance between levels
    space += 2;
    // Process right child first
    print2DUtil(root->right, space);
    // Print current node after space
    // count
    //printf("\n");
    for (int i = 2; i < space; i++) printf(" ");
    printf("%d(key:%lld,size:%lld,oddsum:%lld,evensum:%lld)\n", root->color, root->key, root->size, root->oddsum, root->evensum);
    // Process left child
    print2DUtil(root->left, space);
}
// Wrapper over print2DUtil()
void print2D(RBNode* root){
    // Pass initial space count as 0
    print2DUtil(root, 0);
}*/
int main(){
    NIL=(RBNode*)malloc(sizeof(RBNode));
    NIL->color=0;
    RBNode* root = NIL;
    RBNode* indextoptr[100010];
    int n, m;
    scanf("%d %d", &n, &m);
    long long a, b;
    basiccost=0;
    for (int i=1;i<=n;i++){
        scanf("%lld %lld", &a, &b);
        if (a<b) a^=b^=a^=b;
        indextoptr[i]=RB_insert(&root, a-b, b);
    }
    long long temp=basiccost+root->evensum;
    printf("%lld\n", temp);
    long long t, c, d, e, f;
    //print2D(root);
    if (n==1){
        for (int i=0;i<m-1;i++){
            scanf("%lld %lld %lld %lld %lld", &t, &c, &d, &e, &f);
            long long newa=(c*(temp%1000000007)+d)%1000000007, newb=(e*(temp%1000000007)+f)%1000000007;
            if (newa<newb) newa^=newb^=newa^=newb;
            root->oddsum=newa-newb;
            root->evensum=0;
            root->base=newb;
            basiccost=newb;
            temp=basiccost+root->evensum;
            printf("%lld\n", temp);
            //print2D(root);
        }
    }else{
        for (int i=0;i<m-1;i++){
            scanf("%lld %lld %lld %lld %lld", &t, &c, &d, &e, &f);
            //printf("(key:%lld,size:%lld,oddsum:%lld,evensum:%lld)\n", indextoptr[t]->key, indextoptr[t]->size, indextoptr[t]->oddsum, indextoptr[t]->evensum);
            RB_delete(&root, indextoptr[t]);
            long long newa=(c*(temp%1000000007)+d)%1000000007, newb=(e*(temp%1000000007)+f)%1000000007;
            if (newa<newb) newa^=newb^=newa^=newb;
            indextoptr[t]=RB_insert(&root, newa-newb, newb);
            temp=basiccost+root->evensum;
            printf("%lld\n", temp);
            //print2D(root);
        }
    }
    return 0;
}
