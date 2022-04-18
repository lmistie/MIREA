#include <iostream>
#include <string>
using namespace std;
int main()
{
 setlocale(LC_ALL, "Russian");
 string s;
 cout << "введите строку > ";
 getline(cin, s);
 cout << endl << "в этой строке " << s.length() << " символов"
<< endl;
 for (int i = 0; i < s.length(); i++)
 {
 cout << i << "-й элемент: " << s[i] << endl;
 }
 return 0;
}
