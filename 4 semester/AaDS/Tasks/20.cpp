#include <iostream>
#include <vector>
#include <string>
using namespace std;

int substrings(string s) {
	int n = s.size();
	s = s + ' ';
	vector<vector<int>> a(128);
	int num = 0;
	for (int i = n - 1; i >= 0; --i)
	{
		int lev = 0;
		for (int st = (int)a[s[i]].size() - 1; st >= 0; --st)
		{
			int j = a[s[i]][st];
			if (n - j <= lev)
				break;
			if (s[j + lev] != s[i + lev])
				continue;
			if (s[j + lev / 2] != s[i + lev / 2])
				continue;
			int k = 0;
			while (s[j + k] == s[i + k])
				++k;
			if (k > lev)
				lev = k;
		}
		a[s[i]].push_back(i);
		num += n - i - lev;
	}
	return num;
}

int main()
{
	string s = "";
	cin >> s;
	cout << substrings(s) << endl;
}