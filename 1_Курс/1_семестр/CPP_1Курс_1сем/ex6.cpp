#include <iostream>
#include <string>
using namespace std;
int main()
{
 setlocale(LC_ALL, "Russian");
 string s;
 string sub;
 s = "One Three";
 sub = "Two ";
s.insert(4, sub);
 cout << s << endl;
 return 0;
}

