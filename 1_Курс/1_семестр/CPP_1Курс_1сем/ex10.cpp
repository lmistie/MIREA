#include <iostream>
#include <string>
using namespace std;
int main()
{
 setlocale(LC_ALL, "Russian");
int a, b, c;
cout << "������� ��� ����� >";
cin >> a >> b >> c;
if (!(a > b))
{
cout << "\n����� a �� ������ b" << endl;
}
else
{
cout << "\n����� a ������ b" << endl;
}
if (((a == b)&&(c < b))||(a < 0))
{
cout << "\n������� ��������� ���������" << endl;
}
else
{
cout << "\n������� ��������� �� ���������" << endl;
}
 return 0;
}
