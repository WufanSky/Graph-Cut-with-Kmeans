#ifndef MAXFLOW_H
#define MAXFLOW_H
#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <queue>
#include "maxflow.h"

void addedge(int u, int v, float c);

bool BFS();

float dinic(int u, float c);

void getflow();

void cut();

inline int getnum(int i, int j);
inline int getrow(int num);
inline int getcol(int num);

void build();

void solve();

void findf();

const int maxr = 1520, maxc = 1520, MAXINF = 1000000;

#endif