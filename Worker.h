#pragma once
#include <string>
#include <iostream>

class WORKER {
private:
    std::string fio_;       // ФИО работника
    std::string position_;  // должность
    int hireYear_;          // год поступления

public:
    // Конструкторы
    WORKER();
    WORKER(const std::string& fio, const std::string& position, int hireYear);
    WORKER(const WORKER& other);

    // Доступ (get/set)
    const std::string& getFIO() const;
    const std::string& getPosition() const;
    int getHireYear() const;

    void setFIO(const std::string& fio);
    void setPosition(const std::string& pos);
    void setHireYear(int year);

    // Вспомогательное: получить фамилию (первое слово из ФИО)
    std::string getSurname() const;

    // Перегруженные потоки
    friend std::ostream& operator<<(std::ostream& os, const WORKER& w);
    friend std::istream& operator>>(std::istream& is, WORKER& w);
};
