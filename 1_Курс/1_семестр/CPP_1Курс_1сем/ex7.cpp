#include <iostream>
#include <string>
using namespace std;
int main()
{
 setlocale(LC_ALL, "Russian");
 string s;
 s = "0123456789";
s.erase(3, 5);
 cout << s << endl;
 return 0;
}
