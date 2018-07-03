#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

int main()
{
    std::string str("1,2,3,4,5,6");
    std::istringstream input(str);
    
    vector<int> v; 
    while(std::getline(input, str, ','))
    {
        int i = stoi(str.c_str());
        v.push_back(i); 
    }
    
   for (auto elem : v)
        cout << elem << ' ';
    cout << endl;
}

