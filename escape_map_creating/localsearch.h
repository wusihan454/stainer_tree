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
extern int numP;//��Դ��Ŀ
extern float LSRadio;//ѡ�����־ֲ��������Եĸ�������
extern int MAXNFE;//���ĵ�������
extern int limit;//�޶ȣ���������޶�û�и��²��۷�������  
void walk_around_search(int m);
void purify_search(int m);
void ABC();
void gloal_search(int i);
#endif // _LOCALSEARCH_H_