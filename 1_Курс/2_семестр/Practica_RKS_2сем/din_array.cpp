#include <iostream>
 
using namespace std;
 
int main() {
  setlocale(0, "");
  int *a = new int;
 
  int b = 10;
 
  *a = b;
 
  cout <<"Теперь переменная a = "<< *a << endl;
 
  cout <<"Удаляем переменую.";
 
  return 0;
}
