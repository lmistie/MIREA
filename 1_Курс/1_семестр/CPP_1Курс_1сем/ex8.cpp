#include <iostream>
#include <string>
using namespace std;
int main()
{
 setlocale(LC_ALL, "Russian");
int N = 3;
string s[N];
for (int n = 0; n < N; n++)
{
cout << "\n¬ведите слово > ";
cin >> s[n];
}
for (int n = 0; n < N; n++)
{
cout << s[n] << "\t";
}
cout << endl;

 return 0;
}
