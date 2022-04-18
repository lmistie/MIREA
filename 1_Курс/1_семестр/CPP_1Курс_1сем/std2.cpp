#include <iostream>
using namespace std;
int main()
{
const int N = 25; 
int A[N];
for (int n = 0; n < N; n++)
{
A[n] = n; 
}
int B = 0;
for (int n = 0; n < N; n++)
{
B = B + A[n]; 
}
cout << "B = " << B << "\n"; 
return 0;
}

