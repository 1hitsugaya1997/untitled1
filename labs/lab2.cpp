#include "lab2.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <string>
#include <cstring>
#include <filesystem>

namespace fs = std::filesystem;

// Фиксированная запись для бинарного файла
struct PatientBin {
    char surname[64];   // Фамилия (UTF-8), до 63 байт + '\0'
    char sex;           // 'M' или 'F'
    int  age;           // возраст
    char city[64];      // Город (UTF-8)
    char diagnosis[16]; // Диагноз (например, "J")
};

// -------- утилиты --------
static void trim(std::string& s) {
    auto isspace_ = [](unsigned char ch){ return std::isspace(ch); };
    while (!s.empty() && isspace_(s.back())) s.pop_back();
    size_t i = 0;
    while (i < s.size() && isspace_(s[i])) ++i;
    if (i) s.erase(0, i);
}

static void clear_line() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static std::string prompt_line(const std::string& prompt) {
    std::string s;
    for (;;) {
        std::cout << prompt;
        std::getline(std::cin, s);
        trim(s);
        if (!s.empty()) return s;
        std::cout << "Строка не должна быть пустой. Повторите.\n";
    }
}

static int prompt_int_in_range(const std::string& prompt, int lo, int hi) {
    int x;
    for (;;) {
        std::cout << prompt;
        if (std::cin >> x && x >= lo && x <= hi) { clear_line(); return x; }
        std::cout << "Ожидалось целое число в диапазоне [" << lo << ";" << hi << "].\n";
        std::cin.clear(); clear_line();
    }
}

static char prompt_sex(const std::string& prompt) {
    for (;;) {
        std::cout << prompt;
        char ch;
        if (!(std::cin >> ch)) { std::cin.clear(); clear_line(); continue; }
        clear_line();
        ch = (char)std::toupper((unsigned char)ch);
        if (ch == 'M' || ch == 'F') return ch;
        std::cout << "Допустимые значения: M или F.\n";
    }
}

static void copy_cstr(char* dst, size_t cap, const std::string& src) {
    std::memset(dst, 0, cap);
    if (cap) std::strncpy(dst, src.c_str(), cap - 1); // безопасная обрезка
}

static PatientBin input_patient(int idx) {
    std::cout << "\n— Пациент " << idx << " —\n";
    PatientBin p{}; // zero-init
    std::string surname = prompt_line("Фамилия: ");
    char sex          = prompt_sex("Пол (M/F): ");
    int  age          = prompt_int_in_range("Возраст (0..150): ", 0, 150);
    std::string city  = prompt_line("Город проживания: ");
    std::string diag  = prompt_line("Диагноз (например, J): ");

    copy_cstr(p.surname,   sizeof(p.surname),   surname);
    p.sex = sex;
    p.age = age;
    copy_cstr(p.city,      sizeof(p.city),      city);
    copy_cstr(p.diagnosis, sizeof(p.diagnosis), diag);
    return p;
}

// текстовый файл (для отчётности)
static bool write_text(const fs::path& path, const std::vector<PatientBin>& v) {
    std::ofstream out(path);
    if (!out) { std::cerr << "Не удалось создать текстовый файл: " << path << "\n"; return false; }
    // Формат: Фамилия;Пол;Возраст;Город;Диагноз
    for (const auto& p : v) {
        out << p.surname << ';' << p.sex << ';' << p.age << ';'
            << p.city << ';' << p.diagnosis << "\n";
    }
    return (bool)out;
}

// бинарный файл
static bool write_bin(const fs::path& path, const std::vector<PatientBin>& v) {
    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    if (!out) { std::cerr << "Не удалось создать бинарный файл: " << path << "\n"; return false; }
    for (const auto& p : v) {
        out.write(reinterpret_cast<const char*>(&p), sizeof(PatientBin));
        if (!out) { std::cerr << "Ошибка записи бинарника.\n"; return false; }
    }
    out.flush();
    return (bool)out;
}

