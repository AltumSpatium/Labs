#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

int main()
{
	string str = "";
	cin >> str;
	int *arr = new int[str.size()];
	for (int i = 0; i < str.size(); i++)
	{
		arr[i] = str[i] - '0';
	}

	if (next_permutation(arr, arr + str.size()) == 0)
		cout << "-1";
	else
	{
		string res = "";
		for (int i = 0; i < str.size(); i++)
		{
			res += arr[i] + '0';
		}
		cout << res;
	}
	

}