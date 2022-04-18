#include <iostream>
 
int main()
{
	setlocale(LC_ALL, "Russian");
    int a = 0;
    int b = 0;
    char operation;
    std::cout << "Enter first number: ";
    std::cin >> a;
    std::cout << "Enter operation: ";
    std::cin >> operation;
    std::cout << "Enter second number: ";
    std::cin >> b;
 
    switch (operation)
	{
    case '-':
        std::cout << "a - b = " << a - b << '\n';
        break;
    case '+':
        std::cout << "a + b = " << a + b << '\n';
        break;
    case '*':
        std::cout << "a * b = " << a * b << '\n';
        break;
    case '/':
        std::cout << "a / b = " << a / b << '\n';
        break;
    case '%':
        std::cout << "a % b = " << a % b << '\n';
        break;
    default:
        std::cout << "Error\n";
    }
    return 0;
}
