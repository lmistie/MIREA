#include <fstream>
#include <iostream>
#include <cstdlib>
using namespace std;
 
int main()
{
    ifstream file("numbers.txt"); // ������� ���� ��� ������
	int c;
	
	int k=0;
	int *d_array;
	while(!file.eof()){
		file>>c;
		cout<<c<<"\n";
		k++;// ������ ���-�� � �����
	}
	cout<<"Numbers = "<<k<<"\n";
	
	file.seekg(0);//��������� ��� �� ����!
	
							//  ����� � ������
	cout<<"Array: ";
	d_array = (int*)malloc(k*sizeof(int));
	for(int i=0;i<k;++i)
	{
		file>>d_array[i];
		cout<<d_array[i]<<" ";
	}
	
	file.close();
	
	return 0; 
}
