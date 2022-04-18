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
 	const int n=6;

	int student[n]={5,8,2,3,0,1};
	cout<<"element now: ";
	for(int i=0;i<n;i++)
	{
		cout<<student[i]<<" ";
	}
	cout<<endl;
	 	
	qsort(student,0,n);
	
	//вывод
	cout<<"after sorting: ";
	for(int i=0;i<n;i++)
	{
		cout<<student[i]<< " ";
	}
	cout<<endl;
	return 0;
 }
