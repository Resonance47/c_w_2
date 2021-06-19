/*
Тема: разработка базы данных «Студенческий поток»

Общая часть задания

Студент потока характеризуется следующими данными:

- ФИО (до 50 символов);

- номер группы;

- набор из пяти оценок за последнюю сессию (без указания предметов);

- размер стипендии.

Необходимо:

1) разработать (и программно реализовать) динамические структуры данных и алгоритмы их обработки, позволяющие поддерживать выполнение следующих функций:

- консольный ввод/вывод данных о всех студентах потока;

- файловый ввод/вывод данных о потоке;

- редактирование данных о студентах и группах потока, включающее операции добавления/удаления групп и студентов;
2) Вывести перечень номеров групп, упорядоченный по значению показателя «число двоечников» / «списочная численность группы»;
3) Вывести на экран номера всех групп, не имеющих двоечников, в порядке убывания среднего (по группе) балла;
*/


#include <iostream>
#include <conio.h>
#include <string>
#include <cstring>
#include <windows.h>
#include <fstream>
////////////////////////////////////////////////
#define set_colour system("color 0B")
#define set_colour_2 system("color 03")
#define reset_colour system("color 0F")
////////////////////////////////////////////////
///service and protection
#define rusific 1251
int protect_cin(int in)
{
	while (1)
	{
		std::cin >> in;
		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(32767, '\n');
			std::cout << "\x1b[31mFail, try again: ";
		}
		else
		{
			std::cout << "\x1b[0m";
			return in;
		}
	}
}
void protect_cin(int in[5])
{
	for (int i = 0; i < 5; i++)
	{
		std::cin >> in[i];
		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(32767, '\n');
			std::cout << "\x1b[31mFail, try again: \x1b[0m";
			i = -1;
		}
	}
}
double protect_cin(double in)
{
	while (1)
	{
		std::cin >> in;
		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(32767, '\n');
			std::cout << "\x1b[31mFail, try again: ";
		}
		else
		{
			std::cout << "\x1b[0m";
			return in;
		}
	}
}
void cursor(bool status)
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	if (!status)
	{
		info.dwSize = 100;
		info.bVisible = FALSE;
		SetConsoleCursorInfo(consoleHandle, &info);
	}
	else
	{
		info.dwSize = 10;
		info.bVisible = TRUE;
		SetConsoleCursorInfo(consoleHandle, &info);
	}
}
void text_format(int height)
{
	SetConsoleCP(rusific);
	SetConsoleOutputCP(rusific);
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;                   // Width of each character in the font
	cfi.dwFontSize.Y = height;              // Height
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	rsize_t dest_size = sizeof("Consolas");
	wcscpy_s(cfi.FaceName, dest_size, L"Consolas"); // Choose your font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}
////////////////////////////////////////////////
///menu
int menu_u()
{
	cursor(0);
	int n = 1;
	int choice = 'p';
	while (choice != 13)
	{
		if (choice != 13)
		{
			if (n == 1)
			{
				std::cout << "\x1b[31m>\x1b[0m " << "\x1b[33m(1) Student\x1b[0m" << " \x1b[31m<\x1b[0m" << std::endl;
				std::cout << "  " << "(2) Group  " << "  " << std::endl;
				n = 0;
			}
			else
			{
				std::cout << "  " << "(1) Student" << "  " << std::endl;
				std::cout << "\x1b[31m>\x1b[0m " << "\x1b[33m(2) Group  \x1b[0m" << " \x1b[31m<\x1b[0m" << std::endl;
				n = 1;
			}
		}
		std::cout << "\x1b[2A";
		choice = _getch();
		if (choice == 224)
		{
			choice = _getch();
		}
		else if (choice == 27)
		{
			return 47;
		}
		else if (choice == 49 || choice == 50)
		{
			n = choice - 49;
			break;
		}
		else if (choice == 97 || choice == 98)
		{
			n = choice - 97;
			break;
		}
	}
	cursor(1);
	system("cls");
	return n;
}
int menu()
{
	cursor(0);
	int n = 5;
	int choice = 'p';
	while (choice != 13)
	{
		if (choice != 13)
		{
			if (n == 5)
			{
				std::cout << "\x1b[31m>\x1b[0m " << "\x1b[33m(1) F read\x1b[0m" << " \x1b[31m<\x1b[0m" << std::endl;
				std::cout << "  " << "(2) Add   " << "  " << std::endl;
				std::cout << "  " << "(3) Edit  " << "  " << std::endl;
				std::cout << "  " << "(4) Quest " << "  " << std::endl;
				std::cout << "  " << "(5) Show  " << "  " << std::endl;
				std::cout << "  " << "(6) Exit  " << "  " << std::endl;
				n = 0;
			}
			else if (n == 0)
			{
				std::cout << "  " << "(1) F read" << "  " << std::endl;
				std::cout << "\x1b[31m>\x1b[0m " << "\x1b[33m(2) Add   \x1b[0m" << " \x1b[31m<\x1b[0m" << std::endl;
				std::cout << "  " << "(3) Edit  " << "  " << std::endl;
				std::cout << "  " << "(4) Quest " << "  " << std::endl;
				std::cout << "  " << "(5) Show  " << "  " << std::endl;
				std::cout << "  " << "(6) Exit  " << "  " << std::endl;
				n = 1;
			}
			else if (n == 1)
			{
				std::cout << "  " << "(1) F read" << "  " << std::endl;
				std::cout << "  " << "(2) Add   " << "  " << std::endl;
				std::cout << "\x1b[31m>\x1b[0m " << "\x1b[33m(3) Edit  \x1b[0m" << " \x1b[31m<\x1b[0m" << std::endl;
				std::cout << "  " << "(4) Quest " << "  " << std::endl;
				std::cout << "  " << "(5) Show  " << "  " << std::endl;
				std::cout << "  " << "(6) Exit  " << "  " << std::endl;
				n = 2;
			}
			else if (n == 2)
			{
				std::cout << "  " << "(1) F read" << "  " << std::endl;
				std::cout << "  " << "(2) Add   " << "  " << std::endl;
				std::cout << "  " << "(3) Edit  " << "  " << std::endl;
				std::cout << "\x1b[31m>\x1b[0m " << "\x1b[33m(4) Quest \x1b[0m" << " \x1b[31m<\x1b[0m" << std::endl;
				std::cout << "  " << "(5) Show  " << "  " << std::endl;
				std::cout << "  " << "(6) Exit  " << "  " << std::endl;
				n = 3;
			}
			else if (n == 3)
			{
				std::cout << "  " << "(1) F read" << "  " << std::endl;
				std::cout << "  " << "(2) Add   " << "  " << std::endl;
				std::cout << "  " << "(3) Edit  " << "  " << std::endl;
				std::cout << "  " << "(4) Quest " << "  " << std::endl;
				std::cout << "\x1b[31m>\x1b[0m " << "\x1b[33m(5) Show \x1b[0m" << " \x1b[31m<\x1b[0m" << std::endl;
				std::cout << "  " << "(6) Exit  " << "  " << std::endl;
				n = 4;
			}
			else if (n == 4)
			{
				std::cout << "  " << "(1) F read" << "  " << std::endl;
				std::cout << "  " << "(2) Add   " << "  " << std::endl;
				std::cout << "  " << "(3) Edit  " << "  " << std::endl;
				std::cout << "  " << "(4) Quest " << "  " << std::endl;
				std::cout << "  " << "(5) Show  " << "  " << std::endl;
				std::cout << "\x1b[31m>\x1b[0m " << "\x1b[33m(6) Exit  \x1b[0m" << " \x1b[31m<\x1b[0m" << std::endl;
				n = 5;
			}
		}
		std::cout << "\x1b[6A";
		choice = _getch();
		if (choice == 224)
		{
			choice = _getch();
			if (choice == 72)
			{
				n -= 2;
			}
			if (n < 0)
				n = 5;
		}
		else if (choice == 27)
		{
			n = 5;
			break;
		}
		else if ((choice >= 49 && choice <= 54))
		{
			n = choice - 49;
			break;
		}
		else if ((choice >= 97 && choice <= 102))
		{
			n = choice - 97;
			break;
		}
	}
	cursor(1);
	if (n == 1)
	{
		system("cls");
		return n * 10 + menu_u();
	}
	else
	{
		return n;
	}
}
int group_menu()
{
	cursor(0);
	int n = 3;
	int choice = 'p';
	while (choice != 13)
	{
		if (choice != 13)
		{
			if (n == 3)
			{
				std::cout << "\x1b[31m>\x1b[0m " << "\x1b[33m(1) Edit number \x1b[0m" << " \x1b[31m<\x1b[0m" << std::endl;
				std::cout << "  " << "(2) Add student " << "  " << std::endl;
				std::cout << "  " << "(3) Edit student" << "  " << std::endl;
				std::cout << "  " << "(4) Delete group" << "  " << std::endl;

				n = 0;
			}
			else if (n == 0)
			{
				std::cout << "  " << "(1) Edit number " << "  " << std::endl;
				std::cout << "\x1b[31m>\x1b[0m " << "\x1b[33m(2) Add student \x1b[0m" << " \x1b[31m<\x1b[0m" << std::endl;
				std::cout << "  " << "(3) Edit student" << "  " << std::endl;
				std::cout << "  " << "(4) Delete group" << "  " << std::endl;

				n = 1;
			}
			else if (n == 1)
			{
				std::cout << "  " << "(1) Edit number " << "  " << std::endl;
				std::cout << "  " << "(2) Add student " << "  " << std::endl;
				std::cout << "\x1b[31m>\x1b[0m " << "\x1b[33m(3) Edit student\x1b[0m" << " \x1b[31m<\x1b[0m" << std::endl;
				std::cout << "  " << "(4) Delete group" << "  " << std::endl;
				n = 2;
			}
			else if (n == 2)
			{
				std::cout << "  " << "(1) Edit number " << "  " << std::endl;
				std::cout << "  " << "(2) Add student " << "  " << std::endl;
				std::cout << "  " << "(3) Edit student" << "  " << std::endl;
				std::cout << "\x1b[31m>\x1b[0m " << "\x1b[33m(4) Delete group\x1b[0m" << " \x1b[31m<\x1b[0m" << std::endl;
				n = 3;
			}
		}
		std::cout << "\x1b[4A";
		choice = _getch();
		if (choice == 224)
		{
			choice = _getch();
			if (choice == 72)
			{
				n -= 2;
			}
			if (n < 0)
				n = 2;
		}
		else if (choice == 27)
		{
			n = 47;
			break;
		}
		else if ((choice >= 49 && choice <= 52))
		{
			n = choice - 49;
			break;
		}
		else if ((choice >= 97 && choice <= 100))
		{
			n = choice - 97;
			break;
		}
	}
	cursor(1);
	system("cls");
	return n;
}
int stud_menu()
{
	cursor(0);
	int n = 4;
	int choice = 'p';
	while (choice != 13)
	{
		if (choice != 13)
		{
			if (n == 4)
			{
				std::cout << "\x1b[31m>\x1b[0m " << "\x1b[33m(1) Edit name       \x1b[0m" << " \x1b[31m<\x1b[0m" << std::endl;
				std::cout << "  " << "(2) Edit score      " << "  " << std::endl;
				std::cout << "  " << "(3) Edit scholarship" << "  " << std::endl;
				std::cout << "  " << "(4) Edit group      " << "  " << std::endl;
				std::cout << "  " << "(5) Delete          " << "  " << std::endl;

				n = 0;
			}
			else if (n == 0)
			{
				std::cout << "  " << "(1) Edit name       " << "  " << std::endl;
				std::cout << "\x1b[31m>\x1b[0m " << "\x1b[33m(2) Edit score      \x1b[0m" << " \x1b[31m<\x1b[0m" << std::endl;
				std::cout << "  " << "(3) Edit scholarship" << "  " << std::endl;
				std::cout << "  " << "(4) Edit group      " << "  " << std::endl;
				std::cout << "  " << "(5) Delete          " << "  " << std::endl;
				n = 1;
			}
			else if (n == 1)
			{
				std::cout << "  " << "(1) Edit name       " << "  " << std::endl;
				std::cout << "  " << "(2) Edit score      " << "  " << std::endl;
				std::cout << "\x1b[31m>\x1b[0m " << "\x1b[33m(3) Edit scholarship\x1b[0m" << " \x1b[31m<\x1b[0m" << std::endl;
				std::cout << "  " << "(4) Edit group      " << "  " << std::endl;
				std::cout << "  " << "(5) Delete          " << "  " << std::endl;
				n = 2;
			}
			else if (n == 2)
			{
				std::cout << "  " << "(1) Edit name       " << "  " << std::endl;
				std::cout << "  " << "(2) Edit score      " << "  " << std::endl;
				std::cout << "  " << "(3) Edit scholarship" << "  " << std::endl;
				std::cout << "\x1b[31m>\x1b[0m " << "\x1b[33m(4) Edit group      \x1b[0m" << " \x1b[31m<\x1b[0m" << std::endl;
				std::cout << "  " << "(5) Delete          " << "  " << std::endl;
				n = 3;
			}
			else if (n == 3)
			{
				std::cout << "  " << "(1) Edit name       " << "  " << std::endl;
				std::cout << "  " << "(2) Edit score      " << "  " << std::endl;
				std::cout << "  " << "(3) Edit scholarship" << "  " << std::endl;
				std::cout << "  " << "(4) Edit group      " << "  " << std::endl;
				std::cout << "\x1b[31m>\x1b[0m " << "\x1b[33m(5) Delete          \x1b[0m" << " \x1b[31m<\x1b[0m" << std::endl;
				n = 4;
			}
		}
		std::cout << "\x1b[5A";
		choice = _getch();//считываем
		if (choice == 224)
		{
			choice = _getch();
			if (choice == 72)
			{
				n -= 2;
			}
			if (n < 0)
				n = 4;
		}
		else if (choice == 27)
		{
			n = 47;
			break;
		}
		else if ((choice >= 49 && choice <= 53))
		{
			n = choice - 49;
			break;
		}
		else if ((choice >= 97 && choice <= 101))
		{
			n = choice - 97;
			break;
		}
	}
	cursor(1);
	system("cls");
	return n;
}
int menu_q()
{
	cursor(0);
	int n = 2;
	int choice = 'p';
	while (choice != 13)
	{
		if (choice != 13)
		{
			if (n == 2)
			{
				std::cout << "\x1b[31m>\x1b[0m " << "\x1b[33m(1) By population\x1b[0m" << " \x1b[31m<\x1b[0m" << std::endl;
				std::cout << "  " << "(2) By bad       " << "  " << std::endl;
				std::cout << "  " << "(3) Show 7       " << "  " << std::endl;
				n = 0;
			}
			else if (n == 0)
			{
				std::cout << "  " << "(1) By population" << "  " << std::endl;
				std::cout << "\x1b[31m>\x1b[0m " << "\x1b[33m(2) By bad       \x1b[0m" << " \x1b[31m<\x1b[0m" << std::endl;
				std::cout << "  " << "(3) Show 7       " << "  " << std::endl;
				n = 1;
			}
			else if (n == 1)
			{
				std::cout << "  " << "(1) By population" << "  " << std::endl;
				std::cout << "  " << "(2) By bad       " << "  " << std::endl;
				std::cout << "\x1b[31m>\x1b[0m " << "\x1b[33m(3) Show 7       \x1b[0m" << " \x1b[31m<\x1b[0m" << std::endl;
				n = 2;
			}
		}
		std::cout << "\x1b[3A";
		choice = _getch();
		if (choice == 224)
		{
			choice = _getch();
		}
		else if (choice == 27)
		{
			return 47;
		}
		else if (choice == 49 || choice == 50 || choice == 51)
		{
			n = choice - 49;
			break;
		}
		else if (choice == 97 || choice == 98)
		{
			n = choice - 97;
			break;
		}
	}
	cursor(1);
	system("cls");
	return n;
}
////////////////////////////////////////////////
///main classes
class Student
{
private:
	////////////////Main student class////////////////
	Student* next;
	int g_num;//group number
	std::string full_name;
	int score[5];
	int sc_ship;//scholarship
	Student(std::string full_name, int score[5], int sc_ship, int g_num)
	{
		this->full_name = full_name;
		for (int i = 0; i < 5; i++)
		{
			this->score[i] = score[i];
		}
		this->sc_ship = sc_ship;
		next = nullptr;
		this->g_num = g_num;
	}
public:
	//////////////////////////////////////////////////
	///getters & setters
	void get_inf()
	{
		//getter
		std::cout << "\x1b[33m" << full_name << "\x1b[0m\n";
		std::cout << "\x1b[32m\tScore: \x1b[0m";
		for (int i = 0; i < 5; i++)
		{
			std::cout << score[i] << " ";
		}
		std::cout << std::endl << "\x1b[32m\tScholarship: \x1b[0m" << sc_ship << std::endl;
	}
	std::string get_name()
	{
		return full_name;
	}
	int* get_score()
	{
		return score;
	}
	int get_sc_ship()
	{
		return sc_ship;
	}
	void set_sc_ship()
	{
		//std::cout << "Old \x1b[33m" << full_name << "'s\x1b[0m scholarship:\x1b[32m" << sc_ship;
		std::cout << "\nEnter new scholarship: \x1b[32m";
		sc_ship = 0;
		sc_ship = protect_cin(sc_ship);
		std::cout << "\x1b[0m";
	}
	int* set_score()
	{
		int* sum_old = new int[5];
		std::cout << "\nEnter new score: \x1b[32m";
		for (int i = 0; i < 5; i++)
		{
			sum_old[i] = score[i];
			score[i] = protect_cin(score[i]);
		}
		std::cout << "\x1b[0m";
		return sum_old;
	}
	/////////////////////////////////////////////////
	///friends
	friend class Group;
	////////////////////////////////////////////////
};
class Group
{
	////////////////Main group class////////////////
private:
	Student* first;	//pointer to the head of the group;
	Student* actual;
	int num;//group number
	int sum_score;
	int bad;
	int pop;//population 
	Group* next;//pointer to the next logical group by number
	void headman(int score[5])//it collects score and bad
	{
		pop++;
		sum_score += score[0] + score[1] + score[2] + score[3] + score[4];
		if (score[0] <= 2 || score[1] <= 2 || score[2] <= 2 || score[3] <= 2 || score[4] <= 2)//if bad (двоечник)
		{
			bad++;
		}
	}
public:
	Group(int num)
	{
		this->num = num;
		bad = 0;
		sum_score = 0;
		pop = 0;
		next = 0;
		first = 0;
		actual = 0;
	}
	/*
	//adding without sorting
	void add_student(std::string full_name, int score[5], int sc_ship)
	{
		if (first)
		{
			actual->next = new Student(full_name, score, sc_ship, num);
			actual = actual->next;
			actual->next = 0;
		}
		else
		{
			first = new Student(full_name, score, sc_ship, num);
			actual = first;
		}
		headman(score);
	}
	*/
	void headman(Student* new_sum, int* old)
	{
		int sum = 0;
		int sum_old = 0;
		bool worse = false;
		bool old_worse = false;
		for (int i = 0; i < 5; i++)
		{
			sum_old += old[i];
			if (old[i] < 3)
				old_worse = true;
			sum += new_sum->score[i];
			if (new_sum->score[i] < 3)
				worse = true;
		}
		sum_score += (sum - sum_old);
		if (worse && !old_worse)
			bad++;
		else if (!worse && old_worse)
			bad--;
		delete old;
	}
	void add_student(std::string full_name, int score[5], int sc_ship)//adding in alphabetical order
	{
		Student* extra;
		Student* prev;
		if (first)
		{
			if (first->full_name > full_name)
			{
				prev = new Student(full_name, score, sc_ship, num);
				std::cout << "\x1b[33m" << full_name << "\x1b[0m - \x1b[32msuccess\n\x1b[0m";
				prev->next = first;
				first = prev;
			}
			else if (first->full_name == full_name)
			{
				std::cout << "Student \x1b[33m" << full_name << "\x1b[0m have already joined\n";
				return;
			}
			else
			{
				extra = first->next;
				prev = first;
				while (extra)
				{
					if (extra->full_name == full_name)
					{
						std::cout << "Student \x1b[33m" << full_name << "\x1b[0m have already joined\n";
						return;
					}
					else if (extra->full_name > full_name)
					{
						prev->next = 0;
						prev->next = new Student(full_name, score, sc_ship, num);
						prev->next->next = extra;
						prev = prev->next;
						std::cout << "\x1b[33m" << full_name << "\x1b[0m - \x1b[32msuccess\n\x1b[0m";
						break;
					}
					prev = prev->next;
					extra = extra->next;
				}
				if (!extra)
				{
					prev->next = new Student(full_name, score, sc_ship, num);
					std::cout << "\x1b[33m" << full_name << "\x1b[0m - \x1b[32msuccess\n\x1b[0m";
					actual = prev->next;
					actual->next = 0;
				}
			}
		}
		else
		{
			first = new Student(full_name, score, sc_ship, num);
			std::cout << "\x1b[33m" << full_name << "\x1b[0m - \x1b[32msuccess\n\x1b[0m";
			first->next = 0;
			actual = first;
		}
		headman(score);
	}
	Student* find_student(std::string name, Student*& witness)//it returns pointer to the finding student and pointer to the previous student
	{
		if (first)
		{
			Student* kill;
			kill = first;
			witness = first;
			if (first->full_name == name)
			{
				witness = 0;
				return kill;
			}
			else
			{
				kill = kill->next;
			}
			while (kill)
			{
				if (kill->full_name == name)
				{
					return kill;
				}
				kill = kill->next;
				witness = witness->next;
			}
			std::cout << "Student escaped :/ \n";
		}
		else
		{
			std::cout << "This group is empty\n";
		}
		return 0;
	}
	Student* find_student(std::string name)//it returns pointer to the finding student and pointer to the previous student
	{
		if (first)
		{
			Student* kill;
			kill = first;
			Student* witness = first;
			if (first->full_name == name)
			{
				witness = 0;
				return kill;
			}
			else
			{
				kill = kill->next;
			}
			while (kill)
			{
				if (kill->full_name == name)
				{
					return kill;
				}
				kill = kill->next;
				witness = witness->next;
			}
			std::cout << "Student escaped :/ \n";
		}
		else
		{
			std::cout << "This group is empty\n";
		}
		return 0;
	}
	bool show_student(std::string name)//it returns 1 if the student exists, 2 if doesn't
	{
		Student* prev = 0;
		Student* model = 0;
		model = find_student(name, prev);
		if (model)
		{
			std::cout << "\x1b[41mFrom group " << num << ":\x1b[0m\n";
			model->get_inf();
			return true;
		}
		return false;
	}
	////////////////////////////////////////////////
	///getters & setters
	double get_aver()//average score calculation
	{
		return sum_score / pop;
	}
	int get_bad()
	{
		return bad;
	}
	int get_enum()
	{
		//set_colour_2;
		if (!first)
			return 0;
		actual = first;
		std::cout << "\x1b[41mGroup " << num << "\x1b[0m\n";
		std::cout << "\x1b[35mAverage score: " << sum_score / ((double)pop * 5) << std::endl << "Bad: " << bad << "\x1b[0m\n";
		while (actual->next)
		{
			actual->get_inf();
			std::cout << "\n";
			actual = actual->next;
		}
		actual->get_inf();

		//reser_colour;
		std::cout << "\n\n";
		return 1;
	}
	int get_num()
	{
		return num;
	}
	void set_num(int num)
	{
		this->num = num;
	}
	////////////////////////////////////////////////
	///friends
	friend void add_group(int n, Group*& zero, Group*& end);
	friend void add_into(int n, Group*& zero, Group*& end, int i);
	friend void read_file(Group*& zero, Group*& end, int i);
	friend void delete_group(int n, Group*& zero, Group*& end);
	friend void group_request(Group* zero, Group* end, int& n);
	friend void delete_all(Group*& zero, Group*& end);
	friend void show_all(Group* end);
	friend void sort_by_bad_output(Group* zero, Group* end);
	friend void sort_by_pop_output(Group* zero, Group* end);
	friend void sort_by_aver_without_bad(Group* zero, Group* end);
	friend Group* group_find(Group* crime_scene, int n);
	friend void edit_num(Group*& zero, Group*& end, Group*& here, int new_num);
	friend class Student;
	////////////////////////////////////////////////
	///deleting
	void delete_student(std::string name)//todo пофиксить
	{
		bool b = 0;
		Student* prev = first;
		Student* victim = find_student(name, prev);
		if (victim)
		{
			int sum_s = 0;
			for (int i = 0; i < 5; i++)
			{
				if (victim->score[i] <= 2)
				{
					b = true;
				}
				sum_s += victim->score[i];
			}
			if (!prev)
			{
				first = first->next;
			}
			else if (!victim->next)
			{
				prev->next = 0;
			}
			else
			{
				prev->next = victim->next;
			}
			delete victim;
			pop--;
			if (b)
				bad--;
			sum_score -= sum_s;
		}
	}
	~Group()
	{
		while (first)
		{
			actual = first;
			first = first->next;
			delete actual;
		}
	}
	////////////////////////////////////////////////
};
////////////////////////////////////////////////
///redact function
void read_file(Group*& zero, Group*& end, int i)
{
	system("cls");
	std::string adress = "a.txt";
	std::string key;
	std::string name;
	int n;
	int score[5];
	int s;
	std::ifstream in(adress); // окрываем файл для чтения
	if (in)
	{
		for (i; i > 0; i--)
		{
			name = "";
			in >> key;
			name += key;
			while (1)
			{
				in >> key;
				try
				{
					n = stoi(key);
					break;
				}
				catch (std::invalid_argument e)
				{
					name += (' ' + key);
				}
			}
			for (int j = 0; j < 5; j++)
			{
				in >> score[j];
			}
			in >> s;
			Group* head = zero;
			while (1)
			{
				if (head)
				{
					if (head->get_num() == n)
					{

						head->add_student(name, score, s);
						break;
					}
					head = head->next;
				}
				else
				{
					//clear without explanation
					std::cout << "Group \x1b[31m" << n << "\x1b[0m doesn't exist. Do you want to add it now?\nPress \x1b[31menter\x1b[0m to add or any other key to exit\n";
					if (_getch() == 13)
					{
						add_group(n, zero, end);
						head = zero;
					}
					else
					{
						break;
					}
				}
			}
		}
	}
	else
	{
		system("cls");
		std::cout << "Error! File is not found\nPress enter to continue...\n";
		system("pause");
	}
	system("pause");
}
void add_group(int n, Group*& zero, Group*& end)
{
	if (!zero)//if groups doesn't exist
	{
		zero = new Group(n);
		end = zero;
	}
	else//create sort queue
	{
		Group* find = zero;
		Group* prev = 0;
		if (n < zero->num)
		{
			find = new Group(n);
			find->next = zero;
			zero = find;
			return;
		}
		if (n == zero->num)
		{
			std::cout << "\x1b[31mThis num is reserved!\x1b[0m\n";
			system("pause");
			return;
		}
		else
		{
			prev = find;
			find = find->next;
		}
		while (find)
		{
			if (find->num == n)
			{
				std::cout << "\x1b[31mThis num is reserved!\x1b[0m\n";
				system("pause");
				return;
			}
			if (n < find->num)
			{
				prev->next = 0;
				prev->next = new Group(n);
				prev->next->next = find;
				return;
			}
			else
			{
				find = find->next;
			}
		}
		end->next = new Group(n);
		end = end->next;
	}
}
void add_into(int n, Group*& zero, Group*& end, int i)//adding pack of students or groups
{
	for (i; i > 0; i--)//creating i students as we was asked
	{
		Group* head = zero;
		while (1)
		{
			if (head)
			{
				if (head->get_num() == n)
				{
					std::cout << "Enter full name: \x1b[33m";
					std::string name;
					std::cin.get();
					std::getline(std::cin, name);

					std::cout << "\x1b[0m\tEnter score: \x1b[32m";
					int score[5] = { 0, 0, 0, 0, 0 };
					protect_cin(score);
					std::cout << "\x1b[0m\tEnter the scholarship: \x1b[32m";
					int s = 0;
					s = protect_cin(s);
					std::cout << "\x1b[0m";
					head->add_student(name, score, s);
					break;
				}
				head = head->next;
			}
			else
			{
				//clear without explanation
				std::cout << "This group doesn't exist. Do you want to add it now?\nPress \x1b[31menter\x1b[0m to add or any other key to exit\n";
				if (_getch() == 13)
				{
					add_group(n, zero, end);
					head = zero;
					std::cout << "\x1b[32mGroup have created\x1b[0m\n\n";
				}
				else
				{
					return;
				}
			}
		}
	}
	system("pause");//to control
}
void edit_num(Group*& zero, Group*& end, Group*& here, int new_num)
{
	Group* prev = zero;
	if (end->get_num() == here->get_num() && here->get_num() <= new_num)
	{
		here->set_num(new_num);
		return;
	}
	else if (zero->get_num() == here->get_num() && new_num <= here->get_num())
	{
		here->set_num(new_num);
		return;
	}

	while (prev->next)
	{
		if (prev->next->get_num() == here->get_num() || zero->get_num() == here->get_num())
		{
			if (new_num < zero->get_num())
			{
				prev->next = here->next;
				here->next = zero;
				zero = here;
				zero->set_num(new_num);
				return;
			}
			Group* new_prev = zero;
			while (new_prev->next)
			{
				if (new_num > new_prev->get_num() && new_num < new_prev->next->get_num())
				{
					if (zero->get_num() == here->get_num())
					{
						zero = zero->next;
					}
					prev->next = here->next;
					here->next = new_prev->next;
					new_prev->next = here;
					here->set_num(new_num);
					return;
				}
				new_prev = new_prev->next;
			}
			prev->next = here->next;
			if (zero->get_num() == here->get_num())
			{
				zero = zero->next;
			}
			end->next = here;
			end = here;
			end->next = 0;
			here->set_num(new_num);
			return;
		}
		prev = prev->next;
	}
}
void transit_by_deleting(Group* zero, Group* end, Group* here, Student* she)
{
	std::cout << "\nWhere do you want to transfer " << "\x1b[33m" << she->get_name() << "\x1b[0m?\n";
	int num;
	group_request(zero, end, num);
	if (num == here->get_num())
		return;
	zero = group_find(zero, num);
	if (zero)
	{
		zero->add_student(she->get_name(), she->get_score(), she->get_sc_ship());
		here->delete_student(she->get_name());
	}
	else
	{
		std::cout << "This group doesn't exist!\n";
		system("pause");
	}

}
void rename_by_deleting(Group* here, Student* she, std::string new_name)
{
	here->add_student(new_name, she->get_score(), she->get_sc_ship());
	here->delete_student(she->get_name());
}
void delete_group(int n, Group*& zero, Group*& end)
{
	if (zero)
	{
		Group* find = zero;
		Group* prev = zero;
		if (n == find->get_num())//if we have to kill the first:
		{
			zero = zero->next;
			delete find;
			return;
		}
		else
		{
			find = find->next;
		}
		while (find)//up to the end
		{
			if (n == find->get_num())//if we find a victim
			{
				if (!find->next)
				{
					delete find;
					end = prev;
					end->next = 0;
					return;
				}
				else
				{
					prev->next = find->next;
					delete find;
					return;
				}
			}
			find = find->next;
			prev = prev->next;
		}
	}
}
void delete_all(Group*& zero, Group*& end)
{
	while (zero)
	{
		end = zero->next;
		delete zero;
		zero = end;
	}
	zero = nullptr;
	end = nullptr;
}
////////////////////////////////////////////////
///show & find function
void group_request(Group* zero, Group* end, int& n)//here is entering group numb to create, find, redact etc
{
	Group* count = zero;
	std::cout << "Here is your groups:";
	std::cout << "\x1b[31m";

	while (count)
	{
		std::cout << " " << count->get_num();
		count = count->next;
	}
	std::cout << "\x1b[0m\n";
	std::cout << "Enter group number: \x1b[31m";
	n = protect_cin(n);
	std::cout << "\x1b[0m";
}
void show_all(Group* end)
{
	if (end)
	{
		while (end->next)
		{
			end->get_enum();
			end = end->next;
		}
		end->get_enum();
		system("pause");
	}
}
Group* group_find(Group* crime_scene, int n)
{
	while (crime_scene)
	{
		if (crime_scene->get_num() == n)
			return crime_scene;
		crime_scene = crime_scene->next;
	}
	return 0;
}
/// Следующие две фунции сделаны так криво потому, что после редактирования данных о студентах,
/// затраты на перераспределение групп были бы еще больше и код стал бы настолько громоздким, 
/// что его было бы невозможно кооректировать.
void sort_by_bad_output(Group* zero, Group* end)
{
	//terrible heresy :(
	Group** here;
	int size = 0;
	Group* count = zero;
	while (count)
	{
		size++;
		count = count->next;
	}
	here = new Group * [size];
	count = zero;
	for (int i = 0; i < size; i++)
	{
		here[i] = count;
		count = count->next;
	}

	for (int i = 0; i < size - 1; i++)
	{
		for (int j = 0; j < size - i - 1; j++)
		{
			if (here[j]->get_bad() > here[j + 1]->get_bad())
			{
				count = here[j];
				here[j] = here[j + 1];
				here[j + 1] = count;
			}
		}
	}
	for (int i = 0; i < size; i++)
	{
		here[i]->get_enum();
	}
	delete[] here;
}
void sort_by_pop_output(Group* zero, Group* end)
{
	//terrible heresy :(
	Group** here;
	int size = 0;
	Group* count = zero;
	while (count)
	{
		size++;
		count = count->next;
	}
	here = new Group * [size];
	count = zero;
	for (int i = 0; i < size; i++)
	{
		here[i] = count;
		count = count->next;
	}
	for (int i = 0; i < size - 1; i++)
	{
		for (int j = 0; j < size - i - 1; j++)
		{
			if (here[j]->pop > here[j + 1]->pop)
			{
				count = here[j];
				here[j] = here[j + 1];
				here[j + 1] = count;
			}
		}
	}
	for (int i = 0; i < size; i++)
	{
		here[i]->get_enum();
	}
	delete[] here;
}
void sort_by_aver_without_bad(Group* zero, Group* end)
{
	Group** here;
	int size = 0;
	Group* count = zero;
	while (count)
	{
		size++;
		count = count->next;
	}
	here = new Group * [size];
	count = zero;
	for (int i = 0; i < size; i++)
	{
		here[i] = count;
		count = count->next;
	}
	for (int i = 0; i < size - 1; i++)
	{
		for (int j = 0; j < size - i - 1; j++)
		{
			if (here[j]->get_aver() > here[j + 1]->get_aver())
			{
				count = here[j];
				here[j] = here[j + 1];
				here[j + 1] = count;
			}
		}
	}
	for (int i = size - 1; i > 0; i--)
	{
		if (here[i]->bad == 0)
			here[i]->get_enum();
	}
	delete[] here;

}
////////////////////////////////////////////////
/*
Decoding the menu:
F read         = 0
Add student    = 10
Add group      = 11
Edit           = 2
Quest          = 3
Show           = 4
Exit           = 5
*/
int main()
{
	text_format(20);
	Group* zero = 0;
	Group* end = 0;
	Group* crime_scene = zero;
	int n;
	std::string name;
	/*
	* https://habr.com/ru/post/94647/ *
	\x1b[31m	//red
	\x1b[32m	//green
	\x1b[33m	//yellow
	\x1b[34m	//blue
	\x1b[35m	//violet
	\x1b[36m	//light blue
	\x1b[41m
	\x1b[42m
	\x1b[43m
	\x1b[44m
	\x1b[45m
	\x1b[46m
	\x1b[0m
	*/
	set_colour;
	std::cout << "Hi, press any key to start...\n";
	n = _getch();
	reset_colour;
	system("cls");
	while (1)
	{
		n = menu();
		if (n == 0)////done 80%
		{
			//F_read"
			//TODO: чтение из определенного файла
			system("cls");
			std::cout << "How many students do u want to add?: ";
			n = protect_cin(n);
			read_file(zero, end, n);
		}
		else if (n == 10)//done 100%
		{
			//Add student
			int i = 0;
			group_request(zero, end, n);
			std::cout << "\x1b[0mEnter how many students do u want to add: ";
			i = protect_cin(i);
			std::cout << "\n";
			add_into(n, zero, end, i);
			system("cls");
		}
		else if (n == 11)//done 100%
		{
			//Add group
			group_request(zero, end, n);
			add_group(n, zero, end);
		}
		else if (n == 2)//done 100%
		{
			//Edit group
			system("cls");
			group_request(zero, end, n);
			crime_scene = zero;
			crime_scene = group_find(crime_scene, n);
			system("cls");
			if (crime_scene)
			{
				while (1)
				{
					crime_scene->get_enum();
					n = group_menu();
					if (n == 0)//done 100%
					{
						//Edit name
						group_request(zero, end, n);
						if (!group_find(zero, n))
						{
							edit_num(zero, end, crime_scene, n);
						}
						else
						{
							std::cout << "\n\x1b[31mError, this number has been reserved!\x1b[0m\n";
							system("pause");
						}
					}
					else if (n == 1)
					{
						//Add student, the same
						add_into(crime_scene->get_num(), zero, end, 1);
						system("cls");
					}
					else if (n == 2)//done 100%
					{
						//Edit student
						crime_scene->get_enum();
						std::cout << "Enter full name: \x1b[33m";
						std::cin.get();
						std::getline(std::cin, name);
						std::cout << "\x1b[0m";
						system("cls");
						while (crime_scene->show_student(name))
						{
							//trasfer
							n = stud_menu();
							if (n == 0)
							{
								//Edit name
								crime_scene->show_student(name);
								std::cout << "\nEnter new name: \x1b[33m";
								std::string new_name;
								std::getline(std::cin, new_name);
								std::cout << "\x1b[0m";
								if (!crime_scene->find_student(new_name))
								{
									rename_by_deleting(crime_scene, crime_scene->find_student(name), new_name);
									name = new_name;
								}
							}
							else if (n == 1)
							{
								crime_scene->show_student(name);
								crime_scene->headman(crime_scene->find_student(name), crime_scene->find_student(name)->set_score());
							}
							else if (n == 2)
							{
								//Edit Scholarship
								crime_scene->show_student(name);
								(crime_scene->find_student(name))->set_sc_ship();
							}
							else if (n == 3)
							{
								//Transit
								crime_scene->show_student(name);
								transit_by_deleting(zero, end, crime_scene, crime_scene->find_student(name));
							}
							else if (n == 4)
							{
								//delete
								crime_scene->delete_student(name);
							}
							else
							{
								break;
							}
							system("cls");
						}
					}
					else if (n == 3)//done 100%
					{
						//Delete group
						delete_group(crime_scene->get_num(), zero, end);
						crime_scene = 0;
						break;
					}
					else
					{
						break;
					}
					system("cls");
				}
			}
			else
			{
				std::cout << "This group doesn't exist\n";
				system("pause");
			}

		}
		else if (n == 3)//done 30%
		{
			system("cls");
			n = menu_q();
			if (n == 0)
			{
				sort_by_pop_output(zero, end);
				system("pause");
			}
			else if (n == 1)
			{
				sort_by_bad_output(zero, end);
				system("pause");
			}
			else if (n == 2)
			{
				sort_by_aver_without_bad(zero, end);
				system("pause");
			}
			//Quest
		}
		else if (n == 4)//done 100%
		{
			system("cls");
			show_all(zero);
			//Show
		}
		else if (n == 5)//done 100%
		{
			//Exit
			break;
		}
		system("cls");
	}
	////---Here is deleting:---/////
	delete_all(zero, end);
	system("cls");
	std::cout << "\x1b[1;36mAu revoir...\x1b[0m";
	crime_scene = nullptr;
	////////////////////////////////
	return 47;
}