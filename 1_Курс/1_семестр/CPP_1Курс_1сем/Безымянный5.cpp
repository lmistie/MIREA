#include <iostream> 
 
#include "func.h" // ���������� ������������ ���� � ������� �������� 
using namespace std;
int main()
{ 
 setlocale(LC_ALL, "Russian");
  cout << "����������� �������� ���������\n\n";
  cout << "������� ����� > ";
  int n;
  cin >> n;
  int m;		
  m = func(n);   
  cout << "\n��������� ������ ������� �������: " << m << "\n\n";					   
  return 0;
}
