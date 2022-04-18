#include <iostream>
#include <string>
using namespace std;
int main()
{
 setlocale(LC_ALL, "Russian");
 string s;
 cout << "введите строку > ";
 getline(cin, s);
 cout << "\nСпасибо!" << endl;
 cout << "Вы ввели: \"" << s << "\"" << endl;
 return 0;
}
