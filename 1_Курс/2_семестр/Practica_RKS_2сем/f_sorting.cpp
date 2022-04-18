 #include <iostream>
 using namespace std;
 void sorting(int n,int *student)
 {
 	for(int i=1;i<n;i++)
	{
		for(int m=0;m<n;m++)
		{
			if (student[m] < student[m+1])
			{
				int abc = student[m];
				student[m] = student[m+1];
				student[m+1] = abc;
			}
		}
	}
 }
 
 int main()
 {
 	const int n=3;
	
	int student[n];
	for(int i=0;i<n;i++)
	{
		cout<<i+1<<" element: ";
		cin>>student[i];
	}
	
	cout<<" element now: ";
	for(int i=0;i<n;i++)
	{
		cout<<student[i]<<" ";
	}
	cout<<endl;
	
	sorting(n,student);
	
	//вывод
	cout<<" after sorting: ";
	for(int i=0;i<n;i++)
	{
		cout<<student[i]<< " ";
	}
	cout<<endl;
	
	return 0;
 }
