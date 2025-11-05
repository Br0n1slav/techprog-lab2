#include "Worker.h"
#include <sstream>

WORKER::WORKER() : fio_(), position_(), hireYear_(0) {}
WORKER::WORKER(const std::string& fio, const std::string& position, int hireYear)
    : fio_(fio), position_(position), hireYear_(hireYear) {}
WORKER::WORKER(const WORKER& other)
    : fio_(other.fio_), position_(other.position_), hireYear_(other.hireYear_) {}

const std::string& WORKER::getFIO() const { return fio_; }
const std::string& WORKER::getPosition() const { return position_; }
int WORKER::getHireYear() const { return hireYear_; }

void WORKER::setFIO(const std::string& fio) { fio_ = fio; }
void WORKER::setPosition(const std::string& pos) { position_ = pos; }
void WORKER::setHireYear(int year) { hireYear_ = year; }

std::string WORKER::getSurname() const {
    std::istringstream iss(fio_);
    std::string surname;
    iss >> surname;
    return surname;
}

// Формат вывода: "ФИО; должность; год"
std::ostream& operator<<(std::ostream& os, const WORKER& w) {
    os << w.fio_ << "; " << w.position_ << "; " << w.hireYear_;
    return os;
}

// Аккуратный ввод без подсказок: читаем три строки (ФИО, должность, год)
std::istream& operator>>(std::istream& is, WORKER& w) {
    std::string fio, pos, yearLine;

    // игнорируем ведущие переводы строк
    if (is.peek() == '\n') is.get();

    std::getline(is, fio);
    std::getline(is, pos);
    std::getline(is, yearLine);

    if (!fio.empty() && !pos.empty() && !yearLine.empty()) {
        w.fio_ = fio;
        w.position_ = pos;
        std::istringstream(yearLine) >> w.hireYear_;
    } else {
        is.setstate(std::ios::failbit);
    }
    return is;
}
