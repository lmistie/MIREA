#include <iostream>
#include <cstdlib>
using namespace std;

int main() {
  setlocale(0, "");
 
  int n;
 
  cout << "¬ведите количество чисел, которое вы хотите ввести: ";
  cin >> n;
 
  cout << "¬ведите " << n << " чисел: "<< endl;
                                        
  int *dinamich_array = new int [n];    // создаем
                                        // динамический массив
  for (int i = 0; i < n; i++) { 
    cin >> dinamich_array[i];  // считываем числа в €чейки массива
  }
 
  cout << "“еперь давайте выведем элементы массива в обратном пор€дке: ";
 
  for (int i = n - 1 ; i >= 0; i--) {
    cout << dinamich_array[i] << " ";  // выводим значение всех €чеек
  }
 
  cout << endl << "”дал€ем массив!";
 
  free(dinamich_array);  // удал€ем динамический массив
 
  return 0;
}
