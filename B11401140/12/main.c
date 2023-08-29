#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define P 6469693231
typedef struct rb_node {
    long long key1, key2;
    struct rb_node *parent, *left, *right;
    int color;
    long long num;
} RBNode;
RBNode* createNode(long long key1, long long key2) {
    RBNode* newNode = (RBNode*)malloc(sizeof(RBNode));
    newNode->key1 = key1;
    newNode->key2 = key2;
    newNode->parent = NULL;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->color = 1;
    newNode->num = 1;
    return newNode;
}
void leftRotate(RBNode** root, RBNode* x) {
    RBNode* y = x->right;
    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}
void rightRotate(RBNode** root, RBNode* y) {
    RBNode* x = y->left;
    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == NULL)
        *root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;
    x->right = y;
    y->parent = x;
}
void RB_insert_fixup(RBNode** root, RBNode* z) {
    while (z != *root && z->parent->color == 1) {
        if (z->parent == z->parent->parent->left) {
            RBNode* y = z->parent->parent->right;
            if (y != NULL && y->color == 1) {
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
            if (y != NULL && y->color == 1) {
                z->parent->color = 0;
                y->color = 0;
                z->parent->parent->color = 1;
                z = z->parent->parent;
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
void RB_insert(RBNode** root, long long key1, long long key2) {
    RBNode* z = createNode(key1, key2);
    RBNode* y = NULL;
    RBNode* x = *root;
    while (x != NULL) {
        y = x;
        if (z->key1 < x->key1) x = x->left;
        else if (z->key1 == x->key1 && z->key2 < x->key2) x = x->left;
        else x = x->right;
    }
    z->parent = y;
    if (y == NULL) *root = z;
    else if (z->key1 < y->key1) y->left = z;
    else if (z->key1 == y->key1 && z->key2 < y->key2) y->left = z;
    else y->right = z;
    z->left = NULL;
    z->right = NULL;
    z->color = 1;
    RB_insert_fixup(root, z);
}
RBNode* tree_minimum(RBNode* node) {
    while (node->left != NULL) node = node->left;
    return node;
}
void RB_transplant(RBNode** root, RBNode* u, RBNode* v) {
    if (u->parent == NULL) *root = v;
    else if (u == u->parent->left) u->parent->left = v;
    else u->parent->right = v;
    if (v != NULL) v->parent = u->parent;
}
void RB_delete_fixup(RBNode** root, RBNode* x, RBNode* x_parent) {
    while (x != *root && (x == NULL || x->color == 0)) {
        if (x == x_parent->left) {
            RBNode* w = x_parent->right;
            if (w->color == 1) {
                w->color = 0;
                x_parent->color = 1;
                leftRotate(root, x_parent);
                w = x_parent->right;
            }
            if ((w->left == NULL || w->left->color == 0) && (w->right == NULL || w->right->color == 0)) {
                w->color = 1;
                x = x_parent;
                x_parent = x_parent->parent;
            } else {
                if (w->right == NULL || w->right->color == 0) {
                    if (w->left != NULL)
                        w->left->color = 0;
                    w->color = 1;
                    rightRotate(root, w);
                    w = x_parent->right;
                }
                w->color = x_parent->color;
                x_parent->color = 0;
                if (w->right != NULL)
                    w->right->color = 0;
                leftRotate(root, x_parent);
                x = *root;
                break;
            }
        } else {
            RBNode* w = x_parent->left;
            if (w->color == 1) {
                w->color = 0;
                x_parent->color = 1;
                rightRotate(root, x_parent);
                w = x_parent->left;
            }
            if ((w->right == NULL || w->right->color == 0) && (w->left == NULL || w->left->color == 0)) {
                w->color = 1;
                x = x_parent;
                x_parent = x_parent->parent;
            } else {
                if (w->left == NULL || w->left->color == 0) {
                    if (w->right != NULL)
                        w->right->color = 0;
                    w->color = 1;
                    leftRotate(root, w);
                    w = x_parent->left;
                }
                w->color = x_parent->color;
                x_parent->color = 0;
                if (w->left != NULL)
                    w->left->color = 0;
                rightRotate(root, x_parent);
                x = *root;
                break;
            }
        }
    }
    if (x != NULL) x->color = 0;
}
void RB_delete(RBNode** root, RBNode* z) {
    RBNode* x;
    RBNode* y = z;
    long long y_original_color = y->color;
    if (z->left == NULL) {
        x = z->right;
        RB_transplant(root, z, z->right);
    } else if (z->right == NULL) {
        x = z->left;
        RB_transplant(root, z, z->left);
    } else {
        y = tree_minimum(z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z)
            x->parent = y;
        else {
            RB_transplant(root, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        RB_transplant(root, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if (y_original_color == 0)
        RB_delete_fixup(root, x, y->parent);
    free(z);
}
RBNode* RBsearch(RBNode* root, long long key1, long long key2){
    if (root==NULL) return NULL;
    if (key1==root->key1 && key2==root->key2) return root;
    else if (key1<root->key1 || (key1==root->key1&&key2<root->key2)) return RBsearch(root->left, key1, key2);
    else return RBsearch(root->right, key1, key2);
}
long long TraverseandCalculate(RBNode* root) {
    if (root != NULL) return TraverseandCalculate(root->left)+root->num*(root->num-1)/2+TraverseandCalculate(root->right);
    else return 0;
}
long long Max_inallrotation(char s[]){
    long long temp=0, mul=1;
    for (int i=strlen(s)-1;i>=0;i--){
        temp=(temp+((int)(s[i]-'a')+1)*mul)%P;
        if (i>0) mul=(mul*26)%P;
    }
    long long max=temp;
    for (int i=0;i<strlen(s);i++){
        temp=((temp-((int)(s[i]-'a')+1)*mul)*26+(int)(s[i]-'a')+1);
        while (temp<0) temp+=P;
        temp%=P;
        if (temp>max) max=temp;
    }
    return max;
}
long long Min_inallrotation(char s[]){
    long long temp=0, mul=1;
    for (int i=strlen(s)-1;i>=0;i--){
        temp=(temp+((int)(s[i]-'a')+1)*mul)%P;
        if (i>0) mul=(mul*26)%P;
    }
    long long min=temp;
    for (int i=0;i<strlen(s);i++){
        temp=((temp-((int)(s[i]-'a')+1)*mul)*26+(int)(s[i]-'a')+1);
        while (temp<0) temp+=P;
        temp%=P;
        if (temp<min) min=temp;
    }
    return min;
}
int main(){
    RBNode* root = NULL;
    int N, Q;
    scanf("%d %d", &N, &Q);
    char s[1000010];
    for (int i=0;i<N;i++){
        scanf("%s", s);
        long long newkey1=Max_inallrotation(s), newkey2=Min_inallrotation(s);
        RBNode* result=RBsearch(root, newkey1, newkey2);
        if (result==NULL) RB_insert(&root, newkey1, newkey2);
        else result->num++;
    }
    long long numof_strpairs=TraverseandCalculate(root);
    printf("%lld\n", numof_strpairs);
    int p;
    for (int i=0;i<Q;i++){
        scanf("%d %s", &p, s);
        if (p==1){
            long long newkey1=Max_inallrotation(s), newkey2=Min_inallrotation(s);
            RBNode* result=RBsearch(root, newkey1, newkey2);
            if (result==NULL){
                RB_insert(&root, newkey1, newkey2);
            }else{
                numof_strpairs-=(result->num*(result->num-1)/2);
                result->num++;
                numof_strpairs+=(result->num*(result->num-1)/2);
            }
        }else if (p==2){
            long long newkey1=Max_inallrotation(s), newkey2=Min_inallrotation(s);
            RBNode* result=RBsearch(root, newkey1, newkey2);
            numof_strpairs-=(result->num*(result->num-1)/2);
            if (result->num>0){
                result->num--;
                numof_strpairs+=(result->num*(result->num-1)/2);
            }else RB_delete(&root, result);
        }
        printf("%lld\n", numof_strpairs);
    }
    return 0;
}
