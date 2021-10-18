#include<algorithm>
#include<cstdio>
#include<map>
#include<stack>
#include<vector>
#include<queue>
using namespace std;
#define inf 0x3f3f3f3f
#define EdgeLength 1
typedef pair<int,int> PII;
/*
将G设置为当前点到起点的距离 每次更新的时候加上边长EdgeLength表示实际路径长度
将H1设置为不在位的数字数量，H2设置为当前相邻但在目标状态不相邻的数字对数，H=k1*H1+k2*H2
H可在pre中全部算出，在后面的计算则作为常数使用
*/
const int N=2E6+10;int n; double k1=1,k2=1;                 //只有362880种状态，每种状态最多可以向4个方向转移,4*362880=1,451,520，N为2E6则可。
//Part 1:-----------------------------------------//A*算法计算单源最短路
vector<int>store[N];                             //vector存邻接表，store[i]=j表示i下一次可到达j
bool CloseList[N];                              //若CloseList[i]==1,则表示它被放入CloseList中(CloseList中的点无需再使用，因此不必建立CloseList)
struct Node{                                   //每个节点存放每个位置的父亲fa，F,G,H值还有对应的表格中0的位置
    int fa,F,G,H,H1,H2,pos;
    bool operator <(const Node &x)const{     //在OpenList里排序规则为将更小的f放在前面
        return x.F<F;
    }
    bool ChangeFa(const Node &x){
        int TempF=H+EdgeLength+x.G;
        if(TempF<F){
            F=TempF;
            fa=x.pos;
            G=x.G+EdgeLength;
            return true;
        }return false;
    }
}node[N];
priority_queue<Node>OpenList;
int Source,End;                                                  //s为源点，ed为终点，fa用于记录路径，从终点沿着fa回溯到起始点。
stack<int>path;                                                 //path用于保存路径，可以正序输出最终变换步数
bool A_Star(){
    OpenList.push(node[Source]);
    while(!OpenList.empty()){
        Node cur=OpenList.top();                             //每次都取出队列中到达源点距离最短的点（优先队列会自动对队列排序）
        if(cur.pos==End)return true;                        //最终可以找到路径到达终点
        OpenList.pop();CloseList[cur.pos]=true;           //把当前节点从OpenList删除后移动到CloseList中
        for(auto it=store[cur.pos].begin();it!=store[cur.pos].end();it++){
           // cout<<CloseList[*it]<<endl;
            if(CloseList[*it])continue;
            int NextPos=*it;
            if(node[NextPos].ChangeFa(node[cur.pos]))//判断当前更改路径是否比原路径更佳
                OpenList.push(node[NextPos]);
        }
    }
    return false;
}

//Part 2:-----------------------------------------//处理序号和图之间的关系
map<int ,int>reflect;                            //reflect[i]=j:i保存整个图，有0位数，j相当于i的序号。i从小到大按顺序存储。
int dx[4]={-1,0,0,1},dy[4]={0,-1,1,0};          //dx表示行的移动，dy表示列的移动
int power10[10];                               //保存10的幂次，避免重复计算
int match[N];                                 //match[i]=j:第i个位置保存的是j图
PII ReflectToZeroPosition(int i,int j);
int CheckTransform(int CurState,int direction);
void pre(){
    //获得0-8所有的排列
    int pos=0;
    n=362880;
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

    for(int i=1;i<=n;i++)node[i].pos=i;
  
    End=reflect[123456780];
    const int EndPair[9][5]={{2,6,8},{2,2,4},{3,1,3,5},{2,2,6},{3,1,5,7},
    {4,2,4,6,8},{3,0,3,5},{2,4,8},{3,0,5,7}};
    PII EndPosition[10];//获得最终的表格中0-8的位置
    for(int i=0;i<9;i++){
        EndPosition[i]=ReflectToZeroPosition(End,i);
    }
    for(int i=1;i<=n;i++){
        PII TempPosition[9];
        for(int j=0;j<9;j++){
            TempPosition[j]=ReflectToZeroPosition(i,j);
            if(TempPosition[j].first!=EndPosition[j].first||TempPosition[j].second!=EndPosition[j].second)node[i].H1++;
        }
        for(int k=0;k<9;k++){
            for(int s=k+1;s<9;s++){//有多少对相邻数字在原来数字中没有出现
                if(s==k)continue;
                if((TempPosition[k].first==TempPosition[s].first&&abs(TempPosition[k].second-TempPosition[s].second)==1)
                ||(TempPosition[k].second==TempPosition[s].second&&abs(TempPosition[k].first-TempPosition[s].first)==1))
                {   int fl=0;
                    for(int l=1;l<=EndPair[k][0];l++){
                        if(s==EndPair[k][l]){
                            fl=1;break;
                        }
                    }
                    if(!fl)node[k].H2++;
                }
            }
        }
        node[i].H=k1*node[i].H1+k2*node[i].H2;
    }
    
}

//用序号寻找数字j的位置
PII ReflectToZeroPosition(int i,int j){
    PII GridPosition;//GridPosition保存序号对应的图中j所在的位置(行，列）
    int tmp=match[i];
    //cout<<match[i]<<' '<<"j:"<<endl<<j<<endl;
    int ArrayPosition=0;
    while(tmp%10!=j){
        tmp/=10;
        ArrayPosition++;
    }
    ArrayPosition=8-ArrayPosition;
    GridPosition={ArrayPosition/3,ArrayPosition%3};
    return GridPosition;
}
int CheckTransform(int CurState,int direction){//如果当前的状态可以向某个方向转移就直接返回那个方向的状态，否则返回-1
    PII CurPosition=ReflectToZeroPosition(CurState,0);
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
        node[i].fa=0,node[i].F=inf;CloseList[i]=false;
    }
    while(OpenList.size())OpenList.pop();
}
int main(){

    pre();
    while(true){
        initial();
        printf("Please enter a 9-digit number(0 means the space) as your origin grid and -1 to exit:\n");
        scanf("%d",&Source);
        Source=reflect[Source];
        node[Source].G=0;node[Source].F=node[Source].H;
        if(Source==-1){
            printf("Thank you for playing.\nWish you a nice day~~\n");
            break;
        }
        for(int i=1;i<=n;i++){
            for(int j=0;j<4;j++){
                int v=CheckTransform(i,j);
                if(v!=-1){store[i].push_back(v);}
            }
        }
        bool PathExist=A_Star();
        if(!PathExist){
            printf("No any way to win the game!\n");
            continue;
        }
        int i=End;
        while(i){
            path.push(i);
            i=node[i].fa;
        }
        if(path.size()==1){
            printf("Your origin grid is the final grid!\n");
            continue;
        }
        int step=0;
        printf("There are %d states in total.\nThe origin grid:\n",(int)path.size()-1);
        OutputGrid(path.top());path.pop();
        while(path.size()){
            printf("Step %d:\n",++step);
            OutputGrid(path.top());
            path.pop();
        }
    }
    return 0;
}
