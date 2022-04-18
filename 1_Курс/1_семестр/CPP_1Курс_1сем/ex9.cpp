#include <iostream>
#include <string>
using namespace std;
int main()
{
 setlocale(LC_ALL, "Russian");

 int a;
 cout << "Введите число > ";
 cin >> a;

 a >= 0 ? cout << "Неотрицательно число" : cout <<
"Отрицательное число?";
 return 0;
}

