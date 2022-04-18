#include <iostream>
 using namespace std;
int main()
{
    setlocale(LC_ALL, "Russian");
    cout<<"Таблица квадратов\nЧисло Квадрат\n";
    for(int i = 1; i <= 100; ++i)
        cout<<i<<' '<<(i * i)<<endl;
    return 0;
}
