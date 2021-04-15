#ifndef _DNH_H_
#define _DNH_H_
#include<time.h>
#include<stdlib.h>
#include<unordered_set>
#include <vector>
#include<set>
#include <queue>
#include<map>
using namespace std;
struct bian
{
	int a, b;//a,b分别指一个edge的两个端点的序号。
	int cost;
	bian(int a1, int b1,int cost1) :a(a1), b(b1),cost(cost1) {}
	bian(){}
};
struct bianhash
{
	size_t operator()(const bian& p)const
	{
		return size_t(p.a);
	}
};
//比较函数是一个二元谓词
struct equal_bian
{
	bool operator()(const bian & p1, const bian & p2) const
	{
		return p1.a == p2.a&&p1.b == p2.b;
	}
};
struct bian_cmp
{
	int a;
	int b;
	int cost;
	int dist_a;
	int dist_b;
	bian_cmp(int a1, int b1, int cost1, int dist_a1, int dist_b1) :a(a1), b(b1), cost(cost1), dist_a(dist_a1), dist_b(dist_b1) { }
	bool operator <(const bian_cmp& x)const
	{
		return (cost + dist_a + dist_b) < (x.cost + x.dist_b + x.dist_a);
	}
};
extern float sum_prbability(float Ratio);
extern float InitRatio;
extern vector<int> parent;
extern vector<int> root;
extern vector<int> dist;
extern vector<int> vheap;
extern vector<bian> Eheap;
extern vector<int> SET;
extern vector<bool> BS;
extern unordered_set<bian, bianhash, equal_bian> stainer_tree;
//extern set<point_du> POINT_DU;
extern set<int> keynode;
void creating_tree();
void global_create_bs();
int sum_cost_stainer_tree();
void drew_stainer_tree();
#endif // !_DNH_H_