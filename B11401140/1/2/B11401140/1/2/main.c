#include <stdio.h>
#include <string.h>
int n, m;
int rule[10][4]={};
char puzzle[5][5];

void printpuzzle(){
	for (int i=0;i<n;i++){
		for (int j=0;j<m;j++){
			printf("%c", puzzle[i][j]);
		}
		printf("%c", '\n');
	}
}

int final_board_is_valid(){
	//rule part two
	for (int j=0;j<m;j++){
		int segnum=0, seg[3]={};
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
	
	//consecutive
	int dici[8]={1, 1, 0, -1, -1, -1, 0, 1}, dicj[8]={0, 1, 1, 1, 0, -1, -1, -1};
	for (int i=0;i<n;i++){
		for (int j=0;j<m;j++){
			if (puzzle[i][j]=='o'){
				int is_consecutive=0;
				for (int k=0;k<8;k++){
					if (0<=i+dici[k] && i+dici[k]<n && 0<=j+dicj[k] && j+dicj[k]<m){
						if (puzzle[i+dici[k]][j+dicj[k]]=='o'){
							is_consecutive=1;
							break;
						}
					} 
				}
				if (!is_consecutive) return 0;
			}
			
		}
	}
	
	return 1;
}

int if_row_satisfy_rule(int row){
	int segnum=0, seg[3]={};
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

int Recur(int index){
	int x=index/m, y=index%m;
	if (index==m*n-1){
		puzzle[x][y]='o';
		if (if_row_satisfy_rule(x)){
			if (final_board_is_valid()){
				printpuzzle();
				return 1;
			}
		}
		puzzle[x][y]='_';
		if (if_row_satisfy_rule(x)){
			if (final_board_is_valid()){
				printpuzzle();
				return 1;
			}
		}
		puzzle[x][y]='0';
		return 0;
	}else{
		puzzle[x][y]='o';
		if (index%m==m-1){
			if (if_row_satisfy_rule(x)){
				int succeed=Recur(index+1);
				if (succeed) return 1;
			}
		}else{
			int succeed=Recur(index+1);
			if (succeed) return 1;
		}
		puzzle[x][y]='_';
		if (index%m==m-1){
			if (if_row_satisfy_rule(x)){
				int succeed=Recur(index+1);
				if (succeed) return 1;
			}
		}else{
			int succeed=Recur(index+1);
			if (succeed) return 1;
		}
		puzzle[x][y]='0';
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
	memset(puzzle, '0', sizeof(puzzle));
	/*validation
	int in=0;
	for (int i=0;i<n*m+4;i++){
		char c;
		scanf("%c", &c);
		if (i==0 || i==5 || i==10 || i==15) continue;
		puzzle[in/m][in%m]=c;
		in++;
	}
	printpuzzle();*/
	Recur(0);
	return 0;
} 

