#include <iostream>
using namespace std;
int main()
{
	struct TPerson
	{
		string names;
		int year;
		double height;
		bool active;
	};
	
	TPerson Person;
	
	Person.names ="john";
	Person.year=1995;
	Person.height=175.4;
	Person.active=1;
	
	cout <<Person.names<<"\n"<<Person.year<<"\n"<<Person.height<<"\n"<<Person.active;
	
	return 0;
}
