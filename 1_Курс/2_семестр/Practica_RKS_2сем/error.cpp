#include <iostream>
#include <fstream>
 
using namespace std;
 
int main()
{
    ifstream fin("numbers.txt");
    //ofstream fout("output.txt");
    int N, k;
    int Array[4];
    if (!fin.good())
    {
        cout << "ERROR!!!" << endl;
    }
    else
    {
        k = 0;
        while (fin >> N)     //считываем значение в переменную
        {
            Array[k++] = N;
        }
        cout << "Array:" << endl;
        for (int i = 0; i < k; i++)
        {
            cout << Array[i] << " ";
        }
    }
    fin.close();
    //fout.close();
    cin.get();
    return 0;
}
