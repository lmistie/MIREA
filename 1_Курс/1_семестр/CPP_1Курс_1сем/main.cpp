#include <iostream>   

using namespace std;

#include "func.h"

int main()
{ 
 setlocale(LC_ALL, "Russian");
  cout << "Главная функция\n\n";
  cout << "enter nmb > ";
  int n;
  cin >> n;
  int m;		
  m = func(n);   
  cout << "\nres: " << m << "\n\n";					   
  return 0;
}
