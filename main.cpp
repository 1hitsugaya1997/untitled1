#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif
#include "labs/lab1.h"
#include "labs/lab2.h"
#include "labs/lab3.h"
#include "labs/lab4.h"

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    std::cout << "Выберите лабораторную работу (1-4): ";
    int choice;
    if (!(std::cin >> choice)) return 0;

    switch (choice) {
        case 1: lab1_run(); break;
        case 2: lab2_run(); break;
        case 3: lab3_run(); break;
        case 4: lab4_run(); break;
        default:
            std::cout << "Нет такой лабораторной!\n";
            break;
    }
    return 0;
}
