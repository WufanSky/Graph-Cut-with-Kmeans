#include <iostream>
#include <cstring>
#include <queue>
using namespace std;
#define min(a,b)(a<b?a:b)
#define INF 1000000
const int MAX=210;
struct Node
{
    int c;
    int f;
};
int sx,ex;//sx和ex分别代表源点和汇点
int pre[MAX];//MAX是点的个数的上限
Node map[MAX][MAX];
int n,m;
bool BFS()//BFS搜索层次网络
{
    memset(pre,0,sizeof(pre));
    queue< int > Q;
    Q.push(sx);
    pre[sx]=1;
    while(!Q.empty())
    {
        int d=Q.front();
        Q.pop();
        for(int i=1;i<=n;i++)//n是顶点个数
        {
            if(!pre[i]&&map[d][i].c-map[d][i].f)//未访问过，且仍有流量
            {
                pre[i]=pre[d]+1;
                Q.push(i);
            }
        }
    }
    return pre[ex]!=0;

}
int dinic(int pos,int flow)//pos是顶点号，flow是当前顶点所能得到的流量
{
    int f=flow;
    if(pos==ex)
    return flow;
    for(int i=1;i<=n;i++)
    {
        if(map[pos][i].c-map[pos][i].f&&pre[pos]+1==pre[i])
        {
            int a=map[pos][i].c-map[pos][i].f;
            int t=dinic(i,min(a,flow));
            map[pos][i].f+=t;
            map[i][pos].f-=t;
            flow-=t;//我最开始就是这里没弄明白，我不明白为什么要此顶点得到的流量减去改变量；答案就在下面的  return f-flow;
        }
    }
    return f-flow;//其实这里返回给他前一层的就是这个t;因为t在层函数里面都有，所以所过避免重复就写成这样；

}
int slove()
{
    int sum=0;
    while(BFS())
    {
        sum+=dinic(sx,INF);
    }
    return sum;
}
int main()
{
    int u,v,w;
    while(cin>>m>>n)
    {
        sx=1;
        ex=n;
       memset(map,0,sizeof(map)) ;
       for(int i=1;i<=m;i++)
       {
           cin>>u>>v>>w;
           map[u][v].c += w;
       }
       cout<<slove()<<endl;
    }
    return 0;
}