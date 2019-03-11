#include<iostream>
#include<fstream>
using namespace std;

struct BLOCK{
	bool VTIAL;   //是否为垂直块 
	char SYMBOL;  //符号 
	int LEN;      //长度
	int x,y;      //位置 
}; 

struct SEEK_NODE{
	string STATUE;
	int steps;
};

/* 为了方便，使用全局变量 */
BLOCK block[20];
int key; 
int Block_count; 
/*   */
int MINSTEP,stdcount;
string MINANS;
SEEK_NODE node[100005];

string INPUT_GAME();
void SOLVE_REC(string MOVE, string STAT);
void graph(); 
void print(const string &move_ans);

int main(){
	int i;
	for (i=0; i<20; i++){
		block[i].LEN = 0;
		block[i].SYMBOL = ' ';
		block[i].VTIAL = false;
		block[i].x = 0;
		block[i].y = 0;
	}
	string STR = INPUT_GAME();
	
	MINANS  = "";
	stdcount = 0;
	for (int i=0; i<10000; i++){
		node[i].STATUE = "";
		node[i].steps = 0;
	}
	
	/*node[0].STATUE = STR;
	cout << STR << endl;
	for (i=0; i<Block_count; i++)
		cout << i << ":'" << block[i].SYMBOL <<"' " << block[i].LEN <<" " << block[i].x << "," << block[i].y << endl;  
	cout << key<< endl;	*/

	//graph();
	SOLVE_REC(MINANS, STR);
	
	//cout << MINANS << endl;	
	//cout << MINSTEP << endl;
	print(MINANS);
	cout << endl;
	system("PAUSE");
	//string ANS_MODE = SOLVE_REC() 
}

string INPUT_GAME(){
	/* 关联文件，读入map */
	ifstream input;
	string STR = "";
	string ANS = "";
	string TP;
	int i,j;
	
	input.open("input.txt"); 
	input >> MINSTEP; MINSTEP++;
	for (i=0; i<6; i++){
		input >> TP;
		STR = STR + TP;
	}
	Block_count = 0;
	for (i=0; i<36; i++)
		if (STR[i]!='0'){
			if (((i>0) && (STR[i]!=STR[i-1]) || (i==0)) && 
			    ((i>5) && (STR[i]!=STR[i-6]) || (i<=5)) ){
				block[Block_count].SYMBOL = STR[i];
				if (STR[i]==STR[i+1]){
					block[Block_count].VTIAL = false;
					block[Block_count].LEN = 1;
					while (STR[i]==STR[i+block[Block_count].LEN]) 
						block[Block_count].LEN++;
					block[Block_count].x = i / 6;
					block[Block_count].y = i % 6;
				}else{
					block[Block_count].VTIAL = true;
					block[Block_count].LEN = 1;
					while ((i+6*block[Block_count].LEN<36) && 
						   (STR[i]==STR[i+6*block[Block_count].LEN])) 
						block[Block_count].LEN++;
					block[Block_count].x = i / 6;
					block[Block_count].y = i % 6;
				}
				if (block[Block_count].VTIAL)
					ANS = ANS  + char(block[Block_count].x+48);
				else
					ANS = ANS  + char(block[Block_count].y+48);
				Block_count++;
			}
		}
	
	key = -1;
	for (i=0; i<Block_count; i++){
		if (!block[i].VTIAL && block[i].x==2){
			if (key==-1)
				key = i;
			else
				if (block[key].y < block[i].y)
					key = i;	
		}	
	}
	input.close();
	return ANS;
}

// ASCII('A') = 65     ASCII('0') = 48
//       49  50  51  52  53  54  55  56  57
// 规定  1   2   3   4   5   6   7   8   9
// 移动  -4 -3  -2  -1   0   1   2   3   4

