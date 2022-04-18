#include <iostream>
using namespace std;
void func_sort (int *, int);
int main ()
{
    const int size=5;
    int MAS_one[size]={1,3,5,7,9};
    int MAS_two[size]={2,4,6,8,10};
    int MAS_rez[size*2];
 
    for (int i=0;i<size;i++)
	{
        MAS_rez[i]=MAS_one[i];
        MAS_rez[i+size]=MAS_two[i];
	}
    func_sort (MAS_rez, size*2);
    for (int i=0;i<size*2;i++)
    cout<<" "<< MAS_rez[i];
    cout<<endl;
    return 0;
}
void func_sort (int *MAS,int size)
{
    int tmp;
    for(int i=0;i<size;i++)
        for(int j=size-1;j>i;j--)
        if (MAS[j-1]>MAS[j])
		{
      tmp=MAS[j-1];
	  MAS[j-1]=MAS[j];
	  MAS[j]=tmp;
		}
}
