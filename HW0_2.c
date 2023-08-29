#include <stdio.h>
int n, m;
int rule[30][30];
char puzzle[25][25];

void printpuzzle(){
	for (int i=0;i<n;i++){
		for (int j=0;j<m;j++){
			printf("%c", puzzle[i][j]);
		}
		printf("%c", '\n');
	}
}

int final_board_is_valid(){
	for (int j=0;j<m;j++){
		int segnum=0, seg[30]={};
		int nowwhite=1;
		for (int i=0;i<n;i++){
			if (nowwhite && puzzle[i][j]=='o'){
				nowwhite=0;
				segnum++;
				seg[segnum-1]++;
			}else if (!nowwhite && puzzle[i][j]=='o') seg[segnum-1]++;
			else if (!nowwhite && puzzle[i][j]=='_') nowwhite=1;
		}
		if (segnum!=rule[n+j][0]) return 0;
		for (int k=0;k<segnum;k++){
			if (seg[k]!=rule[n+j][k+1]) return 0;
		}
	}
	return 1;
}

int if_row_valid(int row){
	int segnum=0, seg[30]={};
	int nowwhite=1;
	for (int j=0;j<m;j++){
		if (nowwhite && puzzle[row][j]=='o'){
			nowwhite=0;
			segnum++;
			seg[segnum-1]++;
		}else if (!nowwhite && puzzle[row][j]=='o') seg[segnum-1]++;
		else if (!nowwhite && puzzle[row][j]=='_') nowwhite=1;
	}
	if (segnum!=rule[row][0]) return 0;
	for (int k=0;k<segnum;k++){
		if (seg[k]!=rule[row][k+1]) return 0;
	}
	return 1;
}

int if_cur_col_valid(int x, int y){
	int blacknum=0, real_blacknum=0;
	for (int i=0;i<=x;i++){
		if (puzzle[i][y]=='o') blacknum++; 
	}
	for (int i=1;i<=rule[n+y][0];i++){
		real_blacknum+=rule[n+y][i];
	}
	if (blacknum>real_blacknum) return 0;
	else return 1;
}

int Recur(int index){
	int x=index/m, y=index%m;
	if (index==m*n-1){
		puzzle[x][y]='o';
		if (if_row_valid(x)){
			if (final_board_is_valid()){
				printpuzzle();
				return 1;
			}
		}
		puzzle[x][y]='_';
		if (if_row_valid(x)){
			if (final_board_is_valid()){
				printpuzzle();
				return 1;
			}
		}
		return 0;
	}else{
		puzzle[x][y]='o';
		if (if_cur_col_valid(x, y)){
			if (index%m==m-1){
				if (if_row_valid(x)){
					int succeed=Recur(index+1);
					if (succeed) return 1;
				}
			}else{
				int succeed=Recur(index+1);
				if (succeed) return 1;
			}
		}
		puzzle[x][y]='_';
		if (if_cur_col_valid(x, y)){
			if (index%m==m-1){
				if (if_row_valid(x)){
					int succeed=Recur(index+1);
					if (succeed) return 1;
				}
			}else{
				int succeed=Recur(index+1);
				if (succeed) return 1;
			}
		}
		return 0;
	}
}

int main(){
	scanf("%d %d", &n, &m);
	for (int i=0;i<n+m;i++){
		scanf("%d", &rule[i][0]);
		for (int j=1;j<=rule[i][0];j++){
			scanf("%d", &rule[i][j]);
		}
	}
	Recur(0);
	return 0;
} 
