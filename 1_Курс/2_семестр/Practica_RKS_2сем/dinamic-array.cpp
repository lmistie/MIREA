#include <iostream>
#include <cstdlib>
using namespace std;

int main() {
  setlocale(0, "");
 
  int n;
 
  cout << "������� ���������� �����, ������� �� ������ ������: ";
  cin >> n;
 
  cout << "������� " << n << " �����: "<< endl;
                                        
  int *dinamich_array = new int [n];    // �������
                                        // ������������ ������
  for (int i = 0; i < n; i++) { 
    cin >> dinamich_array[i];  // ��������� ����� � ������ �������
  }
 
  cout << "������ ������� ������� �������� ������� � �������� �������: ";
 
  for (int i = n - 1 ; i >= 0; i--) {
    cout << dinamich_array[i] << " ";  // ������� �������� ���� �����
  }
 
  cout << endl << "������� ������!";
 
  free(dinamich_array);  // ������� ������������ ������
 
  return 0;
}
