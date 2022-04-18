#include <iostream>
using namespace std;
void func(int x);
int main()
{
	setlocale(LC_ALL, "Russian");
	double x=5;
	cout<<"До вызова функции = "<<x;
	func(x);
	cout<<"\n\nПосле вызова функции x = "<<x;
	cout<<"\n\n";
	return 0;
}
void func(int x)
{
	x=3;
	cout<<"\n\nПосле изменения вызова внутри функции x = "<<x;
}
