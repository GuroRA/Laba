#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <cstring>

struct Date {
    int day;
    int month;
    int year;

    Date() : day(0), month(0), year(0) {}

    Date(std::string str) {
        if (str.length() == 8) {
            day = std::stoi(str.substr(0, 2));
            month = std::stoi(str.substr(2, 2));
            year = std::stoi(str.substr(4, 4));
        }
    }

    int getAge() {
        int age = 2026 - year;
        if (5 < month || (5 == month && 21 < day)) {
            age--;
        }
        return age;
    }
};

struct Address {
    char city[50];
    char street[50];
    int house;
    int apartment;

    Address() {
        memset(city, 0, sizeof(city));
        memset(street, 0, sizeof(street));
        house = 0;
        apartment = 0;
    }
};

struct Person {
    char lastName[50];
    char firstName[50];
    char secondName[50];
    Date birthDate;
    char education;
    char maritalStatus;
    int childrenCount;
    char profession;
    Address address;
    char phone[20];

    Person() {
        memset(lastName, 0, sizeof(lastName));
        memset(firstName, 0, sizeof(firstName));
        memset(secondName, 0, sizeof(secondName));
        education = ' ';
        maritalStatus = ' ';
        childrenCount = 0;
        profession = ' ';
        memset(phone, 0, sizeof(phone));
    }
};

void copyString(char* dest, std::string src, int maxLen) {
    strncpy_s(dest, maxLen, src.c_str(), _TRUNCATE);
}

std::string getString(char* src) {
    return std::string(src);
}

std::string getInitials(Person p) {
    return getString(p.lastName) + " " + std::string(1, p.firstName[0]) + "." + std::string(1, p.secondName[0]) + ".";
}

std::string getEduName(char edu) {
    switch (edu) {
    case 'В': return "Высшее";
    case 'С': return "Среднее";
    case 'Н': return "Начальное";
    default: return "Неизвестно";
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::ifstream txtFile("base_1.txt");
    std::ofstream binFile("base.dat", std::ios::binary);

    if (!txtFile.is_open()) {
        std::cerr << "Ошибка: не удалось открыть base_1.txt" << std::endl;
        return 1;
    }

    Person p;
    std::string temp;

    while (txtFile >> temp >> p.firstName >> p.secondName) {
        copyString(p.lastName, temp, 50);

        std::string dateStr;
        txtFile >> dateStr;
        p.birthDate = Date(dateStr);

        txtFile >> p.education >> p.maritalStatus >> p.childrenCount >> p.profession;

        std::string city, street, phone;
        txtFile >> city >> street >> p.address.house >> p.address.apartment >> phone;

        copyString(p.address.city, city, 50);
        copyString(p.address.street, street, 50);
        copyString(p.phone, phone, 20);

        binFile.write(reinterpret_cast<char*>(&p), sizeof(Person));
    }

    txtFile.close();
    binFile.close();

    std::ifstream inFile("base.dat", std::ios::binary);
    if (!inFile.is_open()) {
        std::cerr << "Ошибка: не удалось открыть base.dat" << std::endl;
        return 1;
    }

    std::vector<Person> persons;
    Person person;

    while (inFile.read(reinterpret_cast<char*>(&person), sizeof(Person))) {
        persons.push_back(person);
    }
    inFile.close();

    int countV = 0, countS = 0, countN = 0;
    for (int i = 0; i < persons.size(); i++) {
        if (persons[i].education == 'В') countV++;
        else if (persons[i].education == 'С') countS++;
        else countN++;
    }

    char maxEdu = 'В';
    int maxCount = countV;
    if (countS > maxCount) { maxCount = countS; maxEdu = 'С'; }
    if (countN > maxCount) { maxCount = countN; maxEdu = 'Н'; }

    char minEdu = 'В';
    int minCount = countV;
    if (countS < minCount) { minCount = countS; minEdu = 'С'; }
    if (countN < minCount) { minCount = countN; minEdu = 'Н'; }

    std::vector<Person> groupMax, groupMin;
    for (int i = 0; i < persons.size(); i++) {
        if (persons[i].education == maxEdu) {
            groupMax.push_back(persons[i]);
        }
        if (persons[i].education == minEdu) {
            groupMin.push_back(persons[i]);
        }
    }

    std::ofstream fileMax("max_group.dat", std::ios::binary);
    std::ofstream fileMin("min_group.dat", std::ios::binary);

    for (int i = 0; i < groupMax.size(); i++) {
        fileMax.write(reinterpret_cast<char*>(&groupMax[i]), sizeof(Person));
    }

    for (int i = 0; i < groupMin.size(); i++) {
        fileMin.write(reinterpret_cast<char*>(&groupMin[i]), sizeof(Person));
    }

    fileMax.close();
    fileMin.close();

    std::string titleMin = "Малочисленная: " + getEduName(minEdu);
    std::string titleMax = "Многочисленная: " + getEduName(maxEdu);

    std::cout << std::left;
    std::cout << std::setw(50) << titleMin << std::setw(50) << titleMax << std::endl;
    std::cout << std::string(100, '-') << std::endl;

    std::cout << std::setw(20) << "ФИО"
        << std::setw(8) << "Возр."
        << std::setw(6) << "Сем."
        << std::setw(6) << "Дет."
        << std::setw(6) << "Проф."
        << "  "
        << std::setw(20) << "ФИО"
        << std::setw(8) << "Возр."
        << std::setw(6) << "Сем."
        << std::setw(6) << "Дет."
        << std::setw(6) << "Проф."
        << std::endl;
    std::cout << std::string(100, '-') << std::endl;

    int maxSize = groupMax.size() > groupMin.size() ? groupMax.size() : groupMin.size();

    for (int i = 0; i < maxSize; i++) {
        if (i < groupMin.size()) {
            Person p = groupMin[i];
            std::cout << std::setw(20) << getInitials(p)
                << std::setw(8) << p.birthDate.getAge()
                << std::setw(6) << p.maritalStatus
                << std::setw(6) << p.childrenCount
                << std::setw(6) << p.profession;
        }
        else {
            std::cout << std::string(46, ' ');
        }

        std::cout << "  ";

        if (i < groupMax.size()) {
            Person p = groupMax[i];
            std::cout << std::setw(20) << getInitials(p)
                << std::setw(8) << p.birthDate.getAge()
                << std::setw(6) << p.maritalStatus
                << std::setw(6) << p.childrenCount
                << std::setw(6) << p.profession;
        }
        else {
            std::cout << std::string(46, ' ');
        }
        std::cout << std::endl;
    }

    return 0;
}