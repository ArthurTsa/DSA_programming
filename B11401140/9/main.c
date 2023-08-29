#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define Prime1 646216567629137
#define Prime2 100001617

typedef struct rotation{
    long long mod1, mod2;
}Rotation;
Rotation hashvalue[1000100];
int index=0;

int binarysearch(long long temp1, long long temp2){
    int left=0, right=index-1;
    while (left<=right){
        int mid=(left+right)/2;
        if (hashvalue[mid].mod1==temp1){
            if (hashvalue[mid].mod2==temp2) return 1;
            else{
                int find_index=mid-1;
                while (find_index>=0){
                    if (hashvalue[find_index].mod1==temp1){
                        if (hashvalue[find_index].mod2==temp2) return 1;
                        else find_index--;
                    }else{
                        break;
                    }
                }
                find_index=mid+1;
                while (find_index<index){
                    if (hashvalue[find_index].mod1==temp1){
                        if (hashvalue[find_index].mod2==temp2) return 1;
                        else find_index++;
                    }else{
                        break;
                    }
                }
                return 0;
            }
        }else if (hashvalue[mid].mod1<temp1){
            left=mid+1;
        }else{
            right=mid-1;
        }
    }
    return 0;
}

int compare(const void* a, const void* b){
    Rotation* p1=(Rotation*)a;
    Rotation* p2=(Rotation*)b;
    if (p1->mod1>p2->mod1) return 1;
    else if (p1->mod1<p2->mod2) return -1;
    else return 0;
}

int main(){
    int n, m;
    scanf("%d %d", &n, &m);
    char *S=(char*)malloc((n+1)*sizeof(char)), *P0=(char*)malloc((m+1)*sizeof(char));
    scanf("%s %s", S, P0);
    char *P=(char*)malloc((2*m+1)*sizeof(char));
    P[0]='\0';
    strcat(P, P0);
    strcat(P, P0);

    long long temp1=0, multiply1=1;
    long long temp2=0, multiply2=1;
    for (int i=m-1;i>=0;i--){
        temp1+=(((int)(P[i]-'A')+1)*multiply1);
        while (temp1<0) temp1+=Prime1;
        temp1%=Prime1;
        if (i>0) multiply1*=26;
        multiply1%=Prime1;

        temp2+=(((int)(P[i]-'A')+1)*multiply2);
        while (temp1<0) temp2+=Prime2;
        temp2%=Prime2;
        if (i>0) multiply2*=26;
        multiply2%=Prime2;
    }
    hashvalue[index].mod1=temp1;
    hashvalue[index].mod2=temp2;
    index++;

    for (int i=m;i<2*m;i++){
        temp1=26*(temp1-multiply1*((int)(P[i-m]-'A')+1))+(int)(P[i]-'A')+1;
        while (temp1<0) temp1+=Prime1;
        temp1%=Prime1;
        hashvalue[index].mod1=temp1;

        temp2=26*(temp2-multiply2*((int)(P[i-m]-'A')+1))+(int)(P[i]-'A')+1;
        while (temp2<0) temp2+=Prime2;
        temp2%=Prime2;
        hashvalue[index].mod2=temp2;

        index++;
    }
    qsort(hashvalue, index, sizeof(Rotation), compare);

    int ans=0;
    temp1=0, temp2=0;
    multiply1=1, multiply2=1;
    for (int i=m-1;i>=0;i--){
        temp1+=(((int)(S[i]-'A')+1)*multiply1);
        while (temp1<0) temp1+=Prime1;
        temp1%=Prime1;
        if (i>0) multiply1*=26;
        multiply1%=Prime1;

        temp2+=(((int)(S[i]-'A')+1)*multiply2);
        while (temp2<0) temp2+=Prime2;
        temp2%=Prime2;
        if (i>0) multiply2*=26;
        multiply2%=Prime2;
    }
    if (binarysearch(temp1, temp2)) ans++;
    for (int i=m;i<n;i++){
        temp1=26*(temp1-multiply1*((int)(S[i-m]-'A')+1))+(int)(S[i]-'A')+1;
        while (temp1<0) temp1+=Prime1;
        temp1%=Prime1;

        temp2=26*(temp2-multiply2*((int)(S[i-m]-'A')+1))+(int)(S[i]-'A')+1;
        while (temp2<0) temp2+=Prime2;
        temp2%=Prime2;

        if (binarysearch(temp1, temp2)) ans++;
    }
    printf("%d", ans);
    return 0;
}
