#include <iostream>
#include <string>
using namespace std;
int main()
{
 setlocale(LC_ALL, "Russian");

 int a;
 cout << "������� ����� > ";
 cin >> a;

 a >= 0 ? cout << "�������������� �����" : cout <<
"������������� �����?";
 return 0;
}

