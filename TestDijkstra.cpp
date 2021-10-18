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
//////////////head
const int N=2E6+10;                                     //只有362880种状态，每种状态最多可以向4个方向转移,4*362880=1,451,520，N为2E6则可。

//Part 1:----------------------------------------------//dijkstra计算单源最短路
int n,vis[N],dis[N];                                  //vis[i]判断i点是否已经获得过最短路，dis[i]表示i点到源点的最短路径
vector<int>store[N];                                 //vector存邻接表，store[i]=j表示i下一个状态可到达j
struct Node{
    int dis,pos;
    bool operator <(const Node &x)const{
        return x.dis<dis;                         //节点返回更小的距离，用于二叉堆的排序
    }
};
priority_queue<Node>q;                          //优先队列在队首总是队列中dis最小的节点
int Source,End,fa[N];                          //Source为源点，ed为终点，fa用于记录路径，从终点沿着fa回溯到起始点。
stack<int>path;                               //path用于保存路径，用栈可以正序输出最终变换步数
bool dijkstra(){
    q.push({0,Source});                     
    dis[Source] = 0;
    while(!q.empty()){
        Node cur=q.top();                 //每次都取出队列中到达源点距离最短的点（优先队列会自动对队列排序）
        if(cur.pos==End)return true;     //若已经找到路径到达终点直接返回
        q.pop();
        if(vis[cur.pos])continue;
        vis[cur.pos]=1;
        for(int i=0;i<store[cur.pos].size();i++){
            int v=store[cur.pos][i];
            if(dis[v]>dis[cur.pos]+1&&!vis[v]){     //若v状态从cur状态转移所需步数更少，则让v从cur转移
                fa[v]=cur.pos;
                dis[v]=dis[cur.pos]+1;
                q.push(Node{dis[v],v});
            }
        }
    }
    return false;
}


//Part 2:----------------------------------------//处理序号和图之间的关系
map<int ,int>reflect;                           //reflect[i]=j:i保存整个图，有0位数，j相当于i的序号。i从小到大按顺序存储。
int dx[4]={-1,0,0,1},dy[4]={0,-1,1,0};         //dx表示行的移动，dy表示列的移动
int power10[10];                              //保存10的幂次，减少重复计算
int match[N];                                //match[i]=j:第i个位置保存的是j图

void pre(){
    n=362880;
    //利用c++内置函数next_permutation,获得0-8全排列。将012345678,012345687,...,876543210映射到1,2,...,362880
    int pos=0;
    int a[10]={0,1,2,3,4,5,6,7,8};
    do{
        int sum=0;
        for(int i=0;i<=8;i++){
            sum=sum*10+a[i];
        }match[++pos]=sum,reflect[sum]=pos;
    }while(next_permutation(a,a+9));

    //获得10的1-9的幂次结果
    power10[0]=1;
    for(int i=1;i<=9;i++)power10[i]=power10[i-1]*10;

    End=reflect[123456780];
}

//用序号寻找0的位置，传入序号，返回0在表格中的位置
PII ReflectToZeroPosition(int i){
    PII GridPosition;                               //GridPosition保存序号对应的图中0所在的位置(行，列）
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

//如果当前的状态CurState可以向某个方向direction(0,1,2,3分别表示上，左，右，下）转移
//就直接返回当前状态向该方向转移后的状态，否则返回-1
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

//初始化
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
        bool PathExist=dijkstra();
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
    printf("\nThe duration of Dijkstra: %d\n",duration);
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