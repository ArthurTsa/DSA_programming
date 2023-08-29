#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct node{
    int rank;
    int toast;
    struct node* direction[4];
}Node;
typedef struct proto_copy{
    Node* model;
    struct proto_copy* direction[4];
    int x, y;
}Copy;
typedef struct T_day{
    int r, lk, s;
}T_Day;
typedef struct R_day{
    int r, lk;
}R_Day;
int n, m;
Node *normal[1010][1010], *reverse[1010][1010];
Copy *copy[1010][1010];
Node *ranktoptr1[1000010], *ranktoptr2[1000010];
Copy *ranktoptrcopy[1000010];
Node* origin;
Copy *passed[1000010], *closed[1000010];
int T[2], R;
T_Day *t[2];
R_Day *r1;
Node* next(Node* prev, Node* now, int mode){
    if (mode==0)
        for (int d=0;d<=2;d+=2){
            if (now->direction[d]!=prev) return now->direction[d];
        }
    else if (mode==1)
        for (int d=1;d<=3;d+=2){
            if (now->direction[d]!=prev) return now->direction[d];
        }
    else return NULL;
}
void traverse_toast(Node* t_start){
    Node *row_start=t_start, *curr, *row_prev=NULL, *column_prev, *temp;
    for (int i=0;i<n;i++){
        curr=row_start;
        column_prev=NULL;
        for (int j=0;j<m;j++){
            printf("%d ", curr->toast);
            temp=curr;
            curr=next(column_prev, curr, 0);
            column_prev=temp;
        }
        printf("\n");
        temp=row_start;
        row_start=next(row_prev, row_start, 1);
        row_prev=temp;
    }
}
void rotation_side(Node* step1, Node* step2, int mode){ //mode=0, 1, 2, 3
    Node *out1, *out2;
    out1=step1->direction[mode];
    out2=step2->direction[mode];
    step1->direction[mode]=out2;
    if (out2!=NULL) out2->direction[(mode+2)%4]=step1;
    step2->direction[mode]=out1;
    if (out1!=NULL) out1->direction[(mode+2)%4]=step2;
    if (step1==origin) origin=step2;
    else if (step2==origin) origin=step1;
}
void rotation_corner(Node* step1, Node* step2, int mode){ //mode=0, 1, 2, 3
    Node *outup1, *outdown1, *outup2, *outdown2;
    outup1=step1->direction[mode];
    outdown1=step1->direction[(mode+1)%4];
    outup2=step2->direction[mode];
    outdown2=step2->direction[(mode+1)%4];
    step1->direction[mode]=outup2;
    if (outup2!=NULL) outup2->direction[(mode+2)%4]=step1;
    step1->direction[(mode+1)%4]=outdown2;
    if (outdown2!=NULL) outdown2->direction[((mode+1)%4+2)%4]=step1;
    step2->direction[mode]=outup1;
    if (outup1!=NULL) outup1->direction[(mode+2)%4]=step2;
    step2->direction[(mode+1)%4]=outdown1;
    if (outdown1!=NULL) outdown1->direction[((mode+1)%4+2)%4]=step2;
    if (step1==origin) origin=step2;
    else if (step2==origin) origin=step1;
}
void cut(Copy* now){
    for (int d=0;d<2;d++){
        if (now->direction[d]!=NULL)
            now->direction[d]->direction[(d+2)%4]=now->direction[(d+2)%4];
        if (now->direction[(d+2)%4]!=NULL)
            now->direction[(d+2)%4]->direction[d]=now->direction[d];
    }
}
void connectback(Copy* now){
    for (int d=0;d<4;d++){
        if (now->direction[d]!=NULL) now->direction[d]->direction[(d+2)%4]=now;
    }
}
int distance(Copy* a, Copy* b){
	int xd=a->x-b->x, yd=a->y-b->y;
	if (xd<0) xd=-xd;
	if (yd<0) yd=-yd;
	return xd+yd;
}
void train(int day){
	memset(copy, 0, sizeof(copy));
	for (int i=1;i<=n;i++){
		for (int j=1;j<=m;j++){
			copy[i][j]=(Copy*)malloc(sizeof(Copy));
            copy[i][j]->x=i;
            copy[i][j]->y=j;
		}
	}
	Node *row_start=origin, *curr, *row_prev=NULL, *column_prev, *temp;
    for (int i=1;i<=n;i++){
        curr=row_start;
        column_prev=NULL;
        for (int j=1;j<=m;j++){
            copy[i][j]->model=curr;
            ranktoptrcopy[curr->rank]=copy[i][j];
            temp=curr;
            curr=next(column_prev, curr, 0);
            column_prev=temp;
        }
        temp=row_start;
        row_start=next(row_prev, row_start, 1);
        row_prev=temp;
    }
    for (int i=1;i<=n;i++){
        for (int j=1;j<=m;j++){
            copy[i][j]->direction[0]=copy[i][j+1];
            copy[i][j]->direction[1]=copy[i-1][j];
            copy[i][j]->direction[2]=copy[i][j-1];
            copy[i][j]->direction[3]=copy[i+1][j];
        }
    }
    for (int i=1;i<=n;i++){ // cut initially closed bakery
        for (int j=1;j<=m;j++){
            if (copy[i][j]->model->toast<=0) cut(copy[i][j]);
        }
    }
    memset(passed, 0, sizeof(passed));
    memset(closed, 0, sizeof(closed));
    int p_index, c_index;
    for (int k=0;k<T[day];k++){ // start training
        p_index=0, c_index=0;
        Copy *now=ranktoptrcopy[t[day][k].r];
        cut(now);
        passed[p_index]=now;
        p_index++;
        for (int l=0;l<t[day][k].lk;l++){
            int min_d=-1, min_dis=10000, min_rank=1000010;
            int all_end=1;
            for (int d=0;d<4;d++){
                if (now->direction[d]!=NULL){
                    all_end=0;
                    if ((distance(now, now->direction[d])<min_dis)||(distance(now, now->direction[d])==min_dis&&now->direction[d]->model->rank<min_rank)){
                        min_d=d;
                        min_dis=distance(now, now->direction[d]);
                        min_rank=now->direction[d]->model->rank;
                    }
                }
            }
            if (all_end==1) break;
            now=now->direction[min_d];
            now->model->toast-=t[day][k].s;
            if (now->model->toast<=0){
                now->model->toast=0;
                closed[c_index]=now;
                c_index++;
            }
            passed[p_index]=now;
            p_index++;
            cut(now);
        }
        for (int p=p_index-1;p>=0;p--) connectback(passed[p]);
        for (int c=0;c<c_index;c++) cut(closed[c]);
    }
    for (int i=1;i<=n;i++){
		for (int j=1;j<=m;j++) free(copy[i][j]);
	}
}
int main(){
    if (scanf("%d %d", &n, &m)){};
    memset(normal, 0, sizeof(normal));
    for (int i=1;i<=n;i++){
        for (int j=1;j<=m;j++) normal[i][j]=(Node*)malloc(sizeof(Node));
    }
    int scan_rank, scan_toast;
    for (int i=1;i<=n;i++){
        for (int j=1;j<=m;j++){
            if (scanf("%d", &scan_rank)){};
            ranktoptr1[scan_rank]=normal[i][j];
            normal[i][j]->rank=scan_rank;
        }
    }
    for (int i=1;i<=n;i++){
        for (int j=1;j<=m;j++){
            if (scanf("%d", &scan_toast)){};
            normal[i][j]->toast=scan_toast;
        }
    }
    for (int i=1;i<=n;i++){
        for (int j=1;j<=m;j++){
            normal[i][j]->direction[0]=normal[i][j+1];
            normal[i][j]->direction[1]=normal[i-1][j];
            normal[i][j]->direction[2]=normal[i][j-1];
            normal[i][j]->direction[3]=normal[i+1][j];
        }
    }
    origin=normal[1][1];
    if (scanf("%d %d %d", &T[0], &R, &T[1])){};
    t[0]=(T_Day*)malloc(T[0]*sizeof(T_Day));
    for (int i=0;i<T[0];i++) if (scanf("%d %d %d", &t[0][i].r, &t[0][i].lk, &t[0][i].s)){};
    r1=(R_Day*)malloc(R*sizeof(R_Day));
    for (int i=0;i<R;i++) if (scanf("%d %d", &r1[i].r, &r1[i].lk)){};
    t[1]=(T_Day*)malloc(T[1]*sizeof(T_Day));
    for (int i=0;i<T[1];i++) if (scanf("%d %d %d", &t[1][i].r, &t[1][i].lk, &t[1][i].s)){};
    train(0);
    memset(reverse, 0, sizeof(reverse));
    for (int i=1;i<=n;i++){
    	for (int j=1;j<=m;j++) reverse[i][j]=(Node*)malloc(sizeof(Node));
    }
    for (int i=1;i<=n;i++){
        for (int j=1;j<=m;j++){
            ranktoptr2[normal[i][j]->rank]=reverse[n-i+1][m-j+1];
            reverse[n-i+1][m-j+1]->rank=normal[i][j]->rank;
            reverse[n-i+1][m-j+1]->toast=normal[i][j]->toast;
        }
    }
    for (int i=1;i<=n;i++){
        for (int j=1;j<=m;j++){
            reverse[i][j]->direction[0]=reverse[i][j+1];
            reverse[i][j]->direction[1]=reverse[i-1][j];
            reverse[i][j]->direction[2]=reverse[i][j-1];
            reverse[i][j]->direction[3]=reverse[i+1][j];
        }
    }
    for (int k=0;k<R;k++){
        Node *center1=ranktoptr1[r1[k].r], *center2=ranktoptr2[r1[k].r];
        Node *step1=center1, *step2=center2;
        for (int l=0;l<(r1[k].lk-1)/2;l++){
            step1=step1->direction[0];
            step2=step2->direction[0];
        }
        for (int l=0;l<(r1[k].lk-1)/2;l++){
            rotation_side(step1, step2, 0);
            step1=step1->direction[1];
            step2=step2->direction[1];
        }
        if (r1[k].lk>1){
            for (int d=0;d<3;d++){
                rotation_corner(step1, step2, d);
                step1=step1->direction[(d+2)%4];
                step2=step2->direction[(d+2)%4];
                for (int l=0;l<(r1[k].lk-2);l++){
                    rotation_side(step1, step2, d+1);
                    step1=step1->direction[(d+2)%4];
                    step2=step2->direction[(d+2)%4];
                }
            }
            rotation_corner(step1, step2, 3);
            step1=step1->direction[1];
            step2=step2->direction[1];
            for (int l=0;l<(r1[k].lk-1)/2-1;l++){
                rotation_side(step1, step2, 0);
                step1=step1->direction[1];
                step2=step2->direction[1];
            }
        }
    }
    train(1);
    traverse_toast(origin);
    free(t[0]);
    free(r1);
    free(t[1]);
    return 0;
}
