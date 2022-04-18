//#include <iostream>
//#include <string>
//#include <fstream>
//
//using namespace std;
//
//class Name {
//	public:
//		char a[20];
//		int b;
//
//};
//
//ostream& operator<<(ostream &os, const Name& s)
//{
//    return os  << "[" << s.a << " | "  << s.b << "]";
//}
//int main()
//{
//	setlocale(LC_ALL,"rus");
//	Name s;	
//	int k=0;
//	ifstream file ("katya.txt");
////	while(!file.eof()){
//		file>>s.a;
//	file.get(s.a, 20,',');
////		cout<<s.a<<"\n";
//	//cout<<"pos"<<file.tellg()<<endl;
//		file.get();
//		file >> s.b;
//		k++;// Считаю кол-во строк в файле
////	}
//	cout<<"Строчек = "<<k<<"\n";
//	
////	file >> s;
//	
//	cout<<s<<"\n";//вывод объекта
//	return 0;
//}

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class Name {
	public:
		char a[20];
		int b;

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
int main()
{
	setlocale(LC_ALL,"rus");
	Name s;
	
	int k=0;
	ifstream file ("katya.txt");
	

	
//	while(!file.eof()){
//		file>>s.a;
//	file.get(s.a, 20,',');
////		cout<<s.a<<"\n";
//	//cout<<"pos"<<file.tellg()<<endl;
//	file.get();
//	file >> s.b;
		k++;// Считаю кол-во строк в файле
//	}
	cout<<"Строчек = "<<k<<"\n";
	
	
	
	file >> s;
	cout<<s<<"\n";//вывод объекта

	return 0;
}
