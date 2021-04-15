#ifndef _LOCALSEARCH_H_
#define _LOCALSEARCH_H_
struct result
{
	set<int> keynode;
	int all_cost;
	vector<bian> st;
	float fitness;
	int trival;
};
extern result* RESULT;
extern result BEST_RESULT;
extern int numP;//蜜源数目
extern float LSRadio;//选择哪种局部搜索策略的概率因子
extern int MAXNFE;//最大的迭代次数
extern int limit;//限度，超过这个限度没有更新采蜜蜂变成侦查蜂  
void walk_around_search(int m);
void purify_search(int m);
void ABC();
void gloal_search(int i);
#endif // _LOCALSEARCH_H_