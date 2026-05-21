#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

// Короче сдесь мы задаём структуры так как это сказанно в задании,
// тоесть для даты, адреса и человека.
struct Date {
    int day;
    int month;
    int year;

	// Вот тут идут конструкторы для даты
    // один по умолчанию, а другой принимает строку и парсит её на день, месяц и год.
    Date() : day(0), month(0), year(0) {}

    Date(std::string str) {
		// stoi преобразует строку в число, а substr выделяет подстроку из строки.
        day = std::stoi(str.substr(0, 2));
        month = std::stoi(str.substr(2, 2));
        year = std::stoi(str.substr(4, 4));
    }

	//Этот метод возвращает возраст человека на 2026 год, учитывая его день и месяц рождения.
    int getAge() {
        int age = 2026 - year;
        if (5 < month || (5 == month && 21 < day)) {
            age--;
        }
        return age;
    }

	// Этот метод преобразует данные обратно в строку.
    std::string toString() {
        std::string d = std::to_string(day);
        std::string m = std::to_string(month);
        std::string y = std::to_string(year);

        if (d.length() == 1) d = "0" + d;
        if (m.length() == 1) m = "0" + m;

        return d + m + y;
    }
};
struct Address {
    std::string city;
    std::string street;
    int house;
    int apartment;

    std::string toString() {
        return city + " " + street + " " + std::to_string(house) + " " + std::to_string(apartment);
    }
};
struct Person {
    std::string lastName;
    std::string firstName;
    std::string secondName;
    Date birthDate;
    char education;
    char maritalStatus;
    int childrenCount;
    char profession;
    Address address;
    std::string phone;
};



std::string getEduName(char edu) {
    switch (edu) {
    case 'В': return "Высшее";
    case 'С': return "Среднее";
    case 'Н': return "Начальное";
    }
}

// Этот метод возвращает строку с фамилией и инициалами человека.
std::string getInitials(Person p) {
    return p.lastName + " " + p.firstName.substr(0, 1) + "." + p.secondName.substr(0, 1) + ".";
}


int main() {
    // Эта строчка нужна чтобы у тебя работала кирилица
	setlocale(LC_ALL, "Russian");

    //Эта строка проверяет наличие файла.
    std::ifstream inFile("base_1.txt");
    if (!inFile.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл base_1.txt" << std::endl;
        return 1;
    }

    std::vector<Person> persons;
    Person p;

	// Вот тут мы читаем данные из файла и заполняем структуру Person, а потом добавляем её в вектор persons
    //Конкретно икл идёт до тех пор пока ему в каждой следующей строчке будут встречатся ФИО
    // каждый символ >> переводит чтение на следующее слово в строке
    while (inFile >> p.lastName >> p.firstName >> p.secondName) {
        std::string dateStr; 
        inFile >> dateStr; // т.к в самом условии цикла мы уже использовали >> три раза, то при четвёртом ты попадаешь в дату
        p.birthDate = Date(dateStr);

        inFile >> p.education >> p.maritalStatus >> p.childrenCount >> p.profession;
        inFile >> p.address.city >> p.address.street >> p.address.house >> p.address.apartment >> p.phone;

		persons.push_back(p); //Тут после чтения всех слов разделённых пробелом мы добавляем заполненную структуру Person в вектор persons
    }
    inFile.close();

	// Вот тут мы считаем количество людей с каждым уровнем образования
    int countV = 0;
    int countS = 0;
    int countN = 0;

    for (int i = 0; i < persons.size(); i++) {
        if (persons[i].education == 'В') {
            countV++;
        }
        else if (persons[i].education == 'С') {
            countS++;
        }
        else if (persons[i].education == 'Н') {
            countN++;
        }
    }

	// Вот тут мы определяем какой уровень образования самый многочисленный, а какой самый малочисленный
    char maxEdu = 'В';
    int maxCount = countV;

    if (countS > maxCount) {
        maxCount = countS;
        maxEdu = 'С';
    }
    if (countN > maxCount) {
        maxCount = countN;
        maxEdu = 'Н';
    }
    char minEdu = 'В';
    int minCount = countV;
    if (countS < minCount) {
        minCount = countS;
        minEdu = 'С';
    }
    if (countN < minCount) {
        minCount = countN;
        minEdu = 'Н';
    }

	// Вот тут мы формируем две группы людей: одну с самым многочисленным уровнем образования, а другую с самым малочисленным
    std::vector<Person> groupMax;
    std::vector<Person> groupMin;

    for (int i = 0; i < persons.size(); i++) {
        if (persons[i].education == maxEdu) {
            groupMax.push_back(persons[i]);
        }
        if (persons[i].education == minEdu) {
            groupMin.push_back(persons[i]);
        }
    }

	// Вот тут мы записываем эти две группы в два разных файла
    std::ofstream fileMax("max_group.txt");
    std::ofstream fileMin("min_group.txt");

    for (int i = 0; i < groupMax.size(); i++) {
        Person p = groupMax[i];
        fileMax << p.lastName << " " << p.firstName << " " << p.secondName << " " << p.birthDate.toString() << " " << p.education << " " << p.maritalStatus << " " << p.childrenCount << " " << p.profession << " " << p.address.toString() << " " << p.phone << std::endl;
    }

    for (int i = 0; i < groupMin.size(); i++) {
        Person p = groupMin[i];
        fileMin << p.lastName << " " << p.firstName << " " << p.secondName << " " << p.birthDate.toString() << " " << p.education << " " << p.maritalStatus << " " << p.childrenCount << " " << p.profession << " " << p.address.toString() << " " << p.phone << std::endl;
    }
    fileMax.close();
    fileMin.close();

	// Вот тут мы выводим эти две группы на экран в виде таблицы
    std::string titleMin = "Малочисленная: " + getEduName(minEdu);
    std::string titleMax = "Многочисленная: " + getEduName(maxEdu);

    std::cout << std::left;
    std::cout << std::setw(50) << titleMin << std::setw(50) << titleMax << std::endl;

    std::cout << std::setw(20) << "ФИО" << std::setw(8) << "Возр." << std::setw(6) << "Сем." << std::setw(6) << "Дет." << std::setw(6) << "Проф." << "  " << std::setw(20) << "ФИО" << std::setw(8) << "Возр." << std::setw(6) << "Сем." << std::setw(6) << "Дет." << std::setw(6) << "Проф." << std::endl;

    int maxSize = groupMax.size() > groupMin.size() ? groupMax.size() : groupMin.size();


    for (int i = 0; i < maxSize; i++) {
        if (i < groupMin.size()) {
            Person p = groupMin[i];
            std::cout << std::setw(20) << getInitials(p) << std::setw(8) << p.birthDate.getAge() << std::setw(6) << p.maritalStatus << std::setw(6) << p.childrenCount << std::setw(6) << p.profession;
        }
        else {
			// Если в группе малочисленного образования меньше людей, чем в группе многочисленного, то мы просто выводим пустые строки для выравнивания таблицы.
            std::cout << std::string(46, ' ');
        }
        std::cout << "  ";

        if (i < groupMax.size()) {
            Person p = groupMax[i];
            std::cout << std::setw(20) << getInitials(p) << std::setw(8) << p.birthDate.getAge() << std::setw(6) << p.maritalStatus << std::setw(6) << p.childrenCount << std::setw(6) << p.profession;
        }
        else {
            std::cout << std::string(46, ' ');
        }
		// После каждой строки мы выводим endl для перехода на новую строку.
        std::cout << std::endl;
    }

    return 0;
}