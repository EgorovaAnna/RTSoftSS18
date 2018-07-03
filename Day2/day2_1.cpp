#include <iostream>
#include <vector>

using namespace std;

int main()
{
	string str;
	cin >> str;
	vector<int> v; 
	int a = 0;
	for (int i = 0; i < str.size(); i++)
	{
		if (!((int)str[i] == (int)','))
		{
			a = a*10 + (int)str[i] - (int)'0';
		}
		else
		{
			v.push_back(a);
			a = 0;
		}
	}
	v.push_back(a);
	for (int i = 0; i < v.size(); i++)
		cout << v[i] << ' ';
	cout << endl;
}
