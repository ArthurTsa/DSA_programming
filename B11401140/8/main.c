#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
typedef struct cat{
    int a, b;
    struct cat *prev, *next;
}Cat;
typedef struct cirArr{
    Cat** Array;
    int length;
    int head, tail;
}CirArr;
int compare(const void* ptr1, const void* ptr2){
    Cat** catptr1=(Cat**)ptr1;
    Cat** catptr2=(Cat**)ptr2;
    if ((*catptr1)->a>(*catptr2)->a) return 1;
    else if ((*catptr1)->a<(*catptr2)->a) return -1;
    else return 0;
}
int compare_color(const void* ptr1, const void* ptr2){
    int* color1=(int*)ptr1;
    int* color2=(int*)ptr2;
    if (*color1>*color2) return 1;
    else if (*color1<*color2) return -1;
    else return 0;
}
int bsearchArr(int Arr[], int length, int target){
    int low=0, high=length-1;
    while (low<=high){
        int mid=(low+high)/2;
        if (Arr[mid]==target) return mid;
        else if (Arr[mid]>target) high=mid-1;
        else low=mid+1;
    }
    return -1;
}
int bsearchCirArr_geq(CirArr A, int target){
    int low=(A.head+1)%A.length, high=(A.tail-1+A.length)%A.length;
    if ((A.Array[high]->a)<target) return -1;
    if (high<low) high+=A.length;
    while (low<high){
        int mid=(low+high)/2;
        if (target==A.Array[mid%A.length]->a) return mid;
        else if (target<A.Array[mid%A.length]->a) high=mid;
        else low=mid+1;
        if (low+1==high){
            if (A.Array[low%A.length]->a>=target) return low;
            else return high;
        }
    }
    return high;
}
int bsearchCirArr_leq(CirArr A, int target){
    int low=(A.head+1)%A.length, high=(A.tail-1+A.length)%A.length;
    if ((A.Array[low]->a)>target) return -1;
    if (high<low) high+=A.length;
    while (low<high){
        int mid=(low+high)/2;
        if (target==A.Array[mid%A.length]->a) return mid;
        else if (A.Array[mid%A.length]->a<target) low=mid;
        else high=mid-1;
        if (low+1==high){
            if (A.Array[high%A.length]->a<=target) return high;
            else return low;
        }
    }
    return low;
}
int bsearchCirArr_byappetite(CirArr A, int appetite){
    int low=(A.head+1)%A.length, high=(A.tail-1+A.length)%A.length;
    if (high<low) high+=A.length;
    while (low<=high){
        int mid=(low+high)/2;
        if (appetite==A.Array[mid%A.length]->a) return mid%A.length;
        else if (A.Array[mid%A.length]->a<appetite) low=mid+1;
        else high=mid-1;
    }
    return -1;
}
void deletenode(Cat* a){
    if (a->prev!=NULL) a->prev->next=a->next;
    if (a->next!=NULL) a->next->prev=a->prev;
}
void insertnode(Cat* x, Cat* a, Cat* y){
    a->next=y;
    a->prev=x;
    if (x!=NULL) x->next=a;
    if (y!=NULL) y->prev=a;
}
int main(){
    int n, m;
    if (scanf("%d %d", &n, &m)){}
    Cat** InputArr=(Cat**)malloc(n*sizeof(Cat*));
    for (int i=0;i<n;i++) InputArr[i]=(Cat*)malloc(sizeof(Cat));
    for (int i=0;i<n;i++){
        if (scanf("%d", &InputArr[i]->a)){}
    }
    for (int i=0;i<n;i++){
        if (scanf("%d", &InputArr[i]->b)){}
    }
    Cat** SortArr=(Cat**)malloc(n*sizeof(Cat*));
    for (int i=0;i<n;i++) SortArr[i]=InputArr[i];
    qsort(SortArr, n, sizeof(Cat*), compare);
    Cat* setprev=NULL;
    Cat *MaxappetiteCat, *MinappetiteCat;
    for (int i=0;i<n;i++){
        SortArr[i]->prev=setprev;
        if (setprev!=NULL) setprev->next=SortArr[i];
        setprev=SortArr[i];
        if (i==0) MinappetiteCat=SortArr[i];
        else if (i==n-1) MaxappetiteCat=SortArr[i];
    }
    setprev->next=NULL;
    int* ColorMap0=(int*)malloc(n*sizeof(int));
    for (int i=0;i<n;i++) ColorMap0[i]=SortArr[i]->b;
    qsort(ColorMap0, n, sizeof(int), compare_color);
    int *ColorMap=(int*)malloc(n*sizeof(int));
    ColorMap[0]=ColorMap0[0];
    int numberofColors=1, prevcolor=ColorMap0[0];
    for (int i=1;i<n;i++){
        if (ColorMap0[i]!=prevcolor){
            ColorMap[numberofColors]=ColorMap0[i];
            numberofColors++;
            prevcolor=ColorMap0[i];
        }
    }
    CirArr* SeparateColors=(CirArr*)malloc(numberofColors*sizeof(CirArr));
    for (int i=0;i<numberofColors;i++) SeparateColors[i].length=0;
    for (int i=0;i<n;i++) SeparateColors[bsearchArr(ColorMap, numberofColors, SortArr[i]->b)].length++;
    for (int i=0;i<numberofColors;i++){
        int len=SeparateColors[i].length;
        SeparateColors[i].Array=(Cat**)malloc(len*sizeof(Cat*));
        memset(SeparateColors[i].Array, 0, len*sizeof(Cat*));
        SeparateColors[i].head=len-1;
        SeparateColors[i].tail=0;
    }
    for (int i=0;i<n;i++){
        int ColorIndex=bsearchArr(ColorMap, numberofColors, SortArr[i]->b);
        SeparateColors[ColorIndex].Array[SeparateColors[ColorIndex].tail]=SortArr[i];
        SeparateColors[ColorIndex].tail=(SeparateColors[ColorIndex].tail+1)%SeparateColors[ColorIndex].length;
    }
    for (int time=0;time<m;time++){
        int mode;
        if (scanf("%d", &mode)){}
        if (mode==1){
            int c, l, r;
            if (scanf("%d %d %d", &c, &l, &r)){}
            int ColorIndex=bsearchArr(ColorMap, numberofColors, c);
            if (ColorIndex==-1) printf("%d\n", 0);
            else{
                int left=bsearchCirArr_geq(SeparateColors[ColorIndex], l), right=bsearchCirArr_leq(SeparateColors[ColorIndex], r);
                if (left==-1||right==-1) printf("%d\n", 0);
                else printf("%d\n", right-left+1);
            }
        }else if (mode==2){
            int k;
            if (scanf("%d", &k)){}
            Cat* greedy=InputArr[k];
            Cat* victim=greedy->next;
            Cat* greedyprev=greedy->prev;
            greedy->next=victim->next;
            greedy->prev=victim;
            if (victim->next!=NULL) victim->next->prev=greedy;
            victim->next=greedy;
            victim->prev=greedyprev;
            if (greedyprev!=NULL) greedyprev->next=victim;
            if (victim==MaxappetiteCat) MaxappetiteCat=greedy;
            if (greedy==MinappetiteCat) MinappetiteCat=victim;
            if (greedy->b==victim->b){
                int ColorIndex=bsearchArr(ColorMap, numberofColors, greedy->b);
                int greedyIndex=bsearchCirArr_byappetite(SeparateColors[ColorIndex], greedy->a);
                Cat* temp2=SeparateColors[ColorIndex].Array[greedyIndex];
                SeparateColors[ColorIndex].Array[greedyIndex]=SeparateColors[ColorIndex].Array[(greedyIndex+1)%SeparateColors[ColorIndex].length];
                SeparateColors[ColorIndex].Array[(greedyIndex+1)%SeparateColors[ColorIndex].length]=temp2;
            }
            int temp1=greedy->a;
            greedy->a=victim->a;
            victim->a=temp1;
        }else{
            int c, s, t;
            if (scanf("%d %d %d", &c, &s, &t)){}
            int ColorIndex=bsearchArr(ColorMap, numberofColors, c);
            if (s==1){
                if (t==1){
                    int highest=(SeparateColors[ColorIndex].tail-1+SeparateColors[ColorIndex].length)%SeparateColors[ColorIndex].length;
                    SeparateColors[ColorIndex].Array[highest]->a=MaxappetiteCat->a+1;
                    if (SeparateColors[ColorIndex].Array[highest]==MinappetiteCat) MinappetiteCat=SeparateColors[ColorIndex].Array[highest]->next;
                    if (SeparateColors[ColorIndex].Array[highest]==MaxappetiteCat) MaxappetiteCat=SeparateColors[ColorIndex].Array[highest]->prev;
                    deletenode(SeparateColors[ColorIndex].Array[highest]);
                    insertnode(MaxappetiteCat, SeparateColors[ColorIndex].Array[highest], NULL);
                    MaxappetiteCat=SeparateColors[ColorIndex].Array[highest];
                }else{
                    int highest=(SeparateColors[ColorIndex].tail-1+SeparateColors[ColorIndex].length)%SeparateColors[ColorIndex].length;
                    Cat* temp=SeparateColors[ColorIndex].Array[highest];
                    SeparateColors[ColorIndex].tail=(SeparateColors[ColorIndex].tail-1+SeparateColors[ColorIndex].length)%SeparateColors[ColorIndex].length;
                    int lowest=(SeparateColors[ColorIndex].head+1)%SeparateColors[ColorIndex].length;
                    temp->a=MinappetiteCat->a-1;
                    SeparateColors[ColorIndex].Array[SeparateColors[ColorIndex].head]=temp;
                    SeparateColors[ColorIndex].head=(SeparateColors[ColorIndex].head-1+SeparateColors[ColorIndex].length)%SeparateColors[ColorIndex].length;
                    if (temp==MaxappetiteCat) MaxappetiteCat=temp->prev;
                    if (temp==MinappetiteCat) MinappetiteCat=temp->next;
                    deletenode(temp);
                    insertnode(NULL, temp, MinappetiteCat);
                    MinappetiteCat=temp;
                }
            }else{
                if (t==0){
                    int lowest=(SeparateColors[ColorIndex].head+1)%SeparateColors[ColorIndex].length;
                    SeparateColors[ColorIndex].Array[lowest]->a=MinappetiteCat->a-1;
                    if (SeparateColors[ColorIndex].Array[lowest]==MaxappetiteCat) MaxappetiteCat=SeparateColors[ColorIndex].Array[lowest]->prev;
                    if (SeparateColors[ColorIndex].Array[lowest]==MinappetiteCat) MinappetiteCat=SeparateColors[ColorIndex].Array[lowest]->next;
                    deletenode(SeparateColors[ColorIndex].Array[lowest]);
                    insertnode(NULL, SeparateColors[ColorIndex].Array[lowest], MinappetiteCat);
                    MinappetiteCat=SeparateColors[ColorIndex].Array[lowest];
                }else{
                    int lowest=(SeparateColors[ColorIndex].head+1)%SeparateColors[ColorIndex].length;
                    Cat* temp=SeparateColors[ColorIndex].Array[lowest];
                    SeparateColors[ColorIndex].head=(SeparateColors[ColorIndex].head+1)%SeparateColors[ColorIndex].length;
                    int highest=(SeparateColors[ColorIndex].tail-1+SeparateColors[ColorIndex].length)%SeparateColors[ColorIndex].length;
                    temp->a=MaxappetiteCat->a+1;
                    SeparateColors[ColorIndex].Array[SeparateColors[ColorIndex].tail]=temp;
                    SeparateColors[ColorIndex].tail=(SeparateColors[ColorIndex].tail+1)%SeparateColors[ColorIndex].length;
                    if (temp==MinappetiteCat) MinappetiteCat=temp->next;
                    if (temp==MaxappetiteCat) MaxappetiteCat=temp->prev;
                    deletenode(temp);
                    insertnode(MaxappetiteCat, temp, NULL);
                    MaxappetiteCat=temp;
                }
            }
        }
    }
    return 0;
}
