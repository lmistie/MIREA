#include <iostream>
#include <string>
using namespace std;
int main()
{
 setlocale(LC_ALL, "Russian");
 string Text = "";
 string s;
 cout << "������� �����, � � ����� �������� Ctrl+Z:" << endl;
 while(getline(cin, s))
 {
 Text = Text + s + "\n";
 }
 cout << "\n�������!" << endl;
 cout << "�� �����:" << endl;
 cout << Text;
 return 0;
}		
