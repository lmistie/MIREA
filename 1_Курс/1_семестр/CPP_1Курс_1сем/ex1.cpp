#include <iostream>
#include <string>
using namespace std;
int main()
{
 setlocale(LC_ALL, "Russian");
 string s;
 cout << "������� ������ > ";
 getline(cin, s);
 cout << "\n�������!" << endl;
 cout << "�� �����: \"" << s << "\"" << endl;
 return 0;
}
