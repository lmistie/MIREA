#include <iostream>
using namespace std;
int main()
{
	setlocale(LC_ALL,"Russian");
        const int arraySize = 10;                  
        int A[arraySize];
        cout<<"������� ������ �� 10 �����. \n";
         for (int j=0; j<arraySize; j++)
     {
              cout<<"A["<<j<<"]= ";
       cin>>A[j]; 
     }
        int Sum = 0; 
        for (int j=0; j<arraySize; j++)
          {
            if(A[j]!=A[9])
       {
                Sum += A[j];
       }
    }
        cout << "����� �������� ��������� ������� = " << Sum << '\n';
}
