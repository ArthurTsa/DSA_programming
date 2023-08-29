#include <stdio.h>
#include <stdlib.h>
typedef struct knight{
    long long h, a;
    int attacktime;
    struct position* set;
}Knight;
typedef struct position{
    int size, curr;
    int totalattacktime;
    long long hurt;
    long long ATK;
    struct knight** heap;
}Position;
void swap(Knight** a, Knight** b){
    Knight*c=*a;
    *a=*b;
    *b=c;
}
void min_heapify(Position* place, int i){
    int heapsize=place->curr;
    int left=2*i, right=2*i+1, smallest;
    if (left<=heapsize&&place->heap[left]->h < place->heap[i]->h) smallest=left;
    else smallest=i;
    if (right<=heapsize&&place->heap[right]->h < place->heap[smallest]->h) smallest=right;
    if (smallest!=i){
        swap(&place->heap[i], &place->heap[smallest]);
        min_heapify(place, smallest);
    }
}
void insert_heap(Position* place, Knight* newknight){
    place->curr++;
    place->heap[place->curr]=newknight;
    int cursor=place->curr, smaller;
    while (cursor!=1){
        smaller=(place->heap[cursor]->h <= place->heap[cursor/2]->h)?cursor:cursor/2;
        if (smaller==cursor/2) break;
        else{
            swap(&place->heap[cursor], &place->heap[cursor/2]);
            cursor=cursor/2;
        }
    }
    if (place->curr==place->size){
        place->heap=(Knight**)realloc(place->heap, (1+(place->size)*2)*sizeof(Knight*));
        place->size*=2;
    }
}
void remove_smallest(Position* place){
    place->heap[1]=place->heap[place->curr];
    place->curr--;
    min_heapify(place, 1);
}
Knight* peek_smallest(Position* place){
    return place->heap[1];
}
void AttackUnion(Position* attacker, Position* defenser){
    if (attacker->curr>=defenser->curr){
        attacker->totalattacktime+=1;
        defenser->hurt+=attacker->ATK;
        while ((defenser->curr)>0){
            if ((peek_smallest(defenser)->h)<=(defenser->hurt)){
                defenser->ATK-=peek_smallest(defenser)->a;
                peek_smallest(defenser)->h=0;
                peek_smallest(defenser)->attacktime+=defenser->totalattacktime;
                peek_smallest(defenser)->set=NULL;
                remove_smallest(defenser);
            }else break;
        }
        for (int i=1;i<=defenser->curr;i++){
            (defenser->heap[i]->h)+=(attacker->hurt-defenser->hurt);
            (defenser->heap[i]->attacktime)+=(defenser->totalattacktime-attacker->totalattacktime);
            (defenser->heap[i]->set)=attacker;
            insert_heap(attacker, defenser->heap[i]);
        }
        (attacker->ATK)+=(defenser->ATK);
        free(defenser->heap);
    }else{
        defenser->hurt+=(attacker->ATK);
        attacker->totalattacktime+=1;
        while ((defenser->curr)>0){
            if ((peek_smallest(defenser)->h)<=(defenser->hurt)){
                defenser->ATK-=(peek_smallest(defenser)->a);
                peek_smallest(defenser)->h=0;
                peek_smallest(defenser)->attacktime+=(defenser->totalattacktime);
                peek_smallest(defenser)->set=NULL;
                remove_smallest(defenser);
            }else break;
        }
        for (int i=1;i<=attacker->curr;i++){
            (attacker->heap[i]->h)+=(defenser->hurt-attacker->hurt);
            (attacker->heap[i]->attacktime)+=(attacker->totalattacktime-defenser->totalattacktime);
            (attacker->heap[i]->set)=defenser;
            insert_heap(defenser, attacker->heap[i]);
        }
        (defenser->ATK)+=(attacker->ATK);
        free(attacker->heap);
    }
}

int main(){
    int n, m;
    scanf("%d %d", &n, &m);
    Position* World=(Position*)malloc((n+1)*sizeof(Position));
    Knight** indextoptr=(Knight**)malloc((n+1)*sizeof(Knight*));
    for (int i=1;i<=n;i++){
        World[i].heap=(Knight**)malloc(3*sizeof(Knight*));
        World[i].size=2;
        World[i].curr=1;
        World[i].hurt=0;
        World[i].totalattacktime=0;
        World[i].heap[1]=(Knight*)malloc(sizeof(Knight));
        (World[i].heap[1]->set)=(&World[i]);
        World[i].heap[1]->attacktime=0;
        indextoptr[i]=World[i].heap[1];
    }
    for (int i=1;i<=n;i++) scanf("%lld", &World[i].heap[1]->h);
    for (int i=1;i<=n;i++){
        scanf("%lld", &World[i].heap[1]->a);
        World[i].ATK=World[i].heap[1]->a;
    }
    for (int i=0;i<m;i++){
        int Ka, Ks;
        scanf("%d %d", &Ka, &Ks);
        Position *attacker=indextoptr[Ka]->set, *defenser=indextoptr[Ks]->set;
        if (attacker==defenser||attacker==NULL||defenser==NULL) continue;
        else AttackUnion(attacker, defenser);
    }
    for (int i=1;i<=n;i++){
        if (indextoptr[i]->set!=NULL){
            (indextoptr[i]->attacktime)+=((indextoptr[i]->set)->totalattacktime);
        }
    }
    for (int i=1;i<=n;i++) printf("%lld ", indextoptr[i]->attacktime);
    return 0;
}
