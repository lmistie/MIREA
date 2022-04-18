#include <iostream>
#include <string.h>
using namespace std;
int main()
{
	char a[20] = "Kelem", b[20] = ""; 
		int n = strlen(a); 
		int j = 0;
	for(int i = n - 1; i >= 0; i--)
	{
		b[j] = a[i];
		j++;
   	}
	cout << b;
return 0;
}
