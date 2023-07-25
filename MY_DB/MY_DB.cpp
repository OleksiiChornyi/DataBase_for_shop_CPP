/**
* @file DataBase.c
* @autor Чёрный А.А., гр. 515А, вариант 17
* @date 16 мая 2019
* @brief Расчётная работа
*
* База данных товаров
*/
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<locale.h>
#include<stdbool.h>
#include<string.h>
#include<windows.h>
#include<time.h>
#include <iostream>
#define config 30

typedef struct {
	unsigned int code;
	char name[config];
	float price;
	unsigned int quantity;
	unsigned int ordinal;
	bool deleted;
} products;

// Function prototypes
void plus(FILE*); //Добавление
void del(FILE*); //Удаление
void watch(FILE*); //Просмотр
void changes(FILE*); //Изменение
void recovery(FILE*); //Восстановление
void save(FILE*); //Сохранение
void exp(FILE*); //Експорт
void print1(FILE*); //Вывод к-ва товара по названию или коду
void print2(FILE*); //Вывод всех товаров, к-во которых == 1, а цена - выше заданной

int main(int argc, char* argv[])
{
	srand(time(NULL));
	setlocale(LC_ALL, "rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	FILE* f1, * base;
	int i, what;
	char* name_config = (char*)calloc(100, sizeof(char));
	char* key_correct = (char*)calloc(10, sizeof(char));
	const char sixteen[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	strcpy(name_config, argv[0]);
	for (;;)
	{
		if (!(strstr(name_config, "\\")))
			break;
		name_config = strstr(name_config, "\\");
		_strnset(name_config, ' ', sizeof(char));
	}
	int len = strlen(name_config);
	char* name_config1 = (char*)calloc(30, sizeof(char));
	strncpy(name_config1, name_config, len - 4);
	name_config = strcat(name_config1, ".cfg");
	if (!(f1 = (FILE*)fopen(name_config, "r")))
	{
		if (!(f1 = (FILE*)fopen(name_config, "wb+")))
			printf("Error\n");
		int j = 0;
		while (j < 10)
		{
			*(key_correct + j) = sixteen[rand() % 16];
			j++;
		}
		key_correct[10] = '\0';
		fprintf(f1, "DB = new_base.dat;\nKEY = %s", key_correct);
	}
	char* DB = (char*)calloc(config, sizeof(char));
	char* k = (char*)calloc(config, sizeof(char));
	fseek(f1, +5, SEEK_SET);
	if (!(fgets(k, config, f1))) {
		printf("Не удаётся считать файл\n");
		return 1;
	}
	for (i = 0; k[i] != ';'; i++)
		DB[i] = k[i];
	if (!(base = (FILE*)fopen(DB, "r+b")))
	{
		base = (FILE*)fopen(DB, "w+b");
		printf("Создан новый документ базы данных\n\n");
	}
	free(DB);
	free(k);
	fseek(f1, +6, SEEK_CUR);
	if (!(fgets(k, config, f1))) {
		printf("Не удаётся считать файл\n\n");
		return 1;
	}
	char* person_key = (char*)calloc(config, sizeof(char));

	int attempt = 0;
	bool valid_key = false;

	while (attempt < 3) {
		std::cout << k;
		printf("Введите лицензионный ключ: ");
		scanf("%s", person_key);

		if (strcmp(k, person_key) == 0) {
			printf("Лицензионный ключ верный!\n\n");
			valid_key = true;
			break;
		}
		else {
			attempt++;
			printf("Лицензионный ключ НЕ верный!\n");

			if (attempt < 3) {
				int answer;
				printf("Хотите продолжить?\n1 - ДА, при продолжении будет доступны не все функции программы\n"
					"2 - НЕТ, попробовать ввести ключ ещё раз\n");
				scanf("%d", &answer);
				if (answer == 1)
					break;
				else if (answer == 2)
					continue;
			}
			else {
				printf("Превышено количество попыток ввода ключа. Программа завершена.\n");
				return 0;
			}
		}
	}

	if (!valid_key) {
		printf("Превышено количество попыток ввода ключа. Программа завершена.\n");
		return 0;
	}

	for (;;) {
		fseek(base, 0, SEEK_SET);
		printf("Меню: \n"
			"Добавление записи - 1\n"
			"Удаление записи - 2\n"
			"Просмотр записи - 3\n"
			"Изменение записи - 4\n"
			"Восстановление удалённых записей - 5\n"
			"Сохранение базы данных - 6\n"
			"Экспорт базы данных в текстовый файл - 7\n"
			"Вывод количества товара по названию или коду - 8\n"
			"Вывод всех товаров, количество которых равно 1, а цена - выше заданной - 9\n"
			"Выйти - 0\n\n\n");
		printf("Введите действие: ");
		scanf("%d", &what);
		switch (what) {
		case 1:
			plus(base);
			break;
		case 2:
			del(base);
			break;
		case 3:
			watch(base);
			break;
		case 4:
			changes(base);
			break;
		case 5:
			recovery(base);
			break;
		case 6:
			save(base);
			break;
		case 7:
			exp(base);
			break;
		case 8:
			print1(base);
			break;
		case 9:
			print2(base);
			break;
		case 0:
			return 0;
		default:
			printf("Неизвестная операция!\nПопробуйте ещё раз!\n\n");
			break;
		}
	}

	return 0;
}

void plus(FILE*

	base)
{
	products product1;
	int i = 1, p = 0, j = 0;
	fseek(base, 0, SEEK_SET);
	for (;;) {
		if (!(fread(&product1, sizeof(product1), 1, base)))
			break;
		product1.ordinal = i;
		i++;
		if (product1.deleted) {
			int line = sizeof(product1);
			fseek(base, -line, SEEK_CUR);
			i--;
			break;
		}
	}
	printf("\n\nВведите код товара: ");
	scanf("%d", &product1.code);
	for (;;) {
		products product2;
		fseek(base, sizeof(product2) * p, SEEK_SET);
		if (!(fread(&product2, sizeof(product2), 1, base)))
			break;
		p++;
		if (product1.code == product2.code) {
			printf("\n\nТовар с данным кодом уже существует!\n"
				"Возможно он удалён с подальшей возможностью восстановления!\n"
				"Если вы хотите изменить значения товара с данным кодом - выберете пункт Измеинение записи!\n"
				"Иначе напишите другой код товара!\n\n\n");
			return;
		}
	}
	printf("Введите название товара: ");
	scanf("%s", &product1.name);
	printf("Введите цену товара: ");
	scanf("%f", &product1.price);
	printf("Введите количество товара: ");
	scanf("%d", &product1.quantity);
	product1.deleted = 0;
	product1.ordinal = i;
	fseek(base, sizeof(product1) * (product1.ordinal - 1), SEEK_SET);
	fwrite(&product1, sizeof(product1), 1, base);
	printf("Товар добавлен успешно!\n");
	fprintf(base, "\n");
	system("pause");
	printf("\n\n\n");
	for (;;) {
		fseek(base, sizeof(product1) * i, SEEK_SET);
		if (!(fread(&product1, sizeof(product1), 1, base)))
			return;
		if (product1.deleted == 0) {
			product1.ordinal++;
			fseek(base, sizeof(product1) * i, SEEK_SET);
			fwrite(&product1, sizeof(product1), 1, base);
		}
		i++;
	}
}

void del(FILE* base)
{
	products product1;
	int code_person, i = 0;
	printf("\nВведите код товара, который хотите удалить: ");
	scanf("%d", &code_person);
	for (;;) {
		fseek(base, sizeof(product1) * i, SEEK_SET);
		if (!(fread(&product1, sizeof(product1), 1, base))) {
			printf("Данного товара не обнаружено!\n");
			system("pause");
			printf("\n\n\n");
			return;
		}
		fseek(base, sizeof(product1) * (product1.ordinal - 1), SEEK_SET);
		if (product1.code == code_person) {
			if (product1.deleted == 1) {
				printf("Данный товар уже удалён!\n");
				system("pause");
				printf("\n\n\n");
				return;
			}
			if (product1.deleted == 0) {
				product1.deleted = 1;
				fseek(base, sizeof(product1) * (product1.ordinal - 1), SEEK_SET);
				fwrite(&product1, sizeof(product1), 1, base);
				printf("Удаление успешно!\n");
				system("pause");
				printf("\n\n\n");
				break;
			}
		}
		i++;
	}
	for (;;) {
		fseek(base, sizeof(product1) * i, SEEK_SET);
		if (!(fread(&product1, sizeof(product1), 1, base)))
			return;
		if (product1.deleted == 0) {
			product1.ordinal--;
			fseek(base, sizeof(product1) * (product1.ordinal), SEEK_SET);
			fwrite(&product1, sizeof(product1), 1, base);
		}
		i++;
	}
	return;
}

void watch(FILE* base)
{
	products product1;
	int k = 0, i = 0;
	fseek(base, 0, SEEK_SET);
	for (;;) {
		fseek(base, sizeof(product1) * i, SEEK_SET);
		if (!(fread(&product1, sizeof(product1), 1, base))) {
			if (k == 0)
				printf("\nБаза данных пуста\n");
			system("pause");
			printf("\n\n\n");
			return;
		}
		if (!product1.deleted) {
			if (k == 0) {
				printf("______________________________________________________________________________________________\n");
				printf("\n|№  | Код товара | Название товара | Цена товара | Количество товара | \n");
				printf("______________________________________________________________________________________________\n");
				k = 1;
			}
			printf("|%-3d|", product1.ordinal);
			printf("%-12d|", product1.code);
			printf("%-17s|", product1.name);
			printf("%-13f|", product1.price);
			printf("%-19d|\n", product1.quantity);
			printf("______________________________________________________________________________________________\n");
		}
		i++;
	}
}

void changes(FILE* base)
{
	products product1;
	int code_person, i = 0;
	printf("Введите код товара, который хотите изменить: ");
	scanf("%d", &code_person);
	for (;;) {
		fseek(base, sizeof(product1) * i, SEEK_SET);
		if (!(fread(&product1, sizeof(product1), 1, base)))
			break;
		i++;
		if (product1.code == code_person && product1.deleted == 0) {
			int d;
			printf("Что вы хотите изменить?\n"
				"Код товара - 1\n"
				"Название товара - 2\n"
				"Цену товара - 3\n"
				"Количество товара - 4\n"
				"Выход - 0\n\n"
				"Введитедействие: ");
			scanf("%d", &d);
			switch (d) {
			case 1:
				printf("Введите новый код товара: ");
				scanf("%d", &product1.code);
				break;
			case 2:
				printf("Введите новое название товара: ");
				scanf("%s", &product1.name);
				break;
			case 3:
				printf("Введите новую цену товара: ");
				scanf("%f", &product1.price);
				break;
			case 4:
				printf("Введите текущее количество товара: ");
				scanf("%d", &product1.quantity);
				break;
			case 0:
				return;
			default:
				printf("Неизвестная операция\nПопробуйте ещё раз\n\n");
				break;


			}
			fseek(base, sizeof(product1) * (i - 1), SEEK_SET);
			fwrite(&product1, sizeof(product1), 1, base);
			code_person = product1.code;
			printf("\n\n\n");
			i = 0;
		}
	}
	printf("Данного товара не существует либо он удалён\n\n\n");
	return;
}

void recovery(FILE* base)
{
	products product1;
	int code_person, i = 0;
	printf("Введите код товара, который хотите восстановить: ");
	scanf("%d", &code_person);
	for (;;) {
		fseek(base, sizeof(product1) * i, SEEK_SET);
		if (!(fread(&product1, sizeof(product1), 1, base)))
			break;
		i++;
		if (product1.code == code_person && product1.deleted == 1) {
			product1.deleted = 0;
			product1.ordinal = i;
			fseek(base, sizeof(product1) * (i - 1), SEEK_SET);
			fwrite(&product1, sizeof(product1), 1, base);
			printf("Товар успешно восстановлен!\n");
			for (;;) {
				fseek(base, sizeof(product1) * i, SEEK_SET);
				if (!(fread(&product1, sizeof(product1), 1, base)))
					break;
				if (product1.deleted == 0) {
					product1.ordinal++;
					fseek(base, sizeof(product1) * i, SEEK_SET);
					fwrite(&product1, sizeof(product1), 1, base);
				}
				i++;
			}
			system("pause");
			printf("\n\n\n");
			return;
		}
	}
	printf("Данного товара не существует либо он НЕ удалён\n\n\n");
	return;
}

void save(FILE* base)
{
	FILE* f2;
	products product1;
	char* file = (char*)calloc(config, sizeof(char));
	int k = 0, j = 0;
	printf("Введите название документа, в который вы хотите сохранить базу данных: \n");
	scanf("%s", file);
	strcat(file, ".dat");
	if (!(f2 = fopen(file, "wb+")))
		printf("С открытием файла произошла ошибка\n\n");
	for (;;) {
		fseek(base, sizeof(product1) * j, SEEK_SET);
		if (!(fread(&product1, sizeof(product1), 1, base))) {
			if (k == 0)
				fprintf(f2, "База данных пуста\n");
			else
				printf("\n\nСохранение базы данных в файл выполнено успешно!\n");
			system("pause");
			fclose(f2);
			free(file);
			printf("\n\n\n");
			return;
		}
		else if (!product1.deleted) {
			fwrite(&product1, sizeof(product1), 1, f2);
			k = 1;
		}
		j++;
	}
}

void exp(FILE* base)
{
	FILE* f2;
	products product1;
	char* file = (char*)calloc(config, sizeof(char));
	int k = 0, i = 1, j = 0;
	printf("Введите название документа, в который вы хотите сохранить базу данных: \n");
	scanf("%s", file);
	strcat(file, ".txt");
	if (!(f2 = fopen(file, "wb")))
		printf("С открытием файла произошла ошибка\n\n");
	for (;;) {
		fseek(base, sizeof(product1) * j, SEEK_SET);
		if (!(fread(&product1, sizeof(product1), 1, base))) {
			if (k == 0)
				fprintf(f2, "База данных пуста\n");
			else
				printf("\n\nСохранение базы данных в файл выполнено успешно!\n");
			system("pause");
			printf("\n\n\n\n\n\n");
			fclose(f2);
			free(file);
			return;
		}
		else if (!product1.deleted) {
			if (k == 0) {
				fprintf(f2, "Код товара ; Название товара ; Цена товара ; Количество товара; \n");
				k = 1;
			}
			fprintf(f2, "%-11d; ", product1.code);
			fprintf(f2, "%-16s; ", product1.name);
			fprintf(f2, "%-12f; ", product1.price);
			fprintf(f2, "%-17d;\n", product1.quantity);
			i++;
		}
		j++;
	}
}

void print1(FILE* base)
{
	products product1;
	char* name_person = (char*)calloc(config, sizeof(char));
	int how;
print:
	printf("Вы хотите увидеть информацию о товаре по его коду или имени?\n"
		"По коду - 1\nПо имени - 2\n\n");
	scanf("%d", &how);
	switch (how) {
	case 1:
		unsigned int code_person;
		printf("Введите код товара: ");
		scanf("%d", &code_person);
		for (;;) {
			if (!(fread(&product1, sizeof(product1), 1, base))) {
				printf("Данного товара нет в наличии\n");
				system("pause");
				printf("\n\n\n");
				return;
			}
			if (code_person == product1.code && product1.deleted == 0) {
				printf("Товара %s с кодом %d в наличии: %d\n", product1.name,
					product1.code, product1.quantity);
				system("pause");
				printf("\n\n\n");
				return;
			}
		}
	case 2:
		printf("Введите имя товара: ");
		scanf("%s", name_person);
		for (;;) {
			if (!(fread(&product1, sizeof(product1), 1, base))) {
				printf("Данного товара нет в наличии\n");
				system("pause");
				printf("\n\n\n");
				return;
			}
			if (!strcmp(product1.name, name_person) && product1.deleted == 0) {
				printf("Товара %s с кодом %d в наличии: %d\n", product1.name,
					product1.code, product1.quantity);
				system("pause");
				printf("\n\n\n");
				return;
			}
		}
	default:
		printf("\nНеизвестная операция!\nПопробуйте ещё раз!\n\n");
		fseek(base, 0, SEEK_SET);
		goto print;
	}
	return;
}

void print2(FILE* base)
{
	products product1;
	float price_person;
	int i = 0;
	printf("Далее будут показаны товары, количество которых = 1, а цена - выше заданной вами\n");
	printf("Введите цену: ");
	scanf("%f", &price_person);
	for (;;) {
		fseek(base, sizeof(product1) * i, SEEK_SET);
		if (!(fread(&product1, sizeof(product1), 1, base))) {
			printf("Товары отсутствуют\n");
			system("pause");
			printf("\n\n\n");
			return;
		}
		else if (product1.price > price_person && product1.deleted == 0 && product1.quantity == 1) {
			printf("Товар: %s\nКод товара: %d\n\n", product1.name, product1.code);
		}
		i++;
	}
}