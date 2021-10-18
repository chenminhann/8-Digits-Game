#include<iostream>
#include<algorithm>
using namespace std;
int a[9]={0,1,2,3,4,5,6,7,8};
int store[(int)1e6];
int main(){
    freopen("StoreInput.txt","w",stdout);
    int cnt=0;
    int sum;
    int fl=0;
    do{
        sum=0;
        if(fl==0&&a[0]!=0)fl=cnt;
        for(int i=0;i<9;i++){
            sum=sum*10+a[i];
        }store[cnt++]=sum;
    }while(next_permutation(a,a+9));
    fl-=3000;
    for(int i=1;i<=200;i++){
        int pos=abs(rand())%(cnt-fl)+fl;
        printf("%d\n",store[pos]);
    }
    return 0;
}