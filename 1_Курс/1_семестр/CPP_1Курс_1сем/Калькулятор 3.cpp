#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main()
{
    double a, b, c;
    char d;
    int p = 1;
    while (p != 2)
    {
        printf("1 - calculator\n2 - exit\n");
        scanf("%d", &p);
        if (p != 1) break;
        printf("Write expression (without spaces): ");
        scanf("%lf%c%lf", &a,&d,&b);
        while(getchar() != '\n');
        switch(d)
        {
        case '+': c = a + b; break;
        case '-': c = a - b; break;
        
		case '*': c = a * b; break;
        case '/': c = a / b; break;
        default:
            printf("Wrong action\n");
            continue;
        }
        printf("Result = %lf\n", c);
    }
}
