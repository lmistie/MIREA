#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

void qsort(int *array,int low,int high)
{
	int mid;
	mid = array[(low + high) / 2];
	int i = low - 1;
	int j = high;
	while(1)
	{
		do{ ++i;	} while(array[i] < mid);
		do{ --j;	} while(array[j] > mid);
		if(i >= j) break;
		
		int abc = array[i];
 		array[i] = array[j];
  		array[j] = abc;
	}
	
	if (high - low <= 2)
		return;
		
	qsort(array,low ,i);
	qsort(array,j,high);
}


int main()
{
	setlocale(LC_ALL, "Rus");

	char file_name[20];

	ifstream file;
		do
		{
			cout << "Введите имя файла: ";
    		cin >> file_name;
    		cout<<file_name<<"\n";
    		file.open(file_name);
    	if (file.good())
    		break;
		cout << "Файл не найден.";
    	}
	while (true);


	cout << "\n";
	int c;
	int k=0;
	int *d_array;
		while (!file.eof())
		{
			file>>c;
			cout<<c<<"\n";
			k++;
		}
	cout <<" \nКоличество = " << k << "\n";

	file.seekg(0);
	
	cout<<"Массив: ";
	d_array = (int*)malloc(k*sizeof(int));
		for(int i=0;i<k;++i)
		{
			file>>d_array[i];
			cout<<d_array[i]<<" ";
		}
	
	qsort(d_array,0,k);
	
	cout<<"\nСортировка по возрастанию: ";
		for(int i=0;i<k;i++)
		{
			cout<<d_array[i]<< " ";
		}
		cout<<endl;
  free(d_array);
	
file.close();

return 0;
}
