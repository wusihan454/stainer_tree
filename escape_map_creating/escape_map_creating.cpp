#include"escape_map.h"
vector<length> L;
vector<vector<edge>> MAP;
int terminalnumber;
int obstaclenumber;
vector<obstacle> OBSTACLES;
vector<node> terminal;
vector<node> all_point_print;
vector<EDGE_horizion> EDGE_h;
vector<EDGE_vertical> EDGE_v;
set<int> horizion_coordinate;
set<int> vertical_coordinate;
int X_MAX, Y_MAX, X_MIN, Y_MIN;
map<node, int> all_point;
vector<bool> is_terminal;
void inserting(int a, int b)
{
	int MIN = a, MAX = b;
	for (int i = 0; i < L.size(); i++)
	{
		if (L[i].min > MAX || L[i].max < MIN)
		{
			continue;
		}
		else
		{
			if (a < L[i].min) MIN = a;
			else MIN = L[i].min;
			if (b < L[i].max) MAX = L[i].max;
			else MAX = b;
			L.erase(L.begin() + i);
			i--;
		}
	}
	L.push_back(length(MIN, MAX));
}
void readtxt()//读文件，并且获得图的上下左右边界。 
{
	char s[200] = "";
	char s0[20] = "SECTION Graph";
	while (strcmp(s, s0) != 0)
		cin.getline(s, 200);
	scanf_s("%s", s, 200);
	scanf_s("%d", &terminalnumber);
	scanf_s("%s", s, 200);
	scanf_s("%d", &obstaclenumber);
	cin.get();
	cin.getline(s, 200);
	cin.getline(s, 200);
	cin.getline(s, 200);	//读没有用的连续三行
	terminal.resize(terminalnumber);
	OBSTACLES.resize(obstaclenumber);
	horizion_coordinate.clear();
	vertical_coordinate.clear();
	X_MAX = -1; Y_MAX = -1; X_MIN = -1; Y_MIN = -1;
	for (int i = 0; i < terminalnumber; i++)
	{
		scanf_s("%s", s, 200);
		int dd;
		scanf_s("%d", &dd);
		scanf_s("%d%d", &terminal[i].x, &terminal[i].y);
		horizion_coordinate.insert(terminal[i].x);
		vertical_coordinate.insert(terminal[i].y);
	}

	cin.get();
	cin.getline(s, 200);
	cin.getline(s, 200);
	cin.getline(s, 200);	//读没有用的连续三行
	for (int i = 0; i < obstaclenumber; i++)
	{
		scanf_s("%s", s, 200);
		scanf_s("%d%d%d%d", &OBSTACLES[i].left_x, &OBSTACLES[i].bottle_y, &OBSTACLES[i].right_x, &OBSTACLES[i].top_y);
		horizion_coordinate.insert(OBSTACLES[i].left_x);
		horizion_coordinate.insert(OBSTACLES[i].right_x);
		vertical_coordinate.insert(OBSTACLES[i].bottle_y);
		vertical_coordinate.insert(OBSTACLES[i].top_y);
	}
	X_MIN = *horizion_coordinate.begin();
	X_MAX = *horizion_coordinate.rbegin();
	Y_MIN = *vertical_coordinate.begin();
	Y_MAX = *vertical_coordinate.rbegin();
}
void create_line()
{
	EDGE_v.clear();
	EDGE_h.clear();
	for (auto h : horizion_coordinate)
	{
		bool mark = false;
		for (auto t : terminal)
		{
			if (t.x == h)
			{
				mark = true;
				break;
			}
		}
		L.clear();
		if (mark)
		{
			for (auto t : terminal)
			{
				if (t.x == h)
				{
					int right = Y_MAX;
					int left = Y_MIN;
					for (auto p : OBSTACLES)
					{
						if (t.x > p.left_x&&t.x < p.right_x&&t.y <= p.bottle_y&&p.bottle_y < right)
						{
							right = p.bottle_y;
						}
						if (t.x > p.left_x&&t.x<p.right_x&&t.y >= p.top_y&&p.top_y>left)
						{
							left = p.top_y;
						}
					}
					if (right > left) inserting(left, right);
				}
			}
		}
		for (int i = 0; i < OBSTACLES.size(); i++)
		{
			if (OBSTACLES[i].left_x == h || OBSTACLES[i].right_x == h)
			{
				int x = h;
				int y1 = OBSTACLES[i].bottle_y;
				int y2 = OBSTACLES[i].top_y;
				int top_b = Y_MAX;
				int bottle_b = Y_MIN;
				for (int j = 0; j < OBSTACLES.size(); j++)
				{
					if (i != j)
					{
						if (x > OBSTACLES[j].left_x&&x < OBSTACLES[j].right_x&&OBSTACLES[j].bottle_y <= y1 && OBSTACLES[j].top_y >= y2)
						{
							top_b = bottle_b;
							break;
						}
						else if (x > OBSTACLES[j].left_x&&x < OBSTACLES[j].right_x&&OBSTACLES[j].bottle_y >= y1 && OBSTACLES[j].top_y >= y2 && OBSTACLES[j].bottle_y < top_b)
						{
							top_b = OBSTACLES[j].bottle_y;
						}
						else if (x > OBSTACLES[j].left_x&&x<OBSTACLES[j].right_x&&OBSTACLES[j].top_y <= y2 && OBSTACLES[j].bottle_y <= y1 && OBSTACLES[j].top_y>bottle_b)
						{
							bottle_b = OBSTACLES[j].top_y;
						}
					}
				}
				if (top_b > bottle_b)
				{
					inserting(bottle_b, top_b);
				}
			}
		}
		for (auto l : L)
		{
			EDGE_v.push_back(EDGE_vertical(l.min, l.max, h));
		}
	}
	for (auto v : vertical_coordinate)
	{
		bool mark = false;
		for (auto t : terminal)
		{
			if (t.y == v)
			{
				mark = true;
				break;
			}
		}
		L.clear();
		if (mark)
		{
			for (auto t : terminal)
			{
				if (t.y == v)
				{
					int right = X_MAX;
					int left = X_MIN;
					for (auto p : OBSTACLES)
					{
						if (t.y > p.bottle_y&&t.y < p.top_y&&t.x <= p.left_x&&p.left_x < right)
						{
							right = p.left_x;
						}
						if (t.y > p.bottle_y&&t.y<p.top_y&&t.x >= p.right_x&&p.right_x>left)
						{
							left = p.right_x;
						}
					}
					if (right > left) inserting(left, right);
				}
			}
		}
		for (int i = 0; i < OBSTACLES.size(); i++)
		{
			if (OBSTACLES[i].top_y == v || OBSTACLES[i].bottle_y == v)
			{
				//int y = OBSTACLES[i].bottle_y;
				int x1 = OBSTACLES[i].left_x;
				int x2 = OBSTACLES[i].right_x;
				int right_b = X_MAX;
				int left_b = X_MIN;
				for (int j = 0; j < OBSTACLES.size(); j++)
				{
					if (i != j)
					{
						if (v> OBSTACLES[j].bottle_y&&v< OBSTACLES[j].top_y&&OBSTACLES[j].left_x <= x1 && OBSTACLES[j].right_x >= x2)
						{
							right_b = left_b;
							break;
						}
						else if (v> OBSTACLES[j].bottle_y&&v < OBSTACLES[j].top_y&&OBSTACLES[j].left_x >= x1 && OBSTACLES[j].right_x >= x2 && OBSTACLES[j].left_x < right_b)
						{
							right_b = OBSTACLES[j].left_x;
						}
						else if (v > OBSTACLES[j].bottle_y&&v<OBSTACLES[j].top_y&&OBSTACLES[j].right_x <= x2 && OBSTACLES[j].left_x <= x1 && OBSTACLES[j].right_x>left_b)
						{
							left_b = OBSTACLES[j].right_x;
						}
					}
				}
				if (right_b > left_b)
				{
					inserting(left_b, right_b);
				}
			}
		}
		for (auto l : L)
		{
			EDGE_h.push_back(EDGE_horizion(l.min, l.max, v));
		}
	}
	for (auto m : OBSTACLES)
	{
		for (int i = 0; i < EDGE_h.size(); i++)
		{
			if (EDGE_h[i].xi >= m.right_x || EDGE_h[i].xj <= m.left_x&&EDGE_h[i].y >= m.top_y || EDGE_h[i].y <= m.bottle_y)
				continue;
			if (EDGE_h[i].y > m.bottle_y&&EDGE_h[i].y < m.top_y&&EDGE_h[i].xi<m.left_x&&EDGE_h[i].xj>m.right_x)
			{
				EDGE_h.push_back(EDGE_horizion(EDGE_h[i].xi, m.left_x, EDGE_h[i].y));
				EDGE_h.push_back(EDGE_horizion(m.right_x, EDGE_h[i].xj, EDGE_h[i].y));
				EDGE_h.erase(EDGE_h.begin() + i, EDGE_h.begin() + i + 1);
				i--;
			}
			else if (EDGE_h[i].y > m.bottle_y&&EDGE_h[i].y < m.top_y&&EDGE_h[i].xi < m.left_x&&EDGE_h[i].xj <= m.right_x&&EDGE_h[i].xj >= m.left_x)
			{
				EDGE_h[i].xj = m.left_x;
			}
			else if (EDGE_h[i].y > m.bottle_y&&EDGE_h[i].y<m.top_y&&EDGE_h[i].xj>m.right_x&&EDGE_h[i].xi <= m.right_x&&EDGE_h[i].xi >= m.left_x)
			{
				EDGE_h[i].xi = m.right_x;
			}
		}
		for (int i = 0; i < EDGE_v.size(); i++)
		{
			if (EDGE_v[i].yi >= m.top_y || EDGE_v[i].yj <= m.bottle_y&&EDGE_v[i].x >= m.right_x || EDGE_v[i].x <= m.left_x)
				continue;
			if (EDGE_v[i].x > m.left_x&&EDGE_v[i].x < m.right_x&&EDGE_v[i].yi<m.bottle_y&&EDGE_v[i].yj>m.top_y)
			{
				EDGE_v.push_back(EDGE_vertical(EDGE_v[i].yi, m.bottle_y, EDGE_v[i].x));
				EDGE_v.push_back(EDGE_vertical(m.top_y, EDGE_v[i].yj, EDGE_v[i].x));
				EDGE_v.erase(EDGE_v.begin() + i, EDGE_v.begin() + i + 1);
				i--;
			}
			else if (EDGE_v[i].x > m.left_x&&EDGE_v[i].x < m.right_x&&EDGE_v[i].yi < m.bottle_y&&EDGE_v[i].yj <= m.top_y&&EDGE_v[i].yj >= m.bottle_y)
			{
				EDGE_v[i].yj = m.bottle_y;
			}
			else if (EDGE_v[i].x > m.left_x&&EDGE_v[i].x<m.right_x&&EDGE_v[i].yj>m.top_y&&EDGE_v[i].yi <= m.top_y&&EDGE_v[i].yi >= m.bottle_y)
			{
				EDGE_v[i].yi = m.top_y;
			}
		}
	}
	horizion_coordinate.clear();
	vertical_coordinate.clear();
}
void sum_intersect_point()
{
	vector<EDGE_horizion> EDGE_h_copy;//最后生成的所有横边放在这之中 
	vector<EDGE_vertical> EDGE_v_copy;//最后生成的所有竖边放在这之中
	EDGE_h_copy.clear();
	EDGE_v_copy.clear();
	vector<int> temp;

	for (auto h : EDGE_h)
	{
		temp.clear();
		for (auto v : EDGE_v)
		{
			if (v.x > h.xi&&v.x < h.xj&&h.y >= v.yi&&h.y <= v.yj)
			{
				temp.push_back(v.x);
			}
		}
		if (temp.size() > 0)
		{
			sort(temp.begin(), temp.end());
			EDGE_h_copy.push_back(EDGE_horizion(h.xi, temp[0], h.y));
			for (int j = 0; j < temp.size() - 1; j++)
			{
				EDGE_h_copy.push_back(EDGE_horizion(temp[j], temp[j + 1], h.y));
			}
			EDGE_h_copy.push_back(EDGE_horizion(temp[temp.size() - 1], h.xj, h.y));
		}
		else
		{
			EDGE_h_copy.push_back(h);
		}
	}
	for (auto v : EDGE_v)
	{
		temp.clear();
		for (auto h : EDGE_h)
		{
			if (h.y > v.yi&&h.y < v.yj&&v.x >= h.xi&&v.x <= h.xj)
			{
				temp.push_back(h.y);
			}
		}
		if (temp.size() > 0)
		{
			sort(temp.begin(), temp.end());
			EDGE_v_copy.push_back(EDGE_vertical(v.yi, temp[0], v.x));
			for (int j = 0; j < temp.size() - 1; j++)
			{
				EDGE_v_copy.push_back(EDGE_vertical(temp[j], temp[j + 1], v.x));
			}
			EDGE_v_copy.push_back(EDGE_vertical(temp[temp.size() - 1], v.yj, v.x));
		}
		else
		{
			EDGE_v_copy.push_back(v);
		}
	}
	EDGE_h.assign(EDGE_h_copy.begin(), EDGE_h_copy.end());
	EDGE_v.assign(EDGE_v_copy.begin(), EDGE_v_copy.end());
	EDGE_h_copy.clear();
	EDGE_v_copy.clear();
	vector<EDGE_horizion>().swap(EDGE_h_copy);
	vector<EDGE_vertical>().swap(EDGE_v_copy);
	//printf("sum_intersect\n");
}
void create_escape_graph()
{
	readtxt();
	create_line();
	sum_intersect_point();
	MAP.clear();
	set<node> all_temp;
	for (auto h : EDGE_h)
	{
		all_temp.insert(node(h.xi, h.y));

		all_temp.insert(node(h.xj, h.y));
	}

	for (auto v : EDGE_v)
	{
		all_temp.insert(node(v.x, v.yi));

		all_temp.insert(node(v.x, v.yj));
	}
	is_terminal.clear();
	is_terminal.resize(all_temp.size());
	for (auto b : is_terminal) b = false;
	int seq = 0;
	for (auto p : all_temp)
	{
		all_point.insert(pair<node, int>(p, seq));
		bool is_t = false;
		for (auto t : terminal)
		{
			if (t == p) 
			{
				is_t = true; break;
			}
		}
		if (is_t) is_terminal[seq] = true;
		seq++;
	}
	MAP.resize(all_point.size()); 
	int edge_num=0;
	for (auto h : EDGE_h)
	{
		map<node, int>::iterator iter_a, iter_b;
		iter_a = all_point.find(node(h.xi, h.y));
		iter_b = all_point.find(node(h.xj, h.y));
		MAP[iter_a->second].push_back(edge(iter_b->second, h.xj - h.xi));
		MAP[iter_b->second].push_back(edge(iter_a->second, h.xj - h.xi));
		edge_num++;
	}
	for (auto v : EDGE_v)
	{
		map<node, int>::iterator iter_a, iter_b;
		iter_a = all_point.find(node(v.x, v.yi));
		iter_b = all_point.find(node(v.x, v.yj));
		MAP[iter_a->second].push_back(edge(iter_b->second, v.yj - v.yi));
		MAP[iter_b->second].push_back(edge(iter_a->second, v.yj - v.yi));
		edge_num++;
	}
	EDGE_h.clear();
	EDGE_v.clear();
	vector<EDGE_horizion>().swap(EDGE_h);
	vector<EDGE_vertical>().swap(EDGE_v);
}
void print_init() 
{
	all_point_print.resize(all_point.size());
	for (int i = 0; i < terminal.size(); i++)
	{
		all_point_print[i].x = terminal[i].x;
		all_point_print[i].y = terminal[i].y;
	}
	int temp = terminal.size() - 1;
	for (auto p : all_point)
	{
		bool mark = false;
		for (auto t : terminal)
		{
			if (p.first == t)
			{
				mark = true;
				break;
			}
		}
		if (!mark)
		{
			temp++;
			all_point_print[temp] = p.first;
		}
	}
}
void drew_escape_map()
{
	for (int i = 0; i < all_point_print.size(); i++)
	{
		if(i < terminal.size())
			printf("T %d %d %d\n", i, all_point_print[i].x, all_point_print[i].y);
		else 
			printf("N %d %d %d\n", i, all_point_print[i].x, all_point_print[i].y);

	}
	for (auto h : EDGE_h)
	{
		int a, b;
		for (int i = 0; i < all_point_print.size(); i++)
		{
			if (h.xi == all_point_print[i].x&&all_point_print[i].y == h.y) a = i;
			else if (h.xj == all_point_print[i].x&&all_point_print[i].y == h.y) b = i;
		}
		printf("S %d %d\n", a, b);
	}
	for (auto v : EDGE_v)
	{
		int a, b;
		for (int i = 0; i < all_point_print.size(); i++)
		{
			if (v.yi == all_point_print[i].y&&all_point_print[i].x == v.x) a = i;
			else if (v.yj == all_point_print[i].y&&all_point_print[i].x == v.x) b = i;
		}
		printf("S %d %d\n", a, b);
	}
}