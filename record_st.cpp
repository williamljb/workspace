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

double ans;

void solve()
{
	ofstream fout;
	fout.open("log.txt",std::ofstream::out | std::ofstream::app);
	fout << ans << endl;
	fout.close();
}

void init()
{
	ans = -1e10;
	for (int i = 0; i < 100; ++i)
	{
		string names = "";
		int x = i;
		while (x)
		{
			names = char(x%10+'0')+names;
			x /= 10;
		}
		while (names.size() < 4)
			names = "0" + names;
		ifstream fin(("./out/"+names+"_00.obj").c_str());
		string tmp;
		while (1)
		{
			getline(fin, tmp);
			if (tmp == "") break;
			if (tmp[0] != 'v' || tmp[1] == 't') continue;
			stringstream sin(tmp.substr(2, tmp.size()-2));
			double x,y,z;
			sin >> x >> y >> z;
			if (ans < y)
				ans = y;
		}
		fin.close();
	}
}

int main()
{
	init();
	solve();
	return 0;
}