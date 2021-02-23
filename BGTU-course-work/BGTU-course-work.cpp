#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <locale.h>
#include <stdlib.h>
#include <windows.h>


struct Toy {
	char  name[21]; //название
	int   price;    //цена
	int   k;        //количество
	char  age[6];   //возраст
};

typedef Toy Data;

struct List {
	Data data;
	List *next;
};

void  printCenter(char*);      //вывод текста по центру
void  checkRus(char*);         //корректное чтение русских символов
List* clear(List*);            //тихая очистка памяти
List* cleare(char*, List*);    //очистка памяти
void  printLine();             //вывод линии
void  printHead();             //вывод шапки таблицы
void  printNote(Data, int);    //вывод записи
List* inputNote(List*);        //ввод записи
void  newFname(char*);         //ввод имени файла
List* fileRead(char*, List*);  //чтение списка из файла
void  fileWrite(char*, List*); //запись списка в файл
void  fileGenerate(char*);     //генерирование файла с записями
void  view(List*);             //просмотр списка
List* add(List*, Data);        //добавление записи
List* del(List*);              //удаление записи
void  edit(List*);             //редактирование записи
void  age(List*);              //запрос по цене и возрасту
void  kit(List*);              //запрос самого дорогого конструктора
void  exit(char*, List*);      //выход из программы

int main(void) {

	system("color F0");
	setlocale(LC_ALL, "rus");

	List *list = NULL;
	char fname[21] = "data.bin";
	int c = 0;

	while (c != 27) {
		system("cls");
		printCenter("*^v^*   Меню   *^v^*\n\n\n\n");
		printCenter("0   ......... Добавление нового имени файла");
		printCenter("1   .......... Сгенерировать файл с данными");
		printCenter("2   ................ Чтение списка из файла");
		printCenter("3   .................. Запись списка в файл");
		printCenter("4   ....................... Просмотр списка");
		printCenter("5   ..................... Добавление записи");
		printCenter("6   ....................... Удаление записи");
		printCenter("7   ................. Редактирование записи");
		printCenter("8   ............. Запрос по цене и возрасту");
		printCenter("9   ... Запрос самого дорогого конструктора");
		printCenter("C   ........................ Очистка памяти");
		printCenter("ESC .................... Выход из программы");
		printf("\n\n");
		if (c == 'c') printCenter("> Память очищена <\n");
		else printf("\n\n");
		printCenter("Ожидание выбора пункта меню > ");
		c = _getche();
		if (c == 48)  newFname(fname);
		if (c == 49)  fileGenerate(fname);
		if (c == 50)  list = fileRead(fname, list);
		if (c == 51)  fileWrite(fname, list);
		if (c == 52)  view(list);
		if (c == 53)  list = inputNote(list);
		if (c == 54)  list = del(list);
		if (c == 55)  edit(list);
		if (c == 56)  age(list);
		if (c == 57)  kit(list);
		if (c == 'c') list = cleare(fname, list);
		if (c == 27)  exit(fname, list);
	}

	Sleep(100);
	return 0;
}

//вывод текста по центру
void printCenter(char *buf) {
	printf("\n%*s", 80 / 2 + strlen(buf) / 2, buf);
}

//корректное чтение русских символов
void checkRus(char *buf) {
	char result[80] = "";
	char ch[2] = "";
	for (int i = 0; buf[i] != '\0' && buf[i] != '\n'; i++) {
		if (buf[i] >= -128 && buf[i] <= -81)       buf[i] += 64;
		else if (buf[i] == -15)                    buf[i] -= 57;
		else if (buf[i] >= -32 && buf[i] <= -17)   buf[i] += 16;
		else if (buf[i] == -16)                    buf[i] -= 72;
		ch[0] = buf[i];
		ch[1] = '\0';
		strcat(result, ch);
	}
	strcpy(buf, result);
}

//тихая очистка памяти
List* clear(List *list) {
	List *top = list;
	List *tmp;

	while (list != NULL) {
		tmp = list;
		list = list->next;
		delete tmp;
	}
	return list;
}

//очистка памяти
List* cleare(char *fname, List *list) {
	List *top = clear(list);

	system("cls");
	printCenter("*^.^* Очистка памяти *^.^*\n\n");
	printf("Сохранить данные в %s перед очисткой? (y/n) ", fname);
	if (_getche() == 'y') fileWrite(fname, list);

	return top;
}

//вывод линии
void printLine() {
	for (int i = 0; i < 80; i++)
		printf("-");
	printf("\n");
}

//вывод шапки таблицы
void printHead() {
	printLine();
	printf("  N  |        Название        |     Цена     |    Количество    |    Возраст\n");
	printLine();
}

//вывод записи
void printNote(Data data, int n) {
	printf("%3d  |  %-22s|%9d     |%14d    |%10s\n", n++, data.name, data.price, data.k, data.age);
}