void SOLVE_REC(string MOVE, string STAT){
	int i,j;
	int step = (MOVE.length()+1)/2;
	
	if (step>=MINSTEP || stdcount> 100000) return;
	
	if (STAT[key]=='4'){
		MINSTEP = step;
		MINANS = MOVE;
		return;
	}
	
	//cout << "当前步数：" << step  << endl;
	for (i=stdcount; i>=0; i--){
		//cout << "检查是否有更优解中！" << i << "/" << stdcount << endl;
		
		if (STAT==node[i].STATUE && step>=node[i].steps)
			return;
		else continue;
		}
	//cout <<"达到状态为" << STAT <<"的最小步数更新为" << step << endl;
	stdcount++;
	node[stdcount].STATUE = STAT;
	node[stdcount].steps  = step;

	
	
	bool map[6][6];
	int pos;
	for (i=0; i<6; i++)
		for (j=0; j<6; j++)
			map[i][j] = false;
	for (i=0; i<Block_count; i++){
		pos = int(STAT[i])-48;
		if (block[i].VTIAL){
			for (j=0; j<block[i].LEN; j++)
				map[pos+j][block[i].y] = true;
		}
		else{
			for (j=0; j<block[i].LEN; j++)
				map[block[i].x][pos+j] = true;
		}			
	}
	string TEMP_MOVE,TEMP_STAT;
	int l;
	for (i=0; i<Block_count; i++){
		l = block[i].LEN;
		pos = int(STAT[i]-48);	
		
		if (block[i].VTIAL){
			for (j=0; j<block[i].LEN; j++)
				map[pos+j][block[i].y] = false;
		}
		else{
			for (j=0; j<block[i].LEN; j++)
				map[block[i].x][pos+j] = false;
	    }
		
		if (block[i].VTIAL){
			for (j=1; j<=4; j++){
			if (pos+j+l-1>=6 || map[pos+j+l-1][block[i].y]) break;
			TEMP_MOVE = MOVE + char(i+48) + char(j+53);
			TEMP_STAT = STAT;
			TEMP_STAT[i] = char(int(TEMP_STAT[i])+j); 
			SOLVE_REC(TEMP_MOVE, TEMP_STAT);
			}				
		for (j=1; j<=4; j++){
			if (pos-j<0 || map[pos-j][block[i].y]) break;
			TEMP_MOVE = MOVE + char(i+48) + char(53-j);
			TEMP_STAT = STAT;
			TEMP_STAT[i] = char(int(TEMP_STAT[i])-j); 
			SOLVE_REC(TEMP_MOVE, TEMP_STAT);			
			}	
		}
		else{
			for (j=1; j<=4; j++){
			if (pos+j+l-1>=6 || map[block[i].x][pos+j+l-1]) break;
			TEMP_MOVE = MOVE + char(i+48) + char(j+53);
			TEMP_STAT = STAT;
			TEMP_STAT[i] = char(int(TEMP_STAT[i])+j); 
			SOLVE_REC(TEMP_MOVE, TEMP_STAT);
			}				
		for (j=1; j<=4; j++){
			if (pos-j<0 || map[block[i].x][pos-j]) break;
			TEMP_MOVE = MOVE + char(i+48) + char(53-j);
			TEMP_STAT = STAT;
			TEMP_STAT[i] = char(int(TEMP_STAT[i])-j); 
			SOLVE_REC(TEMP_MOVE, TEMP_STAT);			
			}	
		}
			
		if (block[i].VTIAL){
			for (j=0; j<block[i].LEN; j++)
				map[pos+j][block[i].y] = true;
		}
		else{
			for (j=0; j<block[i].LEN; j++)
				map[block[i].x][pos+j] = true;
		}
	}
}

void graph(){
	int i,j,x,y;
	char map[6][6];
	for (i=0; i<36; i++)
		map[i/6][i%6] = ' ';
	for (i=0; i<Block_count; i++){
		if (block[i].VTIAL){
			for (j=0; j<block[i].LEN; j++)
				map[block[i].x+j][block[i].y] = block[i].SYMBOL;
		}
		else{
			for (j=0; j<block[i].LEN; j++)
				map[block[i].x][block[i].y+j] = block[i].SYMBOL;
		}
	}
	
	for (i=0; i<36; i++){
		x = i / 6;
		y = i % 6;
		if (y==0) cout << endl;
		if (map[x][y]==' ')
			cout << "○"; 
		else if (y>0 && y<5 && map[x][y]==map[x][y+1] && map[x][y]==map[x][y-1])
			cout << "━ ";
		else if (y<5 && map[x][y]==map[x][y+1])
			cout << "┣ ";
		else if (y>0 && map[x][y]==map[x][y-1])
			cout << "┫ ";
		else if (x>0 && x<5 && map[x][y]==map[x+1][y] && map[x][y]==map[x-1][y])
			cout << "┃ ";
		else if (x<5 && map[x][y]==map[x+1][y])
			cout << "┳ ";
		else if (x>0 && map[x][y]==map[x-1][y])
			cout << "┻ ";
		else 
			cout << "○";
	}
}

void print(const string &move_ans){
	 int i,step,bk;
	 step = (move_ans.length()+1)/2;
	 for (i=0; i<step; i++){
	 	cout << endl << endl << "第 " << i+1 << " 步";
	 	bk = int(move_ans[i*2])-48;
	 	if (block[bk].VTIAL){
			block[bk].x += int(move_ans[i*2+1])-48-5;
		}
		else{
			block[bk].y += int(move_ans[i*2+1])-48-5;
		}
	  graph();
	}
}
