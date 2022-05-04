#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
#include <Windows.h>
using namespace std;


class Person {
	/*
	* Класс, в котором хранятся данные об одном постояльце и методы для работы с этими данными.
	*/
public:
	int apartment;
	int floor;
	string name;
	string surname;
	string patronymic;
	string phone;
	string date;

	// Техническая переменная. Если true, то постояльца нет.
	bool empty = true;

	/*
	* Метод, превращающий данные в строку.
	*/
	string ToString() {
		return to_string(apartment) + " "
			+ to_string(floor) + " "
			+ name + " "
			+ surname + " "
			+ patronymic + " "
			+ phone + " "
			+ date;
	}

	/*r
	* Метод, получающий данные из строки.
	* !нет проверки на правильность строки!.
	*/
	void FromString(string data) {
		data += " ";
		string s = "";
		int index = 0;

		for (int i = 0; i < data.size(); i++) {
			// Будем идти по строке, пока не найдем пробел.
			if (data[i] != ' ') {
				s += data[i];
			}
			else {
				switch (index) {
				case 0:
					apartment = stoi(s);
					break;
				case 1:
					floor = stoi(s);
					break;
				case 2:
					name = s;
					break;
				case 3:
					surname = s;
					break;
				case 4:
					patronymic = s;
					break;
				case 5:
					phone = s;
					break;
				case 6:
					date = s;
					break;
				}
				s = "";
				index++;
			}
		}
		empty = false;
	}

	/*
	* Метод, проверяющий, пустой ли это постоялец.
	*/
	bool isNull() {
		return empty;
	}
};


const int COUNT_FLOORS = 5;
const int APARTMENTS_ON_FLOOR = 10;

/*
* Класс, отвечающий за работу отеля.
*/
class Hotel {
public:
	// Постояльцы отеля.
	Person persons[COUNT_FLOORS][APARTMENTS_ON_FLOOR];
	// Имя файла с базой данных.
	string path;

	// Конструктор.
	Hotel(string file_name) {
		for (int i = 0; i < COUNT_FLOORS; i++) {
			for (int j = 0; j < APARTMENTS_ON_FLOOR; j++) {
				persons[i][j] = Person();
			}
		}

		path = file_name;
		OpenDataBase();
	}

	/*
	* Метод, который проверят, занят ли номер. True - занят, иначе не занят.
	*/
	bool Busy(int floor, int apartment) {
		return !persons[floor - 1][apartment - 1].isNull();
	}

	/*
	* Метод, добавляющий постояльца.
	*/
	void AddPerson(Person person) {
		persons[person.floor - 1][person.apartment - 1] = person;
	}

	/*
	* Метод, который считает, сколько свободных мест.
	*/
	int FreeApartmentsCount() {
		int result = 0;
		for (int i = 1; i <= COUNT_FLOORS; i++) {
			for (int j = 1; j <= APARTMENTS_ON_FLOOR; j++) {
				if (!Busy(i, j)) {
					result++;
				}
			}
		}
		return result;
	}

	/*
	* Метод, который записывает данные в базу данных.
	*/
	void WriteToDataBase() {
		ofstream fout;
		// Открывает файл и переписываем его.
		fout.open(path, fstream::out);

		if (!fout.is_open()) {
			cout << "Ошибка открытия файла!" << endl;
		}
		else {
			for (int i = 1; i <= COUNT_FLOORS; i++) {
				for (int j = 1; j <= APARTMENTS_ON_FLOOR; j++) {
					if (Busy(i, j)) {
						string s = persons[i - 1][j - 1].ToString();
						fout << s + "\n";
					}
				}
			}
			fout.close();
		}
	}


private:
	/*
	* Метод, который получает данные из базы данных.
	*/
	void OpenDataBase() {
		ifstream fin;
		fin.open(path, fstream::in);

		if (!fin.is_open()) {
			cout << "Ошибка открытия файла!" << endl;
		}
		else {
			string line;
			while (getline(fin, line)) {
				Person person = Person();
				person.FromString(line);
				persons[person.floor - 1][person.apartment - 1] = person;
			}
			fin.close();
		}
	}

};



int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	bool run = true;
	Hotel hotel = Hotel("data_base.txt");
	Person person;

	while (run) {
		std::cout << "1 - Забронировать номер\n2 - Количество свободных номеров\nЛюбой другой символ - выход\n> ";
		switch (cin.get()) {
		case '1':
			person = Person();
			// Получаем данные.
			cout << "Имя: ";
			cin >> person.name;
			cout << "Фамилия: ";
			cin >> person.surname;
			cout << "Отчество: ";
			cin >> person.patronymic;
			cout << "Телефон: ";
			cin >> person.phone;
			cout << "Дата заселения: ";
			cin >> person.date;
			cout << "Этаж: ";
			cin >> person.floor;
			cout << "Номер: ";
			cin >> person.apartment;

			person.empty = false;

			// Проверка правильности номера.
			if (person.floor < 1 || person.floor > COUNT_FLOORS || person.apartment < 1 || person.apartment > APARTMENTS_ON_FLOOR) {
				cout << "Такого номера не существует.\n";
				break;
			}

			// Проверям, занято ли или нет.
			if (!hotel.Busy(person.floor, person.apartment)) {
				hotel.AddPerson(person);
				cout << "Забронировано.\n";
			}
			else {
				cout << "Уже забронировано.\n";
			}

			break;
		case '2':
			cout << "Свободных номеров: " << hotel.FreeApartmentsCount() << "\n";
			break;
		default:
			run = false;
			hotel.WriteToDataBase();
		}
		cin.ignore(1);
	}
}