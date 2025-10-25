#include "lab3.h"
#include <iostream>
#include <string>
#include <sstream>
#include <limits>
#include <cctype>

class MyString {
private:
    std::string text; // строка символов

public:
    // ✅ Конструктор по строке
    explicit MyString(const std::string& str) : text(str) {}

    // ✅ Конструктор копии
    MyString(const MyString& other) : text(other.text) {}

    // ✅ Функция-член: вывод на экран строки
    void print() const {
        std::cout << "Строка: \"" << text << "\"" << std::endl;
    }

    // ✅ Функция-член: нахождение самого короткого слова (финальная версия)
    std::string findShortestWord() const {
        std::istringstream iss(text);
        std::string word;
        std::string shortest;
        bool first = true;

        auto isWord = [](const std::string& s) {
            for (unsigned char ch : s) {
                // Проверяем: латиница или кириллица
                if ((ch >= 'A' && ch <= 'Z') ||
                    (ch >= 'a' && ch <= 'z') ||
                    (ch >= 0xC0 && ch <= 0xFF) || // русские буквы в CP1251
                    ch == 0xA8 || ch == 0xB8)     // Ё и ё в CP1251
                        return true;
            }
            return false;
        };

        while (iss >> word) {
            if (!isWord(word)) continue; // пропустить "—", "!!!", "123" и т.п.

            if (first || word.length() < shortest.length()) {
                shortest = word;
                first = false;
            }
        }

        return shortest.empty() ? "(нет слов)" : shortest;
    }

};

// ================== Основная логика ЛР3 ==================
void lab3_run() {
    std::cout << "\n=== Лабораторная работа №3 (C++) ===\n";
    std::cout << "Тема: Класс «строка символов»\n";
    std::cout << "Функции: вывод строки и нахождение самого короткого слова\n\n";

    std::cout << "Введите строку символов:\n> ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // очистка предыдущего ввода
    std::string input;
    std::getline(std::cin, input);

    // ✅ Создание объекта и копии
    MyString original(input);
    MyString copy(original);

    // ✅ Вывод строк
    std::cout << "\nОригинал:\n";
    original.print();

    std::cout << "Копия:\n";
    copy.print();

    // ✅ Нахождение самого короткого слова
    std::string shortest = original.findShortestWord();
    std::cout << "\nСамое короткое слово в строке: \"" << shortest << "\"\n";
}
