#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;
int main()
{
	setlocale(LC_ALL,"rus");
	char file_name[100]; // putchar ;enter - break 
	cout << " ������� ��� ����� fail.txt:\t";
	cin >> file_name;
	if(!file_name)
	{
	do {
    cout << "\n���� �� ������.\n"
    << "������� ��� �����: ";
    cin >> file_name;
	} while (true);
	}
    
//    file.open(file_name);
//    if (file.good())
//    break;
//    } while (true);
	
    ifstream file("numbers.txt"); // ������� ���� ��� ������
	int c;
	int k=0;
	int *d_array;
	while(!file.eof()){
		file>>c;
		cout<<c<<"\n";
		k++;// ������ ���-�� � �����
	}
	cout<<"���������� = "<<k<<"\n";
	
	file.seekg(0);//��������� ��� �� ����!
	
							//  ����� � ������
	cout<<"������: ";
	d_array = (int*)malloc(k*sizeof(int));
	for(int i=0;i<k;++i)
	{
		file>>d_array[i];
		cout<<d_array[i]<<" ";
	}
	
	file.close();
//	else {
//	break};
	return 0;
}
