#include <iostream>
#include <string>
#include <sstream>
using namespace std;
string int_to_s(int a);
string number(string s, int n, int k);
int main()
{
 setlocale(LC_ALL, "Russian");
 cout << number("", 35, 3);
return 0;
}
string int_to_s(int a)
{
 stringstream s;
 s.str("");
 s << a;
 return s.str(); 
}
string number(string s, int n, int k)
{
 int n_on_k, n_mod_k;
 n_on_k = n / k;
 n_mod_k = n % k;
 if (n_on_k == 0)
 
 return int_to_s(n_mod_k) + s;
 
 return number(int_to_s(n_mod_k) + s, n_on_k, k);
}

