#include <iostream>
#include <sstream>
#include <cstring>
#include <string>
#include <vector>
#include <map>

using namespace std;

int lookUp(map<int, string> &numbers, string &str)
{
	for (int i = 1; i < 10; i++)
		if (numbers[i] == str)
			return i;
	return 0;
};

int main()
{
    string str("one,nine"), s;
    istringstream input(str);
    map<int, string> numbers{{1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}, {5, "five"}, {6, "six"}, {7, "seven"}, {8, "eight"}, {9, "nine"}};
    vector<int> v; 
    while(getline(input, str, ','))
    {
        int i = lookUp(numbers, str);
        v.push_back(i); 
    }
   for (auto elem : v)
        cout << elem << ' ';
    cout << endl;
}

