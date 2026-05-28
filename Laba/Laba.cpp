#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>

using namespace std;

struct Person {
    char lastName[50];
    char firstName[50];
    char secondName[50];
    int day, month, year;
    char education;      
    char marital;        
    int children;
    char profession;    
    char city[50];
    char street[50];
    int house;
    int apartment;
    char phone[20];
};


void parseDate(const char* dateStr, int& d, int& m, int& y) {
    d = (dateStr[0] - '0') * 10 + (dateStr[1] - '0');
    m = (dateStr[2] - '0') * 10 + (dateStr[3] - '0');
    y = (dateStr[4] - '0') * 1000 + (dateStr[5] - '0') * 100 +
        (dateStr[6] - '0') * 10 + (dateStr[7] - '0');
}


const char* educationToString(char edu) {
    switch (edu) {
    case 'В': return "Высшее";
    case 'С': return "Среднее";
    case 'Н': return "Начальное";
    default: return "Неизвестно";
    }
}

int calculateAge(int bDay, int bMonth, int bYear) {
    int curDay = 21, curMonth = 5, curYear = 2026;
    int age = curYear - bYear;
    if (curMonth < bMonth || (curMonth == bMonth && curDay < bDay))
        age--;
    return age;
}

void getInitials(const Person& p, char* result) {
    sprintf(result, "%s %c.%c.", p.lastName, p.firstName[0], p.secondName[0]);
}

int main() {
	setlocale(LC_ALL, "Russian");
    //1: Создание бинарного файла из текстового

    ifstream in("base_1.txt");
    if (!in) {
        cout << "Ошибка: не найден base_1.txt" << endl;
        cout << "Нажмите Enter для выхода...";
        cin.get();
        return 1;
    }

    ofstream out("base.bin", ios::binary);
    if (!out) {
        cout << "Ошибка: не могу создать base.bin" << endl;
        cin.get();
        return 1;
    }

    char buffer[500];
    Person person;
    int records = 0;

    while (in.getline(buffer, 500)) {
        if (strlen(buffer) < 10) continue;

        char lName[50], fName[50], sName[50];
        char dateStr[20];
        char edu, marital;
        int children;
        char prof;
        char city[50], street[50];
        int house, apt;
        char phone[20];

        int res = sscanf(buffer, "%s %s %s %s %c %c %d %c %s %s %d %d %s",
            lName, fName, sName, dateStr,
            &edu, &marital, &children, &prof,
            city, street, &house, &apt, phone);

        if (res != 13) continue;

        strncpy(person.lastName, lName, 50);
        strncpy(person.firstName, fName, 50);
        strncpy(person.secondName, sName, 50);
        parseDate(dateStr, person.day, person.month, person.year);
        person.education = edu;
        person.marital = marital;
        person.children = children;
        person.profession = prof;
        strncpy(person.city, city, 50);
        strncpy(person.street, street, 50);
        person.house = house;
        person.apartment = apt;
        strncpy(person.phone, phone, 20);

        out.write((char*)&person, sizeof(Person));
        records++;
    }

    in.close();
    out.close();

    //2: Обработка бинарного файла

    ifstream binIn("base.bin", ios::binary);
    if (!binIn) {
        cout << "Ошибка: не найден base.bin" << endl;
        return 1;
    }

    Person* persons = nullptr;
    int total = 0;
    Person temp;
    while (binIn.read((char*)&temp, sizeof(Person))) {
        total++;
    }
    if (total == 0) {
        cout << "Файл пуст." << endl;
        cin.get();
        return 1;
    }

    persons = new Person[total];
    binIn.clear();
    binIn.seekg(0, ios::beg);
    binIn.read((char*)persons, total * sizeof(Person));
    binIn.close();

    int countV = 0, countS = 0, countN = 0;
    for (int i = 0; i < total; i++) {
        if (persons[i].education == 'В') countV++;
        else if (persons[i].education == 'С') countS++;
        else if (persons[i].education == 'Н') countN++;
    }

    char maxEdu = 'В';
    int maxCount = countV;
    if (countS > maxCount) { maxCount = countS; maxEdu = 'С'; }
    if (countN > maxCount) { maxCount = countN; maxEdu = 'Н'; }

    char minEdu = 'В';
    int minCount = countV;
    if (countS < minCount) { minCount = countS; minEdu = 'С'; }
    if (countN < minCount) { minCount = countN; minEdu = 'Н'; }

    Person* groupMax = new Person[maxCount];
    Person* groupMin = new Person[minCount];
    int idxMax = 0, idxMin = 0;

    for (int i = 0; i < total; i++) {
        if (persons[i].education == maxEdu) {
            groupMax[idxMax++] = persons[i];
        }
        if (persons[i].education == minEdu) {
            groupMin[idxMin++] = persons[i];
        }
    }

    ofstream fMax("max_group.bin", ios::binary);
    ofstream fMin("min_group.bin", ios::binary);

    for (int i = 0; i < maxCount; i++) {
        fMax.write((char*)&groupMax[i], sizeof(Person));
    }
    for (int i = 0; i < minCount; i++) {
        fMin.write((char*)&groupMin[i], sizeof(Person));
    }

    fMax.close();
    fMin.close();


    // Вывод на экран в 2 колонки
    cout << left;
    cout << setw(45) << ("Малочисленная: " + string(educationToString(minEdu)))
        << setw(45) << ("Многочисленная: " + string(educationToString(maxEdu))) << endl;
    cout << string(90, '-') << endl;

    cout << setw(22) << "ФИО"
        << setw(8) << "Возр."
        << setw(7) << "Сем."
        << setw(6) << "Дет."
        << setw(6) << "Проф."
        << "  "
        << setw(22) << "ФИО"
        << setw(8) << "Возр."
        << setw(7) << "Сем."
        << setw(6) << "Дет."
        << setw(6) << "Проф." << endl;
    cout << string(90, '-') << endl;

    int maxSize = (maxCount > minCount) ? maxCount : minCount;

    for (int i = 0; i < maxSize; i++) {
        if (i < minCount) {
            char initials[100];
            getInitials(groupMin[i], initials);
            cout << setw(22) << initials
                << setw(8) << calculateAge(groupMin[i].day, groupMin[i].month, groupMin[i].year)
                << setw(7) << groupMin[i].marital
                << setw(6) << groupMin[i].children
                << setw(6) << groupMin[i].profession;
        }
        else {
            cout << string(49, ' ');
        }

        cout << "  ";

        if (i < maxCount) {
            char initials[100];
            getInitials(groupMax[i], initials);
            cout << setw(22) << initials
                << setw(8) << calculateAge(groupMax[i].day, groupMax[i].month, groupMax[i].year)
                << setw(7) << groupMax[i].marital
                << setw(6) << groupMax[i].children
                << setw(6) << groupMax[i].profession;
        }
        else {
            cout << string(49, ' ');
        }
        cout << endl;
    }
    cout << string(90, '-') << endl;

    delete[] persons;
    delete[] groupMax;
    delete[] groupMin;


    return 0;
}