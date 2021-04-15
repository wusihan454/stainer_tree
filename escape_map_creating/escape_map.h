#ifndef _ESCAPE_MAP_H_
#define _ESCAPE_MAP_H_
#include <iostream>
#include <string.h>
#include <cstring>
#include <algorithm>
#include <vector>
#include<set>
#include<map>
using namespace std;
struct node//终端的node 
{
	int x;
	int y;
	node(int a, int b) :x(a), y(b) {}
	bool operator <(const node& a)const { return x < a.x || (x == a.x&&y < a.y); }
	bool operator ==(const node& a)const { return x == a.x&&y == a.y; }
	node() {	}
};
struct obstacle
{
	int left_x;
	int bottle_y;
	int right_x;
	int top_y;
};
struct EDGE_vertical
{
	int yi;
	int yj;
	int x;
	EDGE_vertical(int a, int b, int c) :yi(a), yj(b), x(c) {
	}
};
struct EDGE_horizion
{
	int xi;
	int xj;
	int y;
	EDGE_horizion(int a, int b, int c) :xi(a), xj(b), y(c) {
	}
};
struct edge {
	int cost;
	int nextpoint;
	edge(int a, int b) :nextpoint(a), cost(b) {
	}
};
struct length
{
	int min;
	int max;
	length(int a, int b) :min(a), max(b) {
	}
};
extern vector<length> L;
extern vector<vector<edge>> MAP;
extern int terminalnumber;
extern int obstaclenumber;
extern vector<obstacle> OBSTACLES;
extern vector<node> terminal;
extern vector<node> all_point_print;
extern vector<EDGE_horizion> EDGE_h;
extern vector<EDGE_vertical> EDGE_v;
extern set<int> horizion_coordinate;
extern set<int> vertical_coordinate;
extern int X_MAX, Y_MAX, X_MIN, Y_MIN;
extern map<node, int> all_point;
extern vector<bool> is_terminal;
void create_escape_graph();//后面能用的只有all_point，is_terminal， MAP，terminal
void drew_escape_map();
void print_init();
#endif // !_ESCAPE_MAP_H_