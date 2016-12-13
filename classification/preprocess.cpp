#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <string>
#include <utility>
#include <omp.h>
using namespace std;

typedef vector<double> ObjectFrame;
typedef vector<ObjectFrame> ObjectVideo;
int tot, order[2][1000], ans[2][1000], NN[1000], n[1000], m[1000];
string names[1000];
ObjectVideo videos[1000];
double dist[1000][1000];

double sqr(double x) {return x*x;}

double getdist(ObjectFrame *a, ObjectFrame *b)
{
	if (a->size() > b->size()) swap(a, b);
	double ans = 0;
	for (int i = 0; i < a->size(); ++i)
		ans += sqr((*a)[i]-(*b)[i]);
	for (int i = a->size(); i < b->size(); ++i)
	{
		cout << "fuck" << endl;
		ans += sqr((*b)[i]);
	}
	return ans;
}

double CalculateDistance(int a, int b)
{
	if (videos[a].size()>videos[b].size())swap(a,b);
	int offset = videos[a].size()*0.08;
	double dist[offset*2+1];
	memset(dist, 0, sizeof(dist));
	for (int j = offset; j < offset*2+1; ++j)
	{
		double cost = dist[j-1];
		int k = j - offset;
		dist[j] = 1e100;
		if (k >= videos[b].size()) continue;
		dist[j] = cost + getdist(&videos[a][0], &videos[b][k]);
	}
	for (int i = 1; i < videos[a].size(); ++i)
	{
		for (int j = 0; j < offset*2+1; ++j)
		{
			double cost = 1e100;
			int k = j - offset + i;
			if (j+1<videos[a].size()) cost = dist[j+1];
			if (k>0&&cost>dist[j])cost = dist[j];//modified
			if (k>0&&j>0&&cost>dist[j-1])cost = dist[j-1];
			dist[j] = 1e100;
			if (k < 0) continue;
			if (k >= videos[b].size()) continue;
			dist[j] = cost + getdist(&videos[a][i], &videos[b][k]);
		}
	}
	/*double tot = 0;
	for (int i = videos[b].size()-1; i >= 0; --i)
	{
		int dis = i - videos[a].size()+1;
		if (dis < -offset) break;
		if (dis <= offset)
			dist[dis+offset] += tot;
		tot += getdist(&videos[a][videos[a].size()-1],&videos[b][i]);
	}*/
	double ans = dist[0];
	for (int i = 1; i < offset*2+1; ++i)
		if (ans > dist[i])
			ans = dist[i];
	return ans / videos[a].size();
}

void solve()
{
	omp_set_dynamic(0);
	vector<pair<int, int> > tmp;
	for (int i = 0; i < tot; ++i)
		for (int j = i; j < tot; ++j)
			tmp.push_back(make_pair(i,j));
	#pragma omp parallel for num_threads(20) schedule(dynamic)
	for (int i = 0; i < tmp.size(); ++i)
	{
		cout << i << endl;
		dist[tmp[i].first][tmp[i].second] = dist[tmp[i].second][tmp[i].first] = CalculateDistance(tmp[i].first, tmp[i].second);
	}
	FILE *fp = fopen("distanceMatrix.txt", "w");
	for (int i = 0; i < tot; ++i)
	{
		for (int j = 0; j < tot; ++j)
			fprintf(fp, "%.6lf ", dist[i][j]);
		fprintf(fp, "\n");
	}
	fclose(fp);
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
	vector<int> ord[1000];
	vector<double> w[1000];
		FILE *fp = fopen(("./inter/"+tmp+".txt").c_str(), "r");
		int n0, m0;
		fscanf(fp, "%d %d", &n0, &m0);
		for (int i = 0; i < n0; ++i)
		{
			for (int j = 0; j < m0; ++j)
			{
				int tmp;double weight;
				fscanf(fp, "%d %lf", &tmp, &weight);
				ord[i].push_back(tmp);
				w[i].push_back(weight);
			}
		}
		fclose(fp);
	static ObjectVideo cur;
	cur.clear();
	for (int i = 0; i < n; ++i)
	{
		static ObjectFrame frame, last;
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
		fclose(fp);
		last.clear();
		for (int i = 0; i < n0; ++i)
		{
			double x[3] = {0,0,0};
			for (int j = 0; j < m0; ++j)
				for (int k = 0; k < 3; ++k)
					x[k] += w[i][j] * frame[ord[i][j]*3+k];
			for (int k = 0; k < 3; ++k)
				last.push_back(x[k]);
		}
		cur.push_back(last);
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
		cout << "readVideo " << i << endl;
		fin >> names[i] >> ans[0][i] >> ans[1][i] >> n[i] >> m[i];
		videos[i] = readVideo(names[i], n[i], m[i]);
	}
	fin.close();
}

int main()
{
	init();
	solve();
	return 0;
}
