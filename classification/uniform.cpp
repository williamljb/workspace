#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <string>
#include <utility>
#include <omp.h>
#include <algorithm>
#include <cmath>
#include <sstream>
using namespace std;

#define sqr(x) ((x)*(x))
struct point
{
	double x,y,z;
	point(double x0=0,double y0=0,double z0=0):x(x0),y(y0),z(z0){}
};
point operator-(const point& a,const point &b){return point(a.x-b.x,a.y-b.y,a.z-b.z);}
point operator+(const point& a,const point &b){return point(a.x+b.x,a.y+b.y,a.z+b.z);}
point operator*(const point& a,double b){return point(a.x*b,a.y*b,a.z*b);}
double operator*(const point& a,const point &b){return a.x*b.x+a.y*b.y+a.z*b.z;}

int tot, ans[2][1000], NN[1000], n[1000], m[1000];
string names[1000];
const int part = 30;
const int num = 5;
int order[10000];
double dis[10000], weight[num];
vector<point> mesh;

bool cmp(int a, int b){return dis[a] < dis[b] - 1e-8;}

void doit(string names)
{
	ifstream fin(("../obj_input/"+names+"/0000_00.obj").c_str());
	string tmp;
	mesh.clear();
	while (1)
	{
		getline(fin, tmp);
		if (tmp == "") break;
		if (tmp[0] != 'v' || tmp[1] == 't') continue;
		stringstream sin(tmp.substr(2, tmp.size()-2));
		point cur;
		sin >> cur.x >> cur.y >> cur.z;
		mesh.push_back(cur);
	}
	fin.close();
	ofstream fout(("./inter/"+names+".txt").c_str());
	fout << part*part << " " << num << endl;
	point dx = (mesh[1]-mesh[0])*(1.0/(part-1)), dy = (mesh[3]-mesh[0])*(1.0/(part-1));
	for (int i = 0; i < part; ++i)
		for (int j = 0; j < part; ++j)
		{
			point cur = mesh[0] + dx * i + dy * j;
			for (int k = 0; k < mesh.size(); ++k)
			{
				order[k] = k;
				dis[k] = sqr(cur-mesh[k]);
			}
			sort(order, order+mesh.size(), cmp);
			double sum = 0, s2 = 0;
			for (int k = 0; k < num; ++k)
			{
				sum += dis[order[k]];
				s2 += sqr(dis[order[k]]);
			}
			s2 = s2 / num - sqr(sum/num);
			sum = 0;
			for (int k = 0; k < num; ++k)
				sum += (weight[k] = exp(-sqr(dis[order[k]])/s2));
			for (int k = 0; k < num; ++k)
			{
				fout << order[k] << " " << weight[k] / sum << " ";
			}
			fout << endl;
		}
	fout.close();
}

string int2str(int x, int k)
{
	string ans = "";
	while (x) {ans = char('0'+x%10)+ans;x/=10;}
	while (ans.size() < k)
		ans = "0"+ans;
	return ans;
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
		doit(names[i]);
	}
	fin.close();
}

int main()
{
	init();
	return 0;
}
