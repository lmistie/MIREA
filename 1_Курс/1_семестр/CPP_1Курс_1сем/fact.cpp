#include <iostream>
using namespace std;

int fact(int N);
int main()
{
 cout << fact(5);
 return 0;
}
int fact(int N)
{
 if (N == 1)
 	return 1;
 else
 	return N * fact(N - 1);
}
