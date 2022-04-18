#include <iostream>
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
  setlocale(0, "rus");
 
  int n;
  int *d_array;
  
  
  cout << "¬ведите количество чисел, которое вы хотите ввести: ";
  cin >> n;
  
  cout << "¬ведите " << n << " чисел: "<< endl;
  
  d_array =(int*)malloc(n*sizeof(int)); //  new int[n];
  
  for(int i=0;i<n;++i){
  	cin >> d_array[i];
  }
  
  
  qsort(d_array,0,n);
  
  //вывод
	cout<<"—ортировка по возрастанию: ";
	for(int i=0;i<n;i++)
	{
		cout<<d_array[i]<< " ";
	}
	cout<<endl;
  free(d_array);
  
  return 0;
}
