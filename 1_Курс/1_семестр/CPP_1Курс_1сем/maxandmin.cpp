#include <iostream> 
 
using namespace std; 
 
int main() {     setlocale(LC_ALL, "Russian");// устанавливаем русскую кодировку 
 
 int N = 10; // количество элементов 
 
 int A[N]; 
 
 A[0] = 3;  A[1] = 2;  A[2] = -2;  A[3] = 0;  A[4] = 10;  A[5] = 3;  A[6] = -4; 
 A[7] = 5;  A[8] = 9;  A[9] = -1; 
 
 int min = A[0]; // берем первый элемент массива  int max = A[0]; 
 
 for (int i = 1; i < N; i++) // перебираем все элементы  {   if (A[i] < min)         {             min = A[i]; // обновляем минимальный элемент         }         if (A[i] > max)         {             max = A[i];         }  } 
 
 cout << "Минимальный элемент = " << min << endl;  cout << "Максимальный элемент = " << max << endl; 
 
 return 0; } 
 
