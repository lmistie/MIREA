#include <iostream>
using namespace std;
void fn(float *f)
{
	*f=*f * 2;
	return;
}
int main ()
{
	float a = 4;
	fn(&a);
	cout<<a;
	return 0;
}
