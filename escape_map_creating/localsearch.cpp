#include"dnh.h"
#include"escape_map.h"
#include"localsearch.h"
#include <cstring>
#include <time.h>
#include <stdlib.h>
#include<iterator>
int numP;//蜜源数目
float LSRadio;//选择哪种局部搜索策略的概率因子
int MAXNFE;//最大的迭代次数
int limit;
result* RESULT;
result BEST_RESULT;
void walk_around_search(int m)//不同的部分也就是求两个keynode的并集减去交集 ，相同的部分就是剩余所有的部分。
{
	int a;
	int b;
	a = m;
	b =rand() % numP;
	while(b==a) b=rand() % numP;
	set<int> DIFF;//并集减去交集
	set<int> SAME;
	set_symmetric_difference(RESULT[a].keynode.begin(), RESULT[a].keynode.end(), RESULT[b].keynode.begin(), RESULT[b].keynode.end(), inserter(DIFF, DIFF.begin()));
	int hamm = DIFF.size();
	while (hamm == 0)
	{
		gloal_search(b);//两个解相同，改变其中一个解。
		set_symmetric_difference(RESULT[a].keynode.begin(), RESULT[a].keynode.end(), RESULT[b].keynode.begin(), RESULT[b].keynode.end(), inserter(DIFF, DIFF.begin()));
		hamm = DIFF.size();
	}
	SAME.clear();
	int e = all_point.size();
	while (SAME.size() < hamm)
	{
		int temp = rand() %e;
		while (is_terminal[temp] || DIFF.find(temp) != DIFF.end())
			temp = rand() %e;
		SAME.insert(temp);
	}
	BS.clear();
	BS.resize(all_point.size(),false);
	//for (auto l : BS) l = false;
	for (auto l : RESULT[m].keynode)
	{
		BS[l] = true;
	}
	for (int i=0;i<e;i++)
	{
		if (is_terminal[i])
		{
			BS[i] = true;
		}
	}
	for (auto l : DIFF)
	{
		if (rand() % 2 == 0)
		{
			BS[l] = true;
		}
		else BS[l] = false;
	}
	for (auto l : SAME)
	{
		if (rand() % 2 == 0)
		{
			BS[l] = true;
		}
		else BS[l] = false;
	}
	creating_tree();
	int cost= sum_cost_stainer_tree();
	if (cost < RESULT[m].all_cost)
	{
		RESULT[m].all_cost = cost;
		RESULT[m].fitness = 1.0 / (RESULT[m].all_cost + 1);
		RESULT[m].st.clear();
		for (auto e : stainer_tree)
		{
			RESULT[m].st.push_back(e);
		}
		//sum_keynode();
		RESULT[m].keynode.clear();
		for (auto p : keynode)
			RESULT[m].keynode.insert(p);
		if (cost < BEST_RESULT.all_cost)
		{
			BEST_RESULT.all_cost = RESULT[m].all_cost;
			BEST_RESULT.keynode.clear();
			for (auto l : RESULT[m].keynode)
			{
				BEST_RESULT.keynode.insert(l);
			}
			BEST_RESULT.st.clear();
			BEST_RESULT.st.assign(RESULT[m].st.begin(), RESULT[m].st.end());

		}
	}
	else RESULT[m].trival++;
}
void purify_search(int m)
{
	int a;
	int b;
	a = m;
	b = rand() % numP;
	while (b == a) b = rand() % numP;
	set<int> temp1;
	set_symmetric_difference(RESULT[a].keynode.begin(), RESULT[a].keynode.end(), RESULT[b].keynode.begin(), RESULT[b].keynode.end(), inserter(temp1, temp1.begin()));
	int hamm = temp1.size();
	while (hamm == 0)
	{
		gloal_search(b);//两个解相同，改变其中一个解。
		set_symmetric_difference(RESULT[a].keynode.begin(), RESULT[a].keynode.end(), RESULT[b].keynode.begin(), RESULT[b].keynode.end(), inserter(temp1, temp1.begin()));
		hamm = temp1.size();
	}
	stainer_tree.clear();
	for (auto l: RESULT[a].st)
	{
			stainer_tree.insert(l);
	}
	for (auto l : RESULT[b].st)
	{
		int p1 = l.a, p2 = l.b;
		if (stainer_tree.find(bian(p2, p1, 0)) == stainer_tree.end() && stainer_tree.find(l) == stainer_tree.end())
			stainer_tree.insert(l);
	}
	vector<int> du(all_point.size(), 0);
	for (auto l : stainer_tree)
	{
		du[l.a]++;
		du[l.b]++;
	}
	keynode.clear();
	for (int i = 0; i < du.size(); i++)
	{
		if (du[i] > 2 && !is_terminal[i]) keynode.insert(i);
	}
	BS.clear();
	BS.resize(all_point.size());
	for (auto l : BS) l = false;
	for (int i = 0; i < is_terminal.size(); i++)
	{
		if (is_terminal[i])
		{
			BS[i] = true;
		}
	}
	for (auto l : keynode)
	{
		BS[l] = true;
	}
	creating_tree();
	int cost = sum_cost_stainer_tree();
	if (cost < RESULT[m].all_cost)
	{
		RESULT[m].all_cost = cost;
		RESULT[m].fitness = 1.0 / (RESULT[m].all_cost + 1);
		RESULT[m].st.clear();
		for (auto e : stainer_tree)
		{
			RESULT[m].st.push_back(e);
		}
		//sum_keynode();
		RESULT[m].keynode.clear();
		for (auto p : keynode)
			RESULT[m].keynode.insert(p);
		if (cost < BEST_RESULT.all_cost)
		{
			BEST_RESULT.all_cost = RESULT[m].all_cost;
			BEST_RESULT.keynode.clear();
			for (auto l : RESULT[m].keynode)
			{
				BEST_RESULT.keynode.insert(l);
			}
			BEST_RESULT.st.clear();
			BEST_RESULT.st.assign(RESULT[m].st.begin(), RESULT[m].st.end());
		}
	}
	else RESULT[m].trival++;
}
void gloal_search(int i)
{
	global_create_bs();
	creating_tree();
	RESULT[i].trival = 1;
	RESULT[i].all_cost = sum_cost_stainer_tree();
	RESULT[i].st.clear();
	RESULT[i].fitness = 1.0 / (RESULT[i].all_cost + 1);
	for (auto e : stainer_tree)
	{
		RESULT[i].st.push_back(e);
	}
	//sum_keynode();
	RESULT[i].keynode.clear();
	for (auto p : keynode)
		RESULT[i].keynode.insert(p);
	if (RESULT[i].all_cost < BEST_RESULT.all_cost|| BEST_RESULT.all_cost==-1)
	{
		BEST_RESULT.all_cost = RESULT[i].all_cost;
		BEST_RESULT.keynode.clear();
		for (auto l : RESULT[i].keynode)
		{
			BEST_RESULT.keynode.insert(l);
		}
		BEST_RESULT.st.clear();
		BEST_RESULT.st.assign(RESULT[i].st.begin(), RESULT[i].st.end());

	}
}
void ABC()
{
	numP = 3;
	InitRatio = 2;
	limit =5;
	//limit = (all_point.size()-terminal.size())*2;
	MAXNFE = 10000;
	LSRadio = 0.1;
	RESULT = new result[numP];
	BEST_RESULT.all_cost = -1;
	for(int i=0;i<numP;i++) gloal_search(i);
	int h = 0;
	while (h < MAXNFE)
	{
		for (int i = 0; i < numP; i++)//employed bee
		{
			if (rand() % 100 < LSRadio * 100) walk_around_search(i); 
			else purify_search(i);
		}
		float maxfitness = 0;
		for (int i = 0; i < numP; i++)
		{
			if (RESULT[i].fitness > maxfitness) maxfitness = RESULT[i].fitness;
		}
		int times = 0;
		int choose = 0;
		while (times < numP)
		{
			float p = (0.9*RESULT[choose].fitness) / maxfitness + 0.1;
			if (rand() % 100 <= p * 100)
			{
				if (rand() % 100 < LSRadio * 100) purify_search(choose);
				else walk_around_search(choose);
				times++;
			}
			choose++;
			if (choose == numP) choose = 0;
		}
		for (int i = 0; i < numP; i++)
		{
			if (RESULT[i].trival > limit) gloal_search(i);
		}
		h++;
	}
	delete []RESULT;

}
/*
int main(int argc, char* argv[])
{
	clock_t start_t = clock();
#pragma warning(disable:4996)
	char datapath[256], resultpath[256];
	strcpy(datapath, argv[1]);
	strcpy(resultpath, argv[2]);
	unsigned seed = atoi(argv[3]);
	FILE *stream1, *stream2;
	freopen_s(&stream1, datapath, "r", stdin);
	freopen_s(&stream2, resultpath, "w", stdout);
	srand(seed);
	create_escape_graph();
	ABC();
	printf("%d\n", BEST_RESULT.all_cost);
	clock_t  end_t = clock();//时钟检测
	float m = (end_t - start_t)*1.0 / CLOCKS_PER_SEC;
	printf("run time: %f s\n", m);
	fclose(stdin);
	fclose(stdout);
	return 0;
}
*/

