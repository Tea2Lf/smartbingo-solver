
//	bingo solver
//  version: 0.1 
//	dev by Tea2Lf (6567D)
//	glhf!

#include<iostream>
#include<vector>
#include<iomanip>
#include<queue>
#include<cmath>
using namespace std;
const int N=5;
vector<vector<int>> mp(5,vector<int>(5,0));

vector<int> get_ticks(int state){
	return {
		(state&0x1f00000)>>20,
		(state&0x00f8000)>>15,
		(state&0x0007c00)>>10,
		(state&0x00003e0)>>5,
		state&0x000001f
	};
}

bool check_row(int state){
	vector<int> ticks=get_ticks(state);
	return ticks[0]==0x1f||ticks[1]==0x1f||ticks[2]==0x1f||ticks[3]==0x1f||ticks[4]==0x1f;
}

bool check_col(int state){
	vector<int> ticks=get_ticks(state);
	return ticks[0]&ticks[1]&ticks[2]&ticks[3]&ticks[4];
}

const int fir=0x1041041;
const int sec=0x0111110;
bool check_dia(int state){
	return (state&fir)>=fir||(state&sec)>=sec;
}

const int dx[]={0,-1,0,1};
const int dy[]={1,0,-1,0};
const int exdx[]={-1,-1,1,1};
const int exdy[]={1,-1,-1,1};
bool check_mp(int state){
	bool ret=1;
	vector<vector<bool>> ticks(5,vector<bool>(5,0));
	
	for(int i=5-1;i>=0;i--)
		for(int j=5-1;j>=0;j--)
			ticks[i][j]=state&1,
			state>>=1;
	
	for(int i=0;i<5;i++)
		for(int j=0;j<5;j++)
			if(mp[i][j]){
				int cnt_aro=0,cnt_row=0,cnt_col=0,cnt_dia_p=0,cnt_dia_n=0;
				
				if(mp[i][j]==1||mp[i][j]==2||mp[i][j]==6||mp[i][j]==7||mp[i][j]==8||mp[i][j]==9)
					for(int k=0;k<4;k++){
						int xx=i+dx[k];
						int yy=j+dy[k];
						if(0<=xx&&xx<5&&0<=yy&&yy<5)
							cnt_aro+=ticks[xx][yy];
					}
				
				if(mp[i][j]==1||mp[i][j]==2||mp[i][j]==6||mp[i][j]==7)
					for(int k=0;k<4;k++){
						int xx=i+exdx[k];
						int yy=j+exdy[k];
						if(0<=xx&&xx<5&&0<=yy&&yy<5)
							cnt_aro+=ticks[xx][yy];
					}
				
				if(mp[i][j]==4)
					for(int k=0;k<5;k++)
						cnt_row+=ticks[i][k],
						cnt_col+=ticks[k][j];
				
				if(mp[i][j]==5)
					for(int k=0;k<5;k++)
						for(int l=0;l<5;l++){
							if(k+l==i+j)
								cnt_dia_p+=ticks[k][l];
							if(k-l==i-j)
								cnt_dia_n+=ticks[k][l];
						}
				
				vector<bool> pass={
					1,
					cnt_aro>=1,
					cnt_aro<=2,
					ticks[i][j],
					cnt_row==cnt_col,
					cnt_dia_p==cnt_dia_n,
					(cnt_aro&1)==0,
					cnt_aro&1,
					ticks[i][j]&&cnt_aro==0||!ticks[i][j],
					ticks[i][j]&&cnt_aro>0||!ticks[i][j]
				};
				
				if(pass[mp[i][j]]==0)
					return 0;
			}
	
	return 1;
}

bool check_ans(int state){
	return (check_row(state)||check_col(state)||check_dia(state))&&check_mp(state);
}

int ticks_cnt(int state){
	int ret=0;
	while(state)
		ret++,
		state^=(state&-state);
	return ret;
}

void print_solution(int state){
	int cnt=25;
	for(int i=0;i<5;i++){
		for(int j=0;j<5;j++)
			cout<<(((state&(1<<(--cnt)))>0)?'#':'.');
		cout<<endl;
	}		
}

