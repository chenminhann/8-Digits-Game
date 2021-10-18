#include<algorithm>
#include<cstdio>
#include<time.h>
#include<iostream>
#include<map>
#include<stack>
#include<vector>
#include<queue>
using namespace std;
#define inf 0x3f3f3f3f
typedef pair<int,int> PII;
const int N=2E6+10;                             
int n,vis[N],dis[N];                             
vector<int>store[N];                             
queue<int>q;                   
int Source,End,fa[N];                    
stack<int>path;                          
bool bfs(){
    q.push(Source);                  
    dis[Source] = 0;
    while(!q.empty()){
        int cur=q.front();             
        if(cur==End)return true;  
        q.pop();
        if(vis[cur])continue;
        vis[cur]=1;
        for(int i=0;i<store[cur].size();i++){
            int v=store[cur][i];
            if(dis[v]>dis[cur]+1&&!vis[v]){    
                fa[v]=cur;
                dis[v]=dis[cur]+1;
                q.push(v);
            }
        }
    }
    return false;
}

map<int ,int>reflect;                      
int dx[4]={-1,0,0,1},dy[4]={0,-1,1,0};     
int power10[10];                           
int match[N];                              
void pre(){
    n=362880;
    int pos=0;
    int a[10]={0,1,2,3,4,5,6,7,8};
    do{
        int sum=0;
        for(int i=0;i<=8;i++){
            sum=sum*10+a[i];
        }match[++pos]=sum,reflect[sum]=pos;
    }while(next_permutation(a,a+9));
    power10[0]=1;
    for(int i=1;i<=9;i++)power10[i]=power10[i-1]*10;

    End=reflect[123456780];
}
PII ReflectToZeroPosition(int i){
    PII GridPosition;                              
    int tmp=match[i];
    int ArrayPosition=0;
    while(tmp%10!=0){
        tmp/=10;
        ArrayPosition++;
    }
    ArrayPosition=8-ArrayPosition;
    GridPosition={ArrayPosition/3,ArrayPosition%3};
    return GridPosition;
}
int CheckTransform(int CurState,int direction){ 
    PII CurPosition=ReflectToZeroPosition(CurState);
    int next_x=CurPosition.first+dx[direction],next_y=CurPosition.second+dy[direction];
    if(next_x>-1&&next_x<3&&next_y>-1&&next_y<3){
        int NextDigit=next_x*3+next_y,CurDigit=CurPosition.first*3+CurPosition.second;
        int tmp=match[CurState],num=tmp/power10[8-NextDigit]%10;
        tmp=tmp-num*power10[8-NextDigit]+num*power10[8-CurDigit];
        return reflect[tmp];
    }
    return -1;
}
void OutputGrid(int i){
    int grid=match[i];
    for(int i=8;i>-1;i--){
        printf("%d ",grid/power10[i]%10);
        if(i==6||i==3||i==0)
            printf("\n");
    }
}
void initial(){
    for(int i=0;i<N-1;i++){
        vis[i]=0,dis[i]=inf,fa[i]=0;
    }
    while(q.size())q.pop();
}
vector<PII>Answer;
vector<int>NoWay;
clock_t start,stop;
int main(){
    freopen("StoreInput.txt","r",stdin);
    //freopen("StoreDijkstra.txt","w",stdout);
    pre();
    start=clock();
    double AverageStep=0;
    while(true){
        initial();
        scanf("%d",&Source);
        if(Source==-1){
            break;
        }
        int OriginSource=Source;
        Source=reflect[Source];
        for(int i=1;i<=n;i++){
            for(int j=0;j<4;j++){
                int v=CheckTransform(i,j);
                if(v!=-1){store[i].push_back(v);}
            }
        }
        bool PathExist=bfs();
        if(PathExist) {
            printf("%09d %d\n",OriginSource,dis[End]);
            Answer.push_back({OriginSource,dis[End]});
            AverageStep+=dis[End];
        }
        else {
            NoWay.push_back(OriginSource);
            printf("%09d\n",OriginSource);
        }
    }
    stop=clock();
    int duration = ((double)(stop - start)) / CLK_TCK;
    printf("\nThe duration of Dfs: %d\n",duration);
    printf("The least steps needed for %d states:\n",Answer.size());
    for(int i=0;i<Answer.size();i++){
        printf("%09d %d\n",Answer[i].first,Answer[i].second);
    }
    printf("\nThe average steps are %.lf\n",1.0*AverageStep/Answer.size());

    printf("\nThere are %d states that can not reach the victory:\n",NoWay.size());
    for(int i=0;i<NoWay.size();i++){
        printf("%09d\n",NoWay[i]);
    }
    return 0;
}