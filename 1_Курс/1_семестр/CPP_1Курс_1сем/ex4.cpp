#include <iostream>
#include <string>
using namespace std;
int main()
{
 setlocale(LC_ALL, "Russian");
 string s;
 string sub;
 int pos;
 cout << "Введите строку > ";
 getline(cin, s);
 cout << "\nВведите подстроку > ";
 getline(cin, sub);
 pos = s.find(sub);
 if (pos < 0)
 {
 cout << "\nПодстрока не найдена!" << endl;
 }
 else
 {
 cout << "\nПодстрока найдена на " << pos << "-й позиции!"
<< endl;
 }
 return 0;
}


