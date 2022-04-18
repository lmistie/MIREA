#include <stdlib.h>
#include <stdio.h>

int num[10] = {
  1, 3, 6, 5, 8, 7, 9, 6, 2, 0
};

int comp(const void *, const void *);

int main(void)
{
  int i;

  printf("Array now: ");
  for(i=0; i<10; i++)
  printf("%d ", num[i]);

  qsort(num, 10, sizeof(int), comp);

  printf("Array sorting: ");
  for(i=0; i<10; i++)
  printf("%d ", num[i]);

  return 0;
}

/* сравнение целых */
int comp(const void *i, const void *j)
{
  return *(int *)i - *(int *)j;
}



/*int l_hold = left, r_hold = right;  // (int l_hold = left, int r_hold = right)
  int piv;
  piv = student[left];
  while (left < right)
  {
    while ((student[left] >= piv) && (left < right))
      right--;
    if (left != right)
      {
      	student[left] = student[right];
      	left++;
	  }
	  while ((student[left] <= piv) && (left < right))
	  	left++;
	  if (left != right)
	  {
	  	student[right] = student[left];
	  	right--;
	  }
	}
	student[left] = piv;
	piv=left;
	l_hold=left;
	r_hold=right;
	if (left < piv)
		qsort(student,left,piv -1);
	if (right > piv)
		qsort(student,piv + 1,right);
