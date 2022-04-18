#include <iostream>

using namespace std;
int main()
{
struct TItem
{
string Name; 
int Number;
};
const int N = 3; 
TItem Items[N];
for (int i = 0; i < N; i++) 
{
cout << "Enter Name > ";
cin >> Items[i].Name;
cout << "Enter Number > ";
cin >> Items[i].Number;
}
cout << "\n\n";
for (int i = 0; i < N; i++)
{
cout << "Name : " << Items[i].Name << "\n";
cout << "Number : " << Items[i].Number << "\n";
}
return 0;
}