static bool read_bin(const fs::path& path, std::vector<PatientBin>& v) {
    std::ifstream in(path, std::ios::binary);
    if (!in) { std::cerr << "Не удалось открыть бинарный файл: " << path << "\n"; return false; }
    v.clear();
    PatientBin p{};
    while (in.read(reinterpret_cast<char*>(&p), sizeof(PatientBin))) v.push_back(p);
    if (!in.eof()) { std::cerr << "Ошибка чтения бинарного файла (повреждён?).\n"; return false; }
    return true;
}

// сортировка по фамилии
static std::string lower_ascii(std::string s) {
    for (char& ch : s) ch = (char)std::tolower((unsigned char)ch);
    return s;
}
static void sort_by_surname(std::vector<PatientBin>& v) {
    std::sort(v.begin(), v.end(), [](const PatientBin& a, const PatientBin& b){
        return lower_ascii(a.surname) < lower_ascii(b.surname);
    });
}

// фильтр: иногородние + диагноз == "J"
static std::vector<PatientBin> filter_nonresident_with_J(const std::vector<PatientBin>& v,
                                                         const std::string& localCity) {
    std::string lcLocal = lower_ascii(localCity);
    std::vector<PatientBin> out;
    for (const auto& p : v) {
        if (lower_ascii(p.city) != lcLocal && lower_ascii(p.diagnosis) == "j") out.push_back(p);
    }
    return out;
}

void lab2_run() {
    std::cout << "\n=== Лабораторная работа №2 (C++) ===\n";
    std::cout << "Тема: Файловые потоки. Текст -> бинарный, сортировка по фамилии, выборка (иногородние + J).\n\n";

    // 1) ввод количества
    int n;
    std::cout << "Введите количество записей (n > 0): ";
    while (!(std::cin >> n) || n <= 0) {
        std::cin.clear(); clear_line();
        std::cout << "Ошибка! Введите целое n > 0: ";
    }
    clear_line();

    // 2) ввод записей
    std::vector<PatientBin> patients;
    patients.reserve(n);
    for (int i = 1; i <= n; ++i) patients.push_back(input_patient(i));

    // пути файлов
    fs::path txtPath = "patients.txt";
    fs::path binPath = "patients.bin";

    // 3) текстовый файл
    if (!write_text(txtPath, patients)) {
        std::cerr << "Прерывание: не удалось записать текстовый файл.\n";
        return;
    }
    std::cout << "Создан текстовый файл: " << fs::absolute(txtPath).string() << "\n";

    // 4) бинарный файл
    if (!write_bin(binPath, patients)) {
        std::cerr << "Прерывание: не удалось записать бинарный файл.\n";
        return;
    }
    std::cout << "Создан бинарный файл: " << fs::absolute(binPath).string() << "\n";

    // 5) чтение бинарного
    std::vector<PatientBin> loaded;
    if (!read_bin(binPath, loaded) || loaded.empty()) {
        std::cerr << "Прерывание: не удалось прочитать бинарный файл или он пуст.\n";
        return;
    }

    // 6) сортировка по фамилии
    sort_by_surname(loaded);

    // 7) ввод «местного» города и фильтр
    std::string localCity = prompt_line("\nУкажите «местный» город (для фильтра иногородних): ");
    auto filtered = filter_nonresident_with_J(loaded, localCity);

    // 8) вывод результата
    std::cout << "\nИногородние пациенты с диагнозом J (город != " << localCity << "):\n";
    if (filtered.empty()) {
        std::cout << "— не найдено —\n";
    } else {
        std::cout << std::left << std::setw(20) << "Фамилия"
                  << std::setw(8)  << "Пол"
                  << std::setw(8)  << "Возраст"
                  << std::setw(18) << "Город"
                  << std::setw(10) << "Диагноз" << "\n";
        std::cout << std::string(64, '-') << "\n";
        for (const auto& p : filtered) {
            std::cout << std::left << std::setw(20) << p.surname
                      << std::setw(8)  << p.sex
                      << std::setw(8)  << p.age
                      << std::setw(18) << p.city
                      << std::setw(10) << p.diagnosis << "\n";
        }
    }
    std::cout << "\nГотово.\n";
}
