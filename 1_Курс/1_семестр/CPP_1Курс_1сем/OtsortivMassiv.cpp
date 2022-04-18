#include <iostream> 
 
using namespace std; 
 
int main() {     setlocale(LC_ALL, "Russian");// устанавливаем русскую кодировку 
 
 int N = 10; // количество элементов 
 
 int A[N]; 
 
 A[0] = 3;  A[1] = 2;  A[2] = -2;  A[3] = 0;  A[4] = 10;  A[5] = 3;  A[6] = -4;  A[7] = 5; 
 A[8] = 9;  A[9] = -1; 
 
 for (int i = 0; i < N - 1; i++) 
     {         cout << i + 1 << " проход" << endl;    
	      for (int j = 0; j < N - 1 - i; j++)        
		   {             cout << "сравниваем: A[" << j << "] и A[" << j + 1 << "]" << endl; 
		               if (A[j] > A[j + 1])       
					         {                 // меняем местами            
							      int c = A[j + 1];             
								      A[j + 1] = A[j];          
									         A[j] = c;         
											     }       
												   }  
												      } 
    cout << "\nОтсортированный массив:" << endl;   
	  for (int i = 0; i < N; i++)    
	   {         cout << A[i] << "\t"; 
	       } 
 return 0;
 }
