//#include <iostream>
//using namespace std;
//void qsort(int *array, int low, int high)
//{


//    int mid;
//    mid = array[(low + high) / 2];
//    int i = low - 1;
//    int j = high;
//    while(1)
//    {
//        do{ ++i; } while(array[i] < mid);
//        do{ --j; } while(array[j] > mid);
//        if(i >= j)
//            break;

//        int abc=array[i];
//        array[i]=array[j];
//        array[j]=abc;
//    }

//    if(high - low <= 2)
//        return;

//    qsort(array, low, j+1);
//    qsort(array, j, high);

//    for(int i=0;i<n;i++)
//    {
//        if(i == mid)
//            continue;
//        if (i<mid && array[i] <= array[mid])
//            continue;
//        if (i>mid && array[i] >= array[mid])
//            continue;


//        int abc=array[i];
//        array[i]=array[mid];
//        array[mid]=abc;


//    }

//    cout<<endl;
//    for(int i=0;i<n;i++)
//    {
//        cout<<array[i]<<" ";
//    }
//    cout<<endl;

//    if(n <=2)
//        return;

//    qsort (array,mid+1);
//    qsort(&(array[mid]),n-mid);

//}

//int main()
//{
//    const int n=6;
//    int student[n]={5,8,2,3,0,1};
//    cout<<"element now: ";
//    for(int i=0;i<n;i++)
//    {
//        cout<<student[i]<<" ";
//    }
//    cout<<endl;

//    qsort(student,0, n);

//    cout<<"element: ";
//    for(int i=0;i<n;i++)
//    {
//        cout<<student[i]<<" ";
//    }
//    cout<<endl;

//    return 0;
//}


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
    cout<<"ARRAY 1: ";
    d_array = (Name*)malloc(k*sizeof(Name));
        for(int i=0;i<k;++i)
        {
            file>>d_array[i];
            file.get();
            cout<<d_array[i]<<" ";
        }
    cout<<"\nARRAY 2: ";
        for(int i=0;i<k;i++)
        {
            cout<<d_array[i]<< " ";
        }
        cout<<endl;
    free(d_array);
    file.close();
return 0;
}


















































//void sortirovka(int n,int *student)  //значение в функции void f.c(.....) <--
//{
//    // Сортировка по убыванию
//    for(int i=1;i<n;i++)
//    {
//        for(int m=0;m<n;m++)
//        {
//            if((student)[m] < (student)[m+1]) // int *ss = *student;
//            {
//                int abs=student[m];
//                student[m]=student[m+1];
//                student[m+1]=abs;
//            }
//        }
//    }
//}

////void fn(float *f)
////{

////    *f=*f * 2;
////    return ;
////}




//int main()
//{

////    float a=4;
////    fn(&a);

////    cout<<a;
////    cout.flush();



//    const int n=3;

//    int student[n];
//    for(int i=0;i<n;i++)
//    {
//        cout<<i+1<<" element: ";
//        cin>>student[i];
//    }



//    cout<<"element now: ";
//    for(int i=0;i<n;i++)
//    {
//        cout<<student[i]<<" ";
//    }
//    cout<<endl;

//    sortirovka(n,student);

//    cout<<"element: ";
//    for(int i=0;i<n;i++)
//    {
//        cout<<student[i]<<" ";
//    }
//    cout<<endl

//    return 0;
//}