//ввод записи
List* inputNote(List *list) {
	Data data;

	system("cls");
	printCenter("*^.^* Добавление записи *^.^*\n\n");
	printf("Введите название (20 символов): ");
	gets_s(data.name);
	checkRus(data.name); //корректировка русского языка
	printf("Введите цену:                   ");
	scanf("%d", &data.price);
	printf("Введите количество:             ");
	scanf("%d", &data.k);
	printf("Введите возраст (хх-хх):        ");
	scanf("%s", &data.age);

	list = add(list, data);
	printf(">Запись добавлена\n");

	while (scanf("%*c")); //очистка буфера
	printCenter("Нажмите любую клавишу для продолжения > ");
	_getch();
	return list;
}

//ввод имени файла
void newFname(char *fname) {
	system("cls");
	printCenter("*^.^* Добавление нового имени файла *^.^*\n\n");
	printf("Имя файла: %s\nИзменить? (y/n) ", fname);
	if (_getche() == 'y') {
		printf("\nВведите новое имя файла (20 символов): ");
		gets_s(fname, 20);
	}
	printf(">Добавлено новое имя файла\n");

	while (scanf("%*c")); //очистка буфера
	printCenter("Нажмите любую клавишу для продолжения > ");
	_getch();
}

//чтение списка из файла
List* fileRead(char *fname, List *list) {
	FILE *f;
	Data data;
	bool err = false;

	system("cls");
	printCenter("*^.^* Чтение из файла *^.^*\n\n");
	f = fopen(fname, "rb");
	if (!f) {
		printf("Ошибка открытия файла для чтения!\n");
		err = true;
	}
	if (!err) {
		list = clear(list); //очистка памяти
		while (fread(&data, sizeof(Data), 1, f) == 1)
			list = add(list, data);
		if (!err) printf(">Данные занесены в память\n");
	}
	fclose(f);
	printCenter("Нажмите любую клавишу для продолжения > ");
	_getch();
	return list;
}

//запись списка в файл
void fileWrite(char *fname, List *list) {
	FILE *f;
	bool err = false;

	system("cls");
	printCenter("*^.^* Запись в файл *^.^*\n\n");
	f = fopen(fname, "wb");
	if (!f) {
		printf("Ошибка открытия файла для записи!\n");
		err = true;
	}
	if (!err) {
		while (list != NULL) {
			if (fwrite(&list->data, sizeof(Data), 1, f) != 1) {
				printf("Ошибка записи!\n");
				err = true;
				break;
			}
			list = list->next;
		}
		if (!err) printf(">Данные записаны в файл\n");
	}
	fclose(f);
	printCenter("Нажмите любую клавишу для продолжения > ");
	_getch();
}

//генерирование файла с записями
void fileGenerate(char *fname) {
	FILE *f;
	Toy toy;
	bool err = false;

	system("cls");
	printCenter("*^.^* Генерирование файла с данными *^.^*\n\n");
	f = fopen(fname, "wb");
	if (!f) {
		printf("Ошибка открытия файла для записи сгенерированных данных!\n");
		err = true;
	}
	if (!err) {
		toy = { "Конструктор 1", 267,  18, "05-12" };
		fwrite(&toy, sizeof(Toy), 1, f);
		toy = { "Конструктор 2", 732,  7,  "08-13" };
		fwrite(&toy, sizeof(Toy), 1, f);
		toy = { "Конструктор 3", 1348, 9,  "08-14" };
		fwrite(&toy, sizeof(Toy), 1, f);
		toy = { "Машинка",       154,  13, "03-10" };
		printf(">Данные сгенерированы\n");
		if (fwrite(&toy, sizeof(Toy), 1, f) == 1)
			printf(">Файл %s обновлен\n", fname);
	}
	fclose(f);
	printCenter("Нажмите любую клавишу для продолжения > ");
	_getch();
}

//просмотр списка
void view(List *list) {
	int n = 1;

	system("cls");
	printCenter("*^.^* Просмотр записей *^.^*\n\n");
	if (list != NULL) {
		printHead();
		while (list != NULL) {
			printNote(list->data, n++);
			list = list->next;
		}
		printLine();
		printf("\n");
	}
	else printf("Ошибка! Список пуст\n");
	printCenter("Нажмите любую клавишу для продолжения > ");
	_getch();
}

//добавление записи
List* add(List *list, Data data) {
	List *last = list;
	List *tmp = new List;

	tmp->data = data;
	tmp->next = NULL;
	if (last == NULL) return tmp;
	while (last->next != NULL)
		last = last->next;
	last->next = tmp;
	return list;
}

