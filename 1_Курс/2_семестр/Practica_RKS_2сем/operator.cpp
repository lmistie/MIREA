#include <iostream>

using namespace std;

class Date {
    int day;
    int month;
    int year;
public:
    Date(int d, int m, int y)
        : day {d}, month {m}, year {y} { }
    int getDay()  const { return day; }
    int getMonth()const { return month; }
    int getYear() const { return year; }        

};

ostream& operator<<(ostream &os, const Date& d) {
    return os  << '(' << d.getDay() << '.' << d.getMonth()
        << '.' << d.getYear() << ')' << endl;
}



int main() {
    Date dd {03, 03, 2020};
    cout << dd << endl;
    return 0;
}