void drew_stainer_tree()
{
	for (auto iter = BEST_RESULT.st.begin(); iter != BEST_RESULT.st.end(); ++iter)
	{
		node temp1, temp2;
		int r1, r2;
		for (auto p : all_point)
		{
			if (iter->a == p.second)
			{
				temp1 = p.first;
			}
			else if (iter->b == p.second)
			{
				temp2 = p.first;
			}
		}
		for (int i = 0; i < all_point_print.size(); i++)
		{
			if (all_point_print[i] == temp1) { r1 = i; }
			else if (all_point_print[i] == temp2) { r2 = i; }
		}
		printf("E %d %d\n", r1, r2);
	}
}

int main()
{
	clock_t start_t = clock();
	FILE *stream1;
	//FILE *stream2;
	freopen_s(&stream1, "RC06.stp", "r", stdin);
	srand(unsigned int(time(0)));
	create_escape_graph();
	ABC();
	//printf("%d\n", BEST_RESULT.all_cost);
	//for(int i=0;i<numP;i++)
		//printf("%d\n", RESULT[i].all_cost);
	printf("%d\n",BEST_RESULT.all_cost);
	//print_init();
	//printf("%d\n", all_point.size());
	//freopen_s(&stream2, "RC01.edge", "w", stdout);
	//drew_escape_map();
	//drew_stainer_tree();
	clock_t  end_t = clock();//时钟检测
	float m = (end_t - start_t)*1.0 / CLOCKS_PER_SEC;
	printf("run time: %f s\n", m);
	fclose(stdin);
	//fclose(stdout);
	return 0;
}
