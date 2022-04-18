#include <iostream>
using namespace std;
void func();
int a=10;
int main()
{
	setlocale(LC_ALL, "Russian");
	int a;
	a=-10;
	cout<<"\nÂ main a = "<<a;
	func();
	cout<<"\n Â main a = "<<a;	
}
void func()
{
	cout<<"\n â func";
	a++;
}
