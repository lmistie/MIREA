#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <memory.h>
#include <fstream>

using namespace std;

class Name {
    public:
        char a[20];
        int b;
        Name(){ memset(a,0,20); }
};

ostream& operator<<(ostream &os, const Name& s)
{
    return os  << "[" << s.a << " | "  << s.b << "]";
}
istream& operator >> (istream &is,Name& s)
{
    is.get(s.a,20,',');
    is.get();
    is >> s.b;
//	is.get();
    return is;
}
int main()
{
    setlocale(LC_ALL,"rus");
    Name *d_array;
    Name s;
    int k=0;
    ifstream file ("assessment.txt");
    while(file.good()){
//		cout<<"Количество строк = "<<k<<"\n";
        if(!(file >> s))
            break;
        k++;
        file.get();
//		cout<<s<<"\n";
    }

//    if(file.eof())
    file.clear();

    file.seekg(0);
    cout<<"Массив: ";
    d_array = (Name*)malloc(k*sizeof(Name));
        for(int i=0;i<k;++i)
        {
            file>>d_array[i];
            file.get();
            cout<<d_array[i]<<" ";
        }
    cout<<"\nМассив после: ";
        for(int i=0;i<k;i++)
        {
            cout<<d_array[i]<< " ";
        }
        cout<<endl;
    free(d_array);
    file.close();
return 0;
}
