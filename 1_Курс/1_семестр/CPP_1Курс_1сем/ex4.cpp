#include <iostream>
#include <string>
using namespace std;
int main()
{
 setlocale(LC_ALL, "Russian");
 string s;
 string sub;
 int pos;
 cout << "������� ������ > ";
 getline(cin, s);
 cout << "\n������� ��������� > ";
 getline(cin, sub);
 pos = s.find(sub);
 if (pos < 0)
 {
 cout << "\n��������� �� �������!" << endl;
 }
 else
 {
 cout << "\n��������� ������� �� " << pos << "-� �������!"
<< endl;
 }
 return 0;
}


