#include <iostream>
#include <string>
using namespace std;
int main()
{
 setlocale(LC_ALL, "Russian");
 string Text = "";
 string s;
 cout << "Вводите текст, а в конце наберите Ctrl+Z:" << endl;
 while(getline(cin, s))
 {
 Text = Text + s + "\n";
 }
 cout << "\nСпасибо!" << endl;
 cout << "Вы ввели:" << endl;
 cout << Text;
 return 0;
}		
