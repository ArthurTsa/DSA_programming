#include <stdio.h>
#include <string.h>

int compare(int a[], int b[]){
	for (int i=259;i>=0;i--){
		if (a[i]>b[i]) return 1;
		else if (a[i]<b[i]) return -1;
	}
	return 0;
}

void subtraction(int a[], int b[]){
	for (int i=0;i<260;i++){
		a[i]-=b[i];
		if (a[i]<0){
			a[i+1]--;
			a[i]+=10;
		}
	}
}

void divby2(int a[]){
	int r=0;
	for (int i=259;i>=0;i--){
		r=r*10+a[i];
		a[i]=r/2;
		r%=2;
	}
}

void mulby2k(int a[], int k){
	for (int i=0;i<260;i++){
		a[i]*=k;
		a[i+1]+=(a[i]/10);
		a[i]%=10;
	}
}

int testnot0(int a[]){
	for (int i=0;i<260;i++){
		if (a[i]>0) return 1;
	}
	return 0;
}

void swap(int a[], int b[]){
	int c[260]={};
	for (int i=0;i<260;i++){
		c[i]=a[i];
	}
	for (int i=0;i<260;i++){
		a[i]=b[i];
	}
	for (int i=0;i<260;i++){
		b[i]=c[i];
	}
}

void print(int a[]){
	int not0=0;
	for (int i=259;i>=0;i--){
		if (not0==0 && a[i]==0) continue;
		if (not0==0 && a[i]>0){
			not0=1;
			printf("%d", a[i]);
		}else printf("%d", a[i]);
	} 
}

int main(){
	// input
	int A[260]={}, B[260]={};
	char C[260];
	scanf("%s", C);
	for (int i=0;i<strlen(C);i++){
		A[i]=(int)(C[strlen(C)-i-1]-'0');
	}
	scanf("%s", C);
	for (int i=0;i<strlen(C);i++){
		B[i]=(int)(C[strlen(C)-i-1]-'0');
	}
	
	
	// Binary Algorithm for Greatest Common Divisor
	int n[260]={}, m[260]={}, ans=1;
	if (compare(A, B)>=0){
		for (int i=0;i<260;i++){
			m[i]=A[i];
			n[i]=B[i];
		}
	}else{
		for (int i=0;i<260;i++){
			n[i]=A[i];
			m[i]=B[i];
		}
	}
	while (testnot0(n) && testnot0(m)){
		if (n[0]%2==0 && m[0]%2==0){
			ans*=2;
			divby2(n);
			divby2(m);
		}else if (n[0]%2==0){
			divby2(n);
		}else if (m[0]%2==0){
			divby2(m);
		}
		
		if (compare(n, m)>0) swap(n, m);
		subtraction(m, n);
	} 
	mulby2k(n, ans);
	
	// print out answer
	print(n);
	
	return 0;
}
