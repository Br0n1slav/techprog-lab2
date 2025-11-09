#include <algorithm>
#include <chrono>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Worker.h"
#include <windows.h>

// ===== Часть 2: предложения без запятых =====
static std::string readFileAll(const std::string& path) {
    std::ifstream in(path);
    std::ostringstream buf;
    buf << in.rdbuf();
    return buf.str();
}

static std::vector<std::string> splitSentences(const std::string& text) {
    std::vector<std::string> res;
    std::string cur;
    for (char ch : text) {
        cur.push_back(ch);
        if (ch == '.' || ch == '!' || ch == '?') {
            // обрезаем пробелы по краям
            size_t l = 0, r = cur.size();
            while (l < r && std::isspace((unsigned char)cur[l])) ++l;
            while (r > l && std::isspace((unsigned char)cur[r - 1])) --r;
            res.emplace_back(cur.substr(l, r - l));
            cur.clear();
        }
    }
    // если остался хвост без терминатора — тоже добавим
    if (!cur.empty()) {
        size_t l = 0, r = cur.size();
        while (l < r && std::isspace((unsigned char)cur[l])) ++l;
        while (r > l && std::isspace((unsigned char)cur[r - 1])) --r;
        if (r > l) res.emplace_back(cur.substr(l, r - l));
    }
    return res;
}

size_t my_find(const std::string& text, const std::string& pattern) {
    if (pattern.empty()) return 0;                     // пустой шаблон ? найден в начале
    if (pattern.size() > text.size()) return std::string::npos;

    for (size_t i = 0; i <= text.size() - pattern.size(); ++i) {
        size_t j = 0;
        while (j < pattern.size() && text[i + j] == pattern[j]) {
            ++j;
        }
        if (j == pattern.size()) return i;            // нашли совпадение
    }
    return std::string::npos;                         // не нашли
}

static void printSentencesWithoutCommas(const std::string& filename) {
    std::string text = readFileAll(filename);
    if (text.empty()) {
        std::cout << "Файл пустой или не найден.\n";
        return;
    }
    auto sentences = splitSentences(text);
    bool any = false;
    for (const auto& s : sentences) {
        if (my_find(s, std::string(",")) == std::string::npos) {
            std::cout << s << "\n";
            any = true;
        }
    }
    if (!any) {
        std::cout << "Нет предложений без запятых.\n";
    }
}

// ===== Часть 1: WORKER + сортировка + стаж =====
static int currentYear() {
    std::time_t t = std::time(nullptr);
    std::tm* tm_ptr = std::localtime(&t);
    return 1900 + tm_ptr->tm_year;
}

static void inputWorkersInteractive(std::vector<WORKER>& arr) {
    std::cout << "Ввод работников. (Ввод exit — завершение.)\n";
    while (true) {
        std::cin.clear();
        std::string fio;
        std::cout << "ФИО: ";
        std::getline(std::cin, fio);
        if (fio.empty()) break;

        std::string pos;
        std::cout << "Должность: ";
        std::getline(std::cin, pos);

        std::string yearLine;
        int year = 0;
        std::cout << "Год поступления: ";
        std::getline(std::cin, yearLine);
        std::istringstream(yearLine) >> year;

        arr.emplace_back(fio, pos, year);
    }
}

static void sortAlphabeticallyBySurname(std::vector<WORKER>& arr) {
    std::sort(arr.begin(), arr.end(), [](const WORKER& a, const WORKER& b) {
        std::string sa = a.getSurname();
        std::string sb = b.getSurname();
        // нечувствительно к регистру
        auto tolow = [](std::string s) {
            for (char& c : s) c = (char)std::tolower((unsigned char)c);
            return s;
        };
        sa = tolow(sa);
        sb = tolow(sb);
        if (sa != sb) return sa < sb;
        // если фамилии совпадают — сравним полное ФИО
        return tolow(a.getFIO()) < tolow(b.getFIO());
    });
}

static void printByExperience(const std::vector<WORKER>& arr, int minYears) {
    int cy = currentYear();
    bool any = false;
    for (const auto& w : arr) {
        int years = cy - w.getHireYear();
        if (years > minYears) {
            std::cout << w.getSurname() << "\n";
            any = true;
        }
    }
    if (!any) {
        std::cout << "Нет работников со стажем больше " << minYears << " лет.\n";
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::vector<WORKER> workers;

    while (true) {
        std::cin.clear();
        std::cout << "\n=== ЛР2 Меню ===\n"
                  << "1) Ввести работников (неизвестное число)\n"
                  << "2) Сортировать по алфавиту (фамилия)\n"
                  << "3) Показать всех (<<)\n"
                  << "4) Вывести фамилии со стажем > X\n"
                  << "5) Задание 2: предложения без запятых (читать из файла)\n"
                  << "0) Выход\n"
                  << "Выбор: ";

        int choice = -1;
        if (!(std::cin >> choice)) return 0;
        std::cin.clear();

        if (choice == 0) break;

        switch (choice) {
            case 1: {
                inputWorkersInteractive(workers);
                break;
            }
            case 2: {
                sortAlphabeticallyBySurname(workers);
                std::cout << "Отсортировано.\n";
                break;
            }
            case 3: {
                if (workers.empty()) {
                    std::cout << "Список пуст.\n";
                } else {
                    std::cout << "Список работников:\n";
                    for (const auto& w : workers) {
                        std::cout << w << "\n";
                    }
                }
                break;
            }
            case 4: {
                std::cout << "Введите X (минимальный стаж в годах): ";
                int x = 0;
                std::cin.clear();
                std::cin >> x;
                std::cin.clear();
                printByExperience(workers, x);
                break;
            }
            case 5: {
                std::cin.clear();
                std::cout << "Введите имя файла (например, text.txt): ";
                std::string fname;
                std::getline(std::cin, fname);
                printSentencesWithoutCommas(fname);
                break;
            }
            default:
                std::cout << "Неизвестный пункт.\n";
        }
    }
    return 0;
}
