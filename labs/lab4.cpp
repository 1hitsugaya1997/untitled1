#include "lab4.h"
#include <iostream>
#include <iomanip>
#include <limits>

// =============================
// Функции для работы с матрицами
// =============================

// 🔹 Ввод элементов 4x4 матрицы
void fillMatrix(double matrix[4][4], char name) {
    std::cout << "\nВведите элементы матрицы " << name << " (4x4):\n";
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << name << "[" << i + 1 << "][" << j + 1 << "]: ";
            while (!(std::cin >> matrix[i][j])) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Ошибка! Введите число: ";
            }
        }
    }
}

// 🔹 Вывод матрицы на экран
void printMatrix(double matrix[4][4], char name) {
    std::cout << "\nМатрица " << name << ":\n";
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << std::setw(8) << std::fixed << std::setprecision(2) << matrix[i][j];
        }
        std::cout << "\n";
    }
}

// 🔹 Формирование массива из главной диагонали
void extractDiagonal(double matrix[4][4], double diag[4]) {
    for (int i = 0; i < 4; ++i)
        diag[i] = matrix[i][i];
}

// 🔹 Вывод одномерного массива
void printArray(double arr[4], const std::string& name) {
    std::cout << "\nМассив " << name << " (главная диагональ): ";
    for (int i = 0; i < 4; ++i)
        std::cout << std::setw(6) << std::fixed << std::setprecision(2) << arr[i];
    std::cout << "\n";
}

// =============================
// Основная функция лабораторной
// =============================
void lab4_run() {
    std::cout << "\n=== Лабораторная работа №4 (C++) ===\n";
    std::cout << "Тема: Функции с параметрами. Матрицы и диагонали\n\n";

    double A[4][4];
    double B[4][4];
    double X[4];
    double Y[4];

    // Ввод и вывод
    fillMatrix(A, 'A');
    fillMatrix(B, 'B');

    printMatrix(A, 'A');
    printMatrix(B, 'B');

    // Формирование диагоналей
    extractDiagonal(A, X);
    extractDiagonal(B, Y);

    // Вывод результатов
    printArray(X, "X из A");
    printArray(Y, "Y из B");

    std::cout << "\nРабота программы завершена.\n";
}
