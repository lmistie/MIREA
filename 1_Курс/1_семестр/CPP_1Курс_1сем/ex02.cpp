#include <iostream>
using namespace std;
double power(double x, int n);
int main()
{
	setlocale(LC_ALL, "Russian");
	double x;
	int n;
	cout<<"\n������� ����� - ";
	cin>>x;
	cout<<"\n������� ������� - ";
	cin>>n;
	double  xn=power(x,n);
	cout<<"\n����� "<<x<<" � ������� "<<n<<" = "<<xn<<"\n\n";
	return 0;
}
double power(double x,int n)
{
	double res=1;
	for(int i=0;i<n;i++)
	{
		res=res*x;
	}
	return res;
}
