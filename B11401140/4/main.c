#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

struct player{
    int index;
    long long ai;
    struct player* prev;
    struct player* next;
};
typedef struct player Player;

Player *head=NULL, *tail=NULL;
int size=0;

void insert_aftertail(int scan_index, long long scan_ai){
    Player* newnode=(Player*)malloc(sizeof(Player));
    newnode->index=scan_index;
    newnode->ai=scan_ai;
    newnode->prev=tail;
    newnode->next=NULL;

    if (tail!=NULL){
        tail->next=newnode;
    }else{
        head=newnode;
    }
    tail=newnode;
    size++;
}
void delete_player(Player* node){
    if (node==head){
        head=head->next;
    }else{
        node->prev->next=node->next;
    }
    if (node==tail){
        tail=tail->prev;
    }else{
        node->next->prev=node->prev;
    }
    free(node);
    size--;
}

int main(){
    int n, m, scan_ai;
    Player *temp, *killer;
    scanf("%d %d", &n, &m);
    for (int i=1;i<=n;i++){
        scanf("%d", &scan_ai);
        insert_aftertail(i, scan_ai);
        printf("Round %d:", i);
        if (tail!=head){
            if (tail->prev->ai<scan_ai||size>m){
                temp=tail->prev;
                while (temp!=NULL){
                    if (temp->ai<scan_ai){
                        killer=temp;
                        temp=temp->prev;
                        printf(" %d", killer->index);
                        delete_player(killer);
                    }else{
                        break;
                    }
                }
            }
        }
        if (size>m){
            printf(" %d", head->index);
            delete_player(head);
        }
        printf("\n");
    }
    printf("Final:");
    temp=tail;
    while (temp!=NULL){
        printf(" %d", temp->index);
        killer=temp;
        temp=temp->prev;
        delete_player(killer);
    }
    return 0;
}
