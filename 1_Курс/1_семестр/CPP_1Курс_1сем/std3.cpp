#include <iostream>

using namespace std;
int main()
{
	const int N = 5;
	char T[N][N];
	for (int i = 0; i < N; i++)
	for (int j = 0; j < N; j++)
	T[i][j] = 'O';
	for (int n = 0; n < N; n++)
	T[n][n] = 'X';
	for (int i = 0; i < N; i++)
	{
	for (int j = 0; j < N; j++)
	cout << T[i][j] << "\t"; 
	cout << "\n"; 
	}
	return 0;
}
