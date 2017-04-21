#include "maxflow.h"
using namespace std;
//vector<type>(v).swap(v);
const int f_Maxr = 2250010, f_maxc = 13000000, maxnum = 2147483647;
const int INF = 1000001;
const int MaxEdge = 2000000, MaxVertice = 1300000;//ƒø«∞ºŸ…ËÕº∆¨◊Ó∂‡ «1440≥À900µƒ

//const int maxr = 1520, maxc = 1520, MAXINF = 1000000;
double afg[maxr][maxc], bbg[maxr][maxc], p[maxr][maxc][2];
int picrow = 0, piccol = 0;
int R[maxr][maxc], G[maxr][maxc], B[maxr][maxc], Gray[maxr][maxc];
struct Edge
{
	int dst;	//±ﬂµƒ÷’µ„
	int num;
	int next;
	//	int val;
	float c;	//±ﬂµƒ»›¡ø
	float f;	//±ﬂƒø«∞µƒ¡˜¡ø
	Edge() {}
	Edge(inta tdst, int tnum, float cc, float ff = 0)
	{
		dst = tdst;
		num = tnum;
		c = cc;
		f = ff;
	}
}myedge["最大边数"];
int S, T, CountofEdge = 2;
int fst[MaxVertice], myq[MaxVertice];
//bool f[f_maxn];

vector<vector<Edge> > mv;
bool myf[MaxVertice];


inline int getnum(int i, int j)
{// S : 0, T : 1, ÷Æ∫Ûµƒ∞¥À≥–Ú
	return i * piccol + j + 2;
}
inline int getrow(int num)
{
	return (num - 2) / piccol;
}
inline int getcol(int num)
{
	return (num - 2) % piccol;
}

void Addedge(int srcp, int dstp, float c)
{
	myedge[CountofEdge].dst = dstp;
	myedge[CountofEdge].c = c;
	myedge[CountofEdge].next = fst[srcp];
	fst[srcp] = CountofEdge++;
}

void Build()
{
	S = 1, T = 2;
	for(int i = 0; i < picrow; ++i)
		for(int j = 0; j < piccol; ++j)
		{
			Addedge(S, getnum(i, j), a[i][j]);
			Addedge(getnum(i, j), S, 0);
			Addedge(getnum(i, j), T, b[i][j]);
			Addedge(T, getnum(i, j), 0);
			if(i + 1 < picrow)
			{
				Addedge(getnum(i, j), getnum(i + 1, j), p[i][j][0]);
				Addedge(getnum(i + 1, j), getnum(i, j), p[i][j][0]);
			}
			if(j + 1 < piccol)
			{
				Addedge(getnum(i, j), getnum(i, j + 1), p[i][j][1]);
				Addedge(getnum(i, j + 1), getnum(i, j), p[i][j][1]);
			}
		}
}

bool Bfs()
{
	//int l, r, k, u;
	memset(pre, 0, sizeof(pre));
	myq[0] = S; 
	pre[S] = 1;
	for(int l = 0, r = 0; l <= r; ++l)
	{
		int cur = myq[l];
		for(int next = fst[cur]; next; next = edge[next].next)
		{
			if(edge[next].c - edge[next].f && !pre[next])
			{
				pre[edge[next].dst] = pre[cur] + 1;
				myq[++r] = edge[next].dst;
			}
		}
	}
	return pre[T] != 0;
}

void build()
{
	S = 0, T = 1;
	mv.clear();
	mv.resize(MaxVertice);
	for (int i = 0; i < picrow; ++i)
	for (int j = 0; j < piccol; ++j)
	{
		int tnum = getnum(i, j);
		mv[S].push_back(Edge(tnum, CountofEdge++, -log(afg[i][j])));
		mv[tnum].push_back(Edge(S, CountofEdge++, 0));
		mv[tnum].push_back(Edge(T, CountofEdge++, -log(bbg[i][j])));
		mv[T].push_back(Edge(tnum, CountofEdge++, 0));
	}
	for (int i = 0; i < picrow; ++i)
	for (int j = 0; j < piccol; ++j)
	{
		if (i + 1 < picrow)
		{
			mv[getnum(i, j)].push_back(Edge(getnum(i + 1, j), CountofEdge++, p[i][j][0]));
			mv[getnum(i + 1, j)].push_back(Edge(getnum(i, j), CountofEdge++, p[i][j][0]));
		}
		if (j + 1 < piccol)
		{
			mv[getnum(i, j)].push_back(Edge(getnum(i, j + 1), CountofEdge++, p[i][j][1]));
			mv[getnum(i, j + 1)].push_back(Edge(getnum(i, j), CountofEdge++, p[i][j][1]));
		}
	}
}


int pre[MaxVertice];
queue<int> Q;
bool BFS()
{
	memset(pre, 0, sizeof(pre));
	Q.push(S);
	pre[S] = 1;
	while (!Q.empty())
	{
		int temp = Q.front();
		Q.pop();
		int tempsize = mv[temp].size();
		for (int i = 0; i < tempsize; ++i)
		{
			Edge tmpedge = mv[temp][i];
			if (!pre[tmpedge.dst] && tmpedge.c - tmpedge.f > -0.000001)
			{
				pre[tmpedge.dst] = pre[temp] + 1;
				Q.push(tmpedge.dst);
			}
		}
	}
	return pre[T] != 0;
}
float Dinic(int pos, float flow)//flow is the available input flow
{
	float f = flow;
	if(pos == T) return flow;
	for(int i = fst[pos]; i; i = edge[i].next)
	{
		if(edge[i].c - edge[i].f && pre[pos] + 1 == pre[edge[i].dst])
		{
			float tmp = edge[i].c - edge[i].f;
			float t = dinic(edge[i].dst, min(tmp, flow));
			edge[i].f += t;
			edge[i^1].f -= t;
			flow -= t;
			if(!flow) break;
		}
	}
	return f - flow;
}
float dinic(int pos, float flow)
{
	//printf("%d\n", pos);
	float f = flow;
	if (pos == T) return flow;
	int tsize = mv[pos].size();
	for (int i = 0; i < tsize; ++i)
	{
		Edge tedge = mv[pos][i];
		if (tedge.c - tedge.f > -0.000001 && pre[pos] + 1 == pre[tedge.dst])
		{
			float a = tedge.c - tedge.f;
			float t = dinic(tedge.dst, min(a, flow));
			tedge.f += t;
			for (int j = 0; j < mv[tedge.dst].size(); j++)
			{
				if (mv[tedge.dst][j].dst == pos)
				{
					mv[tedge.dst][j].f -= t;
					break;
				}
			}
			flow -= t;
			if(!flow) break;
		}
	}
	return f - flow;
}

void solve()
{
	while (BFS())
		int temp = dinic(S, INF);
}

void findf()
{
	memset(myf, 0, sizeof(myf));
	for (int i = 0; i < mv[S].size(); ++i)
	{
		if (mv[S][i].f)
			myf[mv[S][i].dst] = S;
		else
			myf[mv[S][i].dst] = T;
	}
}


