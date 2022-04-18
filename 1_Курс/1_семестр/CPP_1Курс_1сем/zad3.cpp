#include <iostream>
using namespace std;
const int N = 7;
int A[N][N]; // матрица смежности
void Go(int v); // функция обхода дерева
int main()
{
// сначала заполним ее нулем
for (int i = 0; i < N; i++)
{
for (int j = 0; j < N; j++)
{
A[i][j] = 0;
}
}
// отметим дуги в дереве
A[0][1] = 1;
A[0][2] = 1;
A[1][3] = 1;
A[1][4] = 1;
A[2][5] = 1;
A[2][6] = 1;
Go(0);
return 0;
}

void Go(int v)
{
 cout << v << endl; // отметим, что были в этой вершине
 // перебираем все вершины
 for (int i = 0; i < N; i++)
 {
 if (A[v][i] == 1)
 {
 Go(i); // если есть дуга в вершину i, то обходим
 }
 }
}
