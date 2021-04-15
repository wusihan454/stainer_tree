#include"dnh.h"
#include"escape_map.h"

//每一次dnh最后只需要保存st以及keynode,以及all_cost
float sum_prbability(float Ratio);
float InitRatio;
vector<int> parent;
vector<int> root;
vector<int> dist;
vector<int> vheap;
vector<int> SET;
vector<bool> BS;//bs同时用于一开始记录产生的虚拟终端，以及保存最后的keynode;
unordered_set<bian, bianhash, equal_bian> stainer_tree;
set<int> keynode;
int findroot(int i)
{
	if (SET[i] == i) return i;
	else
	{
		int temp = findroot(SET[i]);
		SET[i] = temp;
		return temp;
	}
}
void global_create_bs()//BS[i]和MAP[i]的i是对应的,用于最开始的nump个蜜源的形成；
{
	BS.clear();
	BS.resize(all_point.size());
	for (auto l : BS) l = false;
	for (int i=0;i<is_terminal.size();i++)
	{
		if (is_terminal[i])
		{
			BS[i] = true;
		}
	}
	float p = sum_prbability(InitRatio);
	for (int i = 0; i < BS.size(); i++)
	{
		if (rand() % 100 <= p * 100 )
		{
			BS[i] = true;
		}
	}
}
void unique_pruning()
{
	vector<vector<int>> topu;
	topu.resize(all_point.size());
	for(auto l : stainer_tree)
	{
		topu[l.a].push_back(l.b);
		topu[l.b].push_back(l.a);
	}
	vector<int> du(all_point.size(),0);
	for (int i = 0; i < topu.size(); i++)
	{
		du[i] = topu[i].size();
	}
	bool mark = false;
	for (int i = 0; i < du.size(); i++)
	{
		if (du[i] == 1 && !is_terminal[i])
		{
			mark = true;
			break;
		}
	}
	vector<int> temp;
	while (mark)
	{
		temp.clear();
		for (int i = 0; i < du.size(); i++)
		{
			if (du[i] == 1 && !is_terminal[i]) temp.push_back(i);
		}
		for (auto l : temp)
		{
			int point = l;
			int nextpoint;
			while (du[point] == 1 && !is_terminal[point])
			{
				du[point] = 0;
				nextpoint = topu[point][0];
				topu[point].clear();
				if (nextpoint == point) printf("xcvgherror\n");
				du[nextpoint]--;
				for (auto iter1 = topu[nextpoint].begin(); iter1 != topu[nextpoint].end(); ++iter1)
				{
					if (*iter1 == point)
					{
						topu[nextpoint].erase(iter1);
						break;
					}
				}
				auto iter1= stainer_tree.find(bian(point, nextpoint, 0));
				if (iter1 == stainer_tree.end()) iter1 = stainer_tree.find(bian(nextpoint, point, 0));
				if (iter1 == stainer_tree.end()) printf("error\n");
				else stainer_tree.erase(iter1);
				point = nextpoint;
			}
		}
		mark = false;
		for (int i = 0; i < du.size(); i++)
		{
			if (du[i] == 1 && !is_terminal[i])
			{
				mark = true;
				break;
			}
		}
	}
	keynode.clear();
	for (int i = 0; i < du.size(); i++)
	{
		if (du[i] > 2&& !is_terminal[i]) keynode.insert(i);
	}
}
void creating_tree()
{
	parent.clear();
	root.clear();
	dist.clear();
	SET.clear();
	parent.resize(all_point.size());
	root.resize(all_point.size());
	dist.resize(all_point.size());
	SET.resize(all_point.size());
	vheap.clear();
	for (int i = 0; i < parent.size(); i++)
	{
		parent[i] = i;
		root[i] = i;
		if (BS[i])
		{
			dist[i] = 0;
			vheap.push_back(i);
		}
		else dist[i] = -1;
	}
	random_shuffle(vheap.begin(), vheap.end());
	while (!vheap.empty())
	{
		int a = 0;
		int choose = vheap[a];
		if(choose!= parent[choose]) root[choose] = root[parent[choose]];
		for (auto p : MAP[choose])
		{
			if (dist[p.nextpoint]==-1||dist[p.nextpoint] > dist[choose] + p.cost)
			{
				dist[p.nextpoint] = dist[choose] + p.cost;
				parent[p.nextpoint] = choose;
				vheap.push_back(p.nextpoint);
			}
		}
		vheap.erase(vheap.begin() + a);
	}
	for (int i = 0; i < SET.size(); i++)
	{
		SET[i] = root[i];
	}
	//Eheap.clear();
	vector<bian_cmp> d;
	for (int i = 0; i < MAP.size(); i++)
	{
		for (int j = 0; j < MAP[i].size(); j++)
		{
			int np = MAP[i][j].nextpoint;
			if (root[np] != root[i])
			{
				d.push_back(bian_cmp(i, np,MAP[i][j].cost,dist[i],dist[np]));
			}
		}
	}
	sort(d.begin(), d.end());
	stainer_tree.clear();
	for(auto l:d)
	{
		bian_cmp temp =l;
		int root_a = findroot(temp.a);
		int root_b = findroot(temp.b);
		if (root_a!=root_b)
		{
			stainer_tree.insert(bian(temp.a, temp.b,temp.cost));
			int m = temp.a;
			while (parent[m] != m)
			{
				int temp_cost;
				for (auto l : MAP[m])
				{
					if (l.nextpoint == parent[m])
					{
						temp_cost = l.cost;
						break;
					}
				}
				auto iter1 = stainer_tree.find(bian(m, parent[m],  0));
				if (iter1 == stainer_tree.end()) stainer_tree.insert(bian(parent[m], m,temp_cost));
				m = parent[m];
			}
			m = temp.b;
			while (parent[m] != m)
			{
				int temp_cost;
				for (auto l : MAP[m])
				{
					if (l.nextpoint == parent[m])
					{
						temp_cost = l.cost;
						break;
					}
				}
				auto iter1 = stainer_tree.find(bian(m, parent[m],0));
				if (iter1 == stainer_tree.end()) stainer_tree.insert(bian(parent[m], m,temp_cost));
				m = parent[m];
			}
			SET[root_a] = SET[root_b];
		}
	}
	unique_pruning();
}
float sum_prbability(float Ratio)
{
	float max, min;
	int n = all_point.size();
	int l = terminal.size();
	float a = (l - 2)*Ratio / (n - l);
	if (a > 1.0 / n) max = a; else max = 1.0 / n;
	float b = (n - l)*1.0 / n;
	if (max < b) min = max;	else min = b;
	return min;
}
int sum_cost_stainer_tree()
{
	int all_cost=0;
	for (auto iter = stainer_tree.begin(); iter != stainer_tree.end(); ++iter)
	{
		all_cost = all_cost + iter->cost;
	}
	return all_cost;
}
