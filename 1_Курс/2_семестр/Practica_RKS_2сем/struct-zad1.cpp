#include <iostream>

using namespace std;
int main()
{
	
	cout << "sorting array"<< "\n" << endl;
    int array[4] = { 3, 5, 1, 9 };
    cout << "indeks" << "\t\t" << "element array" << endl;
    for (int counter = 0; counter < 4; counter++)
    {
	
     cout << "array[" << counter << "]" << "\t\t" << array[counter] << endl;
    }
	
	
	return 0;
}
