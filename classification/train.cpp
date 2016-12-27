#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <string>
#include <utility>
#include <omp.h>
#include <ctime>
#include <algorithm>
using namespace std;

typedef vector<double> ObjectFrame;
typedef vector<ObjectFrame> ObjectVideo;
int tot, order[2][1000], ans[2][1000], NN[1000], n[1000], m[1000];
string names[1000];
ObjectVideo videos[1000];
double dist[1000][1000];
vector<vector<bool> > pool;
vector<int> p, rank;
vector<double> value;
const int ele = 1;

double sqr(double x) {return x*x;}

vector<int> findkNN(vector<int> &s, int x, int except, int k)
{
	vector<int> ans(k);
	for (int i = 0; i < k; ++i)
		ans[i] = (except==s[0]?s[1]:s[0]);
	for (int i = 0; i < s.size(); ++i)
	{
		if (s[i] == except) continue;
		double tmp = dist[x][s[i]];
		int cur = s[i];
		for (int j = 0; j < k; ++j)
			if (tmp < dist[x][ans[j]])
			{
				tmp = dist[x][ans[j]];
				swap(cur, ans[j]);
			}
	}
	return ans;
}

int findMaxi(vector<int> x, int k, int *ans)
{
	int cur = ans[x[0]], cnt = 1;
	for (int i = 1; i < k; ++i)
		if (ans[x[i]] == cur)
			++cnt;
		else
		{
			if (cnt == 0)
			{
				cur = ans[x[i]];
				cnt = 1;
			}
			else
				--cnt;
		}
	return cur;
}

bool cmp(int a, int b) { return value[a] > value[b]+1e-8; }

double getValue(int *ans, vector<bool> cur)
{
	vector<int> s;
	s.clear();
	for (int i = 0; i < tot; ++i)
		if (cur[i])
			s.push_back(i);
	if (s.empty())
		return -1;
	int cor = 0;
	for (int i = 0; i < tot; ++i)
	{
		vector<int> x = findkNN(s, i, i, ele+0);
		int cl = findMaxi(x, ele+0, ans);
		if (cl == ans[i])
			++cor;
	}
	return (double)cor/tot;
}

void doit(int *order, int *ans, const char *file)
{
	cout << "doit" << endl;
	double best = 0;
	vector<bool> tmp(tot), cur;
	srand((unsigned)time(NULL));
	int iter = 200, mut = 2000, mat = 2000, left = 1000;
	for (int times = 0; times < iter; ++times)
	{
		for (int i = 0; i < left; ++i)
		{
			for (int j = 0; j < tot; ++j)
				tmp[j] = rand()%2;
			pool.push_back(tmp);
		}
		for (int i = 0; i < mut; ++i)
		{
			cur = pool[rand()%pool.size()];
			int k = rand()%tot;
			cur[k] = cur[k]^true;
			pool.push_back(cur);
		}
		for (int k = 0; k < mat; ++k)
		{
			int i = rand()%pool.size();
			cur = pool[rand()%pool.size()];
			for (int j = 0; j < tot; ++j)
				if (rand()%2)
					cur[j] = pool[i][j];
			pool.push_back(cur);
		}
		value.resize(pool.size());
		p.resize(pool.size());
		rank.resize(pool.size());
		#pragma omp parallel for num_threads(20) schedule(dynamic)
		for (int i = 0; i < pool.size(); ++i)
		{
			p[i] = i;
			value[i] = getValue(ans, pool[i]);
		}
		sort(p.begin(), p.end(), cmp);
		for (int i = 0; i < pool.size(); ++i)
			rank[p[i]] = i;
		pool[0] = pool[p[0]];
		int k = 1;
		if (times % 5 == 0)
			cout << value[p[0]] << endl;
		for (int i = 1; i < pool.size(); ++i)
			if (rank[i] < left)
				pool[k++] = pool[i];
		pool.resize(left);
	}
	FILE *fp = fopen(file, "w");
	vector<int> s;
	s.clear();
	for (int i = 0; i < tot; ++i)
		if (pool[0][i])
			s.push_back(i);
	int a[6][6], cor = 0;
	memset(a, 0, sizeof(a));
	for (int i = 0; i < tot; ++i)
	{
		int xj = i;
		vector<int> x = findkNN(s, i, i, ele+0);
		int cl = findMaxi(x, ele+0, ans);
		if (cl == ans[xj])
			++cor;
		++a[ans[xj]][cl];
	}
	for (int i = 0; i < 6; ++i)
	{
		for (int j = 0; j < 6; ++j)
			cout << a[i][j] << " ";
		cout << endl;
	}
	for (int i = 0; i < s.size(); ++i)
		fprintf(fp, "%s %d %d\n", names[s[i]].c_str(), n[s[i]], m[s[i]]);
	fclose(fp);
}

double getdist(ObjectFrame *a, ObjectFrame *b)
{
	if (a->size() > b->size()) swap(a, b);
	double ans = 0;
	for (int i = 0; i < a->size(); ++i)
		ans += sqr((*a)[i]-(*b)[i]);
	return ans;
}

void solve()
{
	omp_set_dynamic(0);
	FILE *fp = fopen("distanceMatrix.txt", "r");
	for (int i = 0; i < tot; ++i)
		for (int j = 0; j < tot; ++j)
			fscanf(fp, "%lf", &dist[i][j]);
	fclose(fp);
	doit(order[0],ans[0],"order0.txt");
	doit(order[1],ans[1],"order1.txt");
}

string int2str(int x, int k)
{
	string ans = "";
	while (x) {ans = char('0'+x%10)+ans;x/=10;}
	while (ans.size() < k)
		ans = "0"+ans;
	return ans;
}

void sortValue(double &a, double &b, double &c)
{
	if (a > b) swap(a, b);
	if (b < c) return;
	swap(b, c);
	if (a > b) swap(a, b);
}

ObjectVideo readVideo(string tmp, int n, int m)
{
	static ObjectVideo cur;
	cur.clear();
	for (int i = 0; i < n; ++i)
	{
		static ObjectFrame frame;
		frame.clear();
		string cur0 = int2str(i,4);
		FILE *fp = fopen(("../motion/"+tmp+"/"+cur0+"_00.txt").c_str(), "r");
		for (int j = 0; j < m; ++j)
		{
			double value;
			fscanf(fp, "%lf", &value);
			frame.push_back(value);
			if (j % 3 == 2)
				sortValue(frame[j-2], frame[j-1], frame[j]);
		}
		cur.push_back(frame);
		fclose(fp);
	}
	return cur;
}

void init()
{
	cout << "init" << endl;
	ifstream fin("trainList.txt");
	fin >> tot;
	for (int i = 0; i < tot; ++i)
	{
		/*cout << "readVideo " << i << endl;*/
		fin >> names[i] >> ans[0][i] >> ans[1][i] >> n[i] >> m[i];
		/*videos[i] = readVideo(names[i], n[i], m[i]);*/
		order[0][i] = i;
		order[1][i] = i;
	}
	fin.close();
}

int main()
{
	init();
	solve();
	return 0;
}