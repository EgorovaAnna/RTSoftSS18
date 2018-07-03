// Paste your solution here if you're done.
#include <iostream>
#include <map>

int lookUp(std::map<std::string, int> m, std::string str){
    return m.find(str) == m.end() ? 0 : m[str];
}

int main(){
    std::string str;
    std:: cin >> str;

    std::map<std::string, int> m{};

    m.insert(std::pair<std::string,int>("one",1));
    m.insert(std::pair<std::string,int>("two",2));
    m.insert(std::pair<std::string,int>("three",3));
    m.insert(std::pair<std::string,int>("four",4));
    m.insert(std::pair<std::string,int>("five",5));
    m.insert(std::pair<std::string,int>("six",6));
    m.insert(std::pair<std::string,int>("seven",7));
    m.insert(std::pair<std::string,int>("eigth",8));
    m.insert(std::pair<std::string,int>("nine",9));

    std::cout << lookUp(m, str) << std::endl;
    return 0;
}

