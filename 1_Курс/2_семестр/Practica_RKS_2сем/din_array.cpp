#include <iostream>
 
using namespace std;
 
int main() {
  setlocale(0, "");
  int *a = new int;
 
  int b = 10;
 
  *a = b;
 
  cout <<"������ ���������� a = "<< *a << endl;
 
  cout <<"������� ���������.";
 
  return 0;
}