int main(){
	
	cout<<"Thank you so much to use bingo_solver by Tea2Lf"<<endl;
	cout<<"Please input the map"<<endl;
	cout<<"There's the rule list:"<<endl;
	cout<<". empty / R red / b blue / B black / G green"<<endl;
	cout<<"Y yellow / O orange / P purple / p pink / C cyan"<<endl; 
	cout<<endl<<"--------------------------"<<endl<<endl;
	
	vector<int> trans(128,0);
	trans['.']=0;trans['R']=1;trans['b']=2;trans['B']=3;trans['G']=4;
	trans['Y']=5;trans['O']=6;trans['P']=7;trans['p']=8;trans['C']=9;
	vector<vector<char>> ch(5,vector<char>(5,'.'));
	for(int i=0;i<5;i++)
		for(int j=0;j<5;j++)
			cin>>ch[i][j],
			mp[i][j]=trans[ch[i][j]];
	
	cout<<endl<<"--------------------------"<<endl;
	cout<<endl<<"It's solving now, please wait..."<<endl; 
	int min_ans=26;//!?MrZ?!
	int max_ans=4;
	vector<int> solutions={};
	for(int state=0;state<(1<<25);state++){
		if(check_ans(state)){
			int cnt=ticks_cnt(state);
			min_ans=min(min_ans,cnt);
			max_ans=max(max_ans,cnt);
			solutions.push_back(state);
		}
		if(((state+1)&0x1fffff)==0)
			cout<<fixed<<setprecision(2)<<((double)(state)/335544.31)<<"%..."<<endl; 
	}
		
	
	cout<<endl<<"--------------------------"<<endl;
	cout<<"Checked "<<(1<<25)<<" states"<<endl; 
	cout<<"Found "<<solutions.size()<<" solutions"<<endl<<endl;
	
	bool exist_solution=solutions.size()>0;
	if(exist_solution){
		if(solutions.size()==1)
			print_solution(solutions[0]);
		else{
			cout<<"Min ticks count: "<<min_ans<<endl;
			cout<<"Min state solution: "<<endl;
			for(int state:solutions)
				if(ticks_cnt(state)==min_ans){
					print_solution(state);
					break; 
				}
			cout<<endl;
			
			cout<<"Max ticks count: "<<max_ans<<endl;
			cout<<"Max state solution: "<<endl;
			for(int i=solutions.size()-1;i>=0;i--)
				if(ticks_cnt(solutions[i])==max_ans){
					print_solution(solutions[i]);
					break; 
				}
			cout<<endl;
			
			if(solutions.size()>2){
				cout<<"Min state solution of all: "<<endl;
				print_solution(solutions[0]);
				cout<<endl;
				cout<<"Max state solution of all: "<<endl;
				print_solution(solutions[solutions.size()-1]);
				cout<<endl; 
			}
		}
	}
	
	int bingogo=-2147483648;
	
	if(exist_solution){
		vector<int> cnt(10,0);
		for(int i=0;i<5;i++)
			for(int j=0;j<5;j++)
				++cnt[mp[i][j]];
		int cnt_color=0;
		for(int i=0;i<=9;i++)
			if(cnt[i])
				++cnt_color;
		
		int cnt_block=0;
		vector<vector<bool>> vis(5,vector<bool>(5,0));
		queue<pair<int,int>> q;
		auto bfs=[&q,&vis](int x,int y){
			q.push({x,y});
			vis[x][y]=1;
			int ret=1;
			while(!q.empty()){
				pair<int,int> top=q.front();
				q.pop();
				ret++;
				int tx=top.first;
				int ty=top.second;
				int tr=mp[tx][ty];
				for(int i=0;i<5;i++)
					for(int j=0;j<5;j++){
						bool e=0;
						int nr=mp[i][j];
						if(nr==0)continue;
						if(tr==1||tr==2||tr==6||tr==7||tr==8||tr==9)
							for(int k=0;k<4;k++){
								int xx=x+dx[k];
								int yy=y+dy[k];
								if(xx==i&&yy==j)
									e=1;
							}
						if(tr==1||tr==2||tr==6||tr==7)
							for(int k=0;k<4;k++){
								int xx=x+exdx[k];
								int yy=y+exdy[k];
								if(xx==i&&yy==j)
									e=1;
							}
						if(tr==4||nr==4)
							e=e||(i==tx)||(j==ty);
						if(tr==5||nr==5)
							e=e||(i+j==tx+ty)||(i-j==tx-ty);
						if(e&&!vis[i][j])
							vis[i][j]=1,
							q.push({i,j});
					} 
			}
			return ret;
		};
		for(int i=0;i<5;i++)
			for(int j=0;j<5;j++)
				if(mp[i][j]&&!vis[i][j])
					cnt_block=max(cnt_block,bfs(i,j));
		
		const int MAX=9717109;
		bingogo=(
			0*cnt[0]+1*cnt[1]+2*cnt[2]+(-1)*cnt[3]+5*cnt[4]+
			5*cnt[5]+3*cnt[6]+3*cnt[7]+1*cnt[8]+1*cnt[9]
		)*(cnt_block/cnt_color+1)*(min_ans-4)*(log10(MAX/solutions.size())+1);
	}
	
	cout<<"Your bingo gets "<<bingogo<<" bingogo!"<<endl<<endl;
	
	cout<<"Do you want to share your bingo? 0:No / 1:Yes"<<endl;
	bool share;
	cin>>share;
	if(share){
		cout<<endl;
		for(int i=0;i<5;i++){
			for(int j=0;j<5;j++)
				cout<<ch[i][j];
			cout<<endl;
		}
		cout<<solutions.size()<<" solution"<<endl;
		cout<<bingogo <<" bingogo!"<<endl;
		cout<<endl; 
	}
	
	cout<<"Thanks for using!"; 
	return 0;
} 
