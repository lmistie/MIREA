#include <iostream>
using namespace std;
const int Infty = 9999;
const int N = 6;
const int S = 0;
const int T = 5;
int A[N][N];
int M[N];
bool P[N];
void Go();
int main()
{
for (int i = 0; i < N; i++)
{
for (int j = 0; j < N; j++)
{
A[i][j] = 0;
}
}
A[0][1] = 11;
A[0][3] = 12;
A[0][4] = 26;
A[1][4] = 16;
A[1][2] = 15;
A[2][5] = 20;
A[3][4] = 13;
A[4][5] = 14;
for (int n = 0; n < N; n++)
 {
 M[n] = Infty;
 P[n] = false;
 }
 M[S] = 0;
 P[S] = true;
 for (int n = 0; n < N; n++)
 {
 if (A[0][n] != 0)
 {
 M[n] = A[0][n];
 }
 }
 Go();
 for(int n = 0; n < N; n++)
 {
 if (P[n])
 {
 cout << "Vertex = " << n << "\t distance = " << M[n] << endl;
 }
 }
return 0;
}
void Go()
{
 int min_M = Infty;
 int R;
 for (int n = 0; n < N; n++)
 {
 if ((!P[n]) && (M[n] < min_M))
 {
 min_M = M[n];
 R = n;
 }
 }
 P[R] = true;
 if (R == T)
 {
 return;
 }
 for (int n = 0; n < N; n++)
 {
 if ((A[R][n] != 0) && (!P[n]))
 {
 int D = A[R][n] + M[R];
 if (D < M[n])
 {
 M[n] = D;
 }
 }
 }
 Go();
}
