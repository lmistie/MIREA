#include <iostream>
#include <string>
using namespace std;
int main()
{
 setlocale(LC_ALL, "Russian");
int a, b, c;
cout << "Введите три числа >";
cin >> a >> b >> c;
if (!(a > b))
{
cout << "\nЧисло a не больше b" << endl;
}
else
{
cout << "\nЧисло a больше b" << endl;
}
if (((a == b)&&(c < b))||(a < 0))
{
cout << "\nСложное выражение выполнено" << endl;
}
else
{
cout << "\nСложное выражение не выполнено" << endl;
}
 return 0;
}
