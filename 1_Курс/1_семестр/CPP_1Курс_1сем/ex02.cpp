#include <iostream>
using namespace std;
double power(double x, int n);
int main()
{
	setlocale(LC_ALL, "Russian");
	double x;
	int n;
	cout<<"\nВВедите число - ";
	cin>>x;
	cout<<"\nВВедите степень - ";
	cin>>n;
	double  xn=power(x,n);
	cout<<"\nЧисло "<<x<<" в степени "<<n<<" = "<<xn<<"\n\n";
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
