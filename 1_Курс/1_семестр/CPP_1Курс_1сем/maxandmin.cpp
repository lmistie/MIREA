#include <iostream> 
 
using namespace std; 
 
int main() {     setlocale(LC_ALL, "Russian");// ������������� ������� ��������� 
 
 int N = 10; // ���������� ��������� 
 
 int A[N]; 
 
 A[0] = 3;  A[1] = 2;  A[2] = -2;  A[3] = 0;  A[4] = 10;  A[5] = 3;  A[6] = -4; 
 A[7] = 5;  A[8] = 9;  A[9] = -1; 
 
 int min = A[0]; // ����� ������ ������� �������  int max = A[0]; 
 
 for (int i = 1; i < N; i++) // ���������� ��� ��������  {   if (A[i] < min)         {             min = A[i]; // ��������� ����������� �������         }         if (A[i] > max)         {             max = A[i];         }  } 
 
 cout << "����������� ������� = " << min << endl;  cout << "������������ ������� = " << max << endl; 
 
 return 0; } 
 
