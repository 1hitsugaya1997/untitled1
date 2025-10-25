#include "lab1.h"
#include <iostream>
#include <limits>
#include <stdexcept>

class LinkedList {
private:
    struct Node {
        int data;
        Node* next;
        explicit Node(int v) : data(v), next(nullptr) {}
    };

    Node* head = nullptr;

public:
    LinkedList() = default;
    LinkedList(const LinkedList&) = delete;
    LinkedList& operator=(const LinkedList&) = delete;

    ~LinkedList() { clear(); }

    void push_back(int value) {
        Node* n = new Node(value);
        if (!head) {
            head = n;
            return;
        }
        Node* cur = head;
        while (cur->next)
            cur = cur->next;
        cur->next = n;
    }

    double average() const {
        if (!head)
            throw std::runtime_error("Список пуст (по условию должен быть непустым)");
        long long sum = 0;
        int count = 0;
        for (Node* cur = head; cur; cur = cur->next) {
            sum += cur->data;
            count++;
        }
        return static_cast<double>(sum) / count;
    }

    void clear() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

void lab1_run() {
    std::cout << "\n=== Лабораторная работа №1 (C++) ===\n";
    std::cout << "Тема: Динамические структуры данных (односвязный список)\n\n";

    int n;
    std::cout << "Введите количество элементов (n > 0): ";
    while (!(std::cin >> n) || n <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Ошибка! Введите положительное целое n: ";
    }

    LinkedList list;

    std::cout << "\nВведите " << n << " элементов списка:\n";
    for (int i = 0; i < n; ++i) {
        int value;
        std::cout << "Введите элемент " << (i + 1) << ": ";
        while (!(std::cin >> value)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка! Повторите ввод элемента " << (i + 1) << ": ";
        }
        list.push_back(value);
    }

    try {
        double avg = list.average();
        std::cout.setf(std::ios::fixed);
        std::cout.precision(2);
        std::cout << "\nСреднее арифметическое элементов списка: "
                  << avg << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
}