//удаление записи
List* del(List *list) {
	List *top = list;
	List *tmp = NULL;
	bool flag = true;
	int n = 1, k;

	system("cls");
	printCenter("*^.^* Удаление записи *^.^*\n\n");
	if (list != NULL) {
		printf("Введите номер записи: ");
		scanf("%d", &k);
		while (list != NULL) {
			if (k == 1) {
				tmp = list;
				list = list->next;
				delete tmp;
				top = list;
				flag = false;
				break;
			}
			else if ((k - 1) == n++) {
				if (list->next != NULL) {
					flag = false;
					if (list->next->next != NULL)
						tmp = list->next->next;
				}
				delete list->next;
				if (tmp != NULL) {
					list->next = tmp;
				}
				else list->next = NULL;
				break;
			}
			list = list->next;
		}
		if (flag) printf("Ошибка! Запись с таким номером не найдена\n");
		else printf(">Запись удалена\n");
	}
	else printf("Ошибка! Список пуст\n");
	printCenter("Нажмите любую клавишу для продолжения > ");
	_getch();
	return top;
}

//редактирование записи
void edit(List *list) {
	Data data;
	bool flag = true;
	int n = 1, k;

	system("cls");
	printCenter("*^.^* Редактирование записи *^.^*\n\n");
	if (list != NULL) {
		printf("Введите номер записи: ");
		scanf("%d", &k);
		while (list != NULL) {
			if (k == n++) {
				data = list->data;
				printf("Название:   %s\nИзменить? (y/n) ", data.name);
				if (_getche() == 'y') {
					while (scanf("%*c")); //очистка буфера
					printf("\nВведите новое название (20 символов):   ");
					gets_s(data.name);
					checkRus(data.name); //корректировка русского языка
				}
				else printf("\n");
				printf("Цена:       %d\nИзменить? (y/n) ", data.price);
				if (_getche() == 'y') {
					printf("\nВведите новую цену:                     ");
					scanf("%d", &data.price);
				}
				else printf("\n");
				printf("Количество: %d\nИзменить? (y/n) ", data.k);
				if (_getche() == 'y') {
					printf("\nВведите новое количество:               ");
					scanf("%d", &data.k);
				}
				else printf("\n");
				printf("Возраст:    %s\nИзменить? (y/n) ", data.age);
				if (_getche() == 'y') {
					printf("\nВведите новый возраст (хх-хх):          ");
					scanf("%s", &data.age);
				}
				else printf("\n");
				list->data = data;
				flag = false;
				break;
			}
			list = list->next;
		}
		if (flag) printf("Ошибка! Запись с таким номером не найдена\n");
		else printf(">Запись отредактирована\n");
		while (scanf("%*c")); //очистка буфера
	}
	else printf("Ошибка! Список пуст\n");
	printCenter("Нажмите любую клавишу для продолжения > ");
	_getch();
}

//запрос по цене и возрасту
void age(List *list) {
	List *tmp = list;
	Data data;
	bool flag = false;
	char age[6];
	int  prise;
	int  n = 1;

	system("cls");
	printCenter("*^.^* Просмотр игрушек дешевле заданной стоимости для заданного возраста *^.^*\n\n");
	printf("Введите возраст (хх-хх): ");
	scanf("%s", &age);
	printf("Введите цену:            ");
	scanf("%d", &prise);
	while (list != NULL) {
		data = list->data;
		if (!strcmp(data.age, age) && data.price <= prise) flag = true;
		list = list->next;
	}
	if (!flag) printf("Игрушки соответствующие введенным параметрам не найдены!\n\n");
	else {
		list = tmp;
		printf("\n");
		printHead();
		while (list != NULL) {
			data = list->data;
			if (!strcmp(data.age, age) && data.price <= prise)
				printNote(data, n++);
			list = list->next;
		}
		printLine();
		printf("\n");
	}
	printCenter("Нажмите любую кнопку для продолжения > ");
	_getch();
}

//запрс самого дорогого конструктора
void kit(List *list) {
	List *tmp = list;
	Data data;
	bool flag = false;
	int  prise = 0;
	int  n = 1;

	system("cls");
	printCenter("*^.^* Просмотр самого дорогого конструктора *^.^*\n\n");
	while (list != NULL) {
		data = list->data;
		if (strstr(data.name, "Конструктор") != NULL && data.price > prise) {
			prise = data.price;
			flag = true;
		}
		list = list->next;
	}
	if (!flag) printf("В списке нет конструктора!\n\n");
	else {
		list = tmp;
		printHead();
		while (list != NULL) {
			data = list->data;
			if (strstr(data.name, "Конструктор") != NULL && data.price == prise)
				printNote(data, n++);
			list = list->next;
		}
		printLine();
		printf("\n");
	}
	printCenter("Нажмите любую кнопку для продолжения > ");
	_getch();
}

//выход из программы
void exit(char *fname, List *list) {
	system("cls");
	printCenter("*^.^* Выход *^.^*\n\n");
	printf("Сохранить данные в %s перед выходом? (y/n) ", fname);
	if (_getche() == 'y') fileWrite(fname, list);
	clear(list); //очистка памяти
}