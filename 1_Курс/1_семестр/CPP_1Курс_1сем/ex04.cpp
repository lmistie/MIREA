#include <iostream>
using namespace std;
void func();
int a=5;
int main()
{
	setlocale(LC_ALL, "Russian");
	cout<<"\n\n�������� � = "<<a;
	a=6;
	cout<<"\n\n�������� ����� ��� � ���� � = "<<a;
	cout<<"\n\n";
	return 0;
}
void func()
{
	a++;
}
