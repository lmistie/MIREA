#include <iostream>

using namespace std;

int main()
{
struct TPerson 
{
string Name; 
int Year;
double Height;
bool Active;
};
TPerson Person; 
Person.Name = "John"; 
Person.Year = 1995;
Person.Height = 175.4;
Person.Active = true;
cout << Person.Name << "\n"; 
cout << Person.Year << "\n";
cout << Person.Height << "\n";
cout << Person.Active;
return 0;
}

