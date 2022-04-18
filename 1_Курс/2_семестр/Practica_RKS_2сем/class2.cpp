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
    return is;
}

bool operator < (const Name &s1,const Name &s2)
{
	if (s1.b < s2.b){
		return true;
	}
	if(s1.b == s2.b){
		return s1.a < s2.a;
	}
		return false;
}

bool compare(const char* s1 ,const char* s2,int max)//сначало это
{
	for(int  i = 0;i < max;i++)
	{
		if (s1[i] < s2[i])
			return true;
		
		if(s1[i] > s2[i])
			return false;
//----------------------------

		if(s1[i] == 0 && s2[i] == 0)
			break;
	}
	return false;
}

int main()
{
    Name *d_array;
    Name s;
    int k=0;
    ifstream file ("assessment.txt");
    while(file.good()){

        if(!(file >> s))
            break;
        k++;
        file.get();

    }

    file.clear();

    file.seekg(0);
    
    cout<<"ARRAY DO: ";
    d_array = (Name*)malloc(k*sizeof(Name));
        for(int i=0;i<k;++i)
        {
            file>>d_array[i];
            file.get();
            cout<<d_array[i]<<" ";
        }
        
    cout<<"\nARRAY AFTER: ";
        for(int i=0;i<k;i++)
        {
        	
            cout<<d_array[i]<< " ";
        }
        cout<<endl;
        
    free(d_array);
    file.close();
return 0;
}
