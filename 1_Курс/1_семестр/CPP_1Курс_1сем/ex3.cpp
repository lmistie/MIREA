#include <iostream>
#include <string>
using namespace std;
int main()
{
 setlocale(LC_ALL, "Russian");
 string s;
 cout << "������� ������ > ";
 getline(cin, s);
 cout << endl << "� ���� ������ " << s.length() << " ��������"
<< endl;
 for (int i = 0; i < s.length(); i++)
 {
 cout << i << "-� �������: " << s[i] << endl;
 }
 return 0;
}
