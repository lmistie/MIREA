#include <iostream>
#include <cstdlib>
 
int vector[] = { 14, 10, 11, 19, 2, 25 };

int compare(const void * x1, const void * x2)   // ������� ��������� ��������� �������
{
  return ( *(int*)x1 - *(int*)x2 );              // ���� ��������� ��������� ����� 0, �� ����� �����, < 0: x1 < x2; > 0: x1 > x2
}
 
int main ()
{
  qsort(vector, 6, sizeof(int), compare);      // ��������� ������ �����
  for ( int ix = 0; ix < 6; ix++)
      std::cout << vector[ix] << " ";
  return 0;
}
