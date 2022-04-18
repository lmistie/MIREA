#include <iostream>
#include <string>
using namespace std;
int main()
{
 setlocale(LC_ALL, "Russian");
 string s;
 string sub;
 s = "One Two Three";
 sub = s.substr(4, 3);
 cout << sub << endl;
 sub = s.substr(4);
 cout << sub << endl;
 return 0;
}
