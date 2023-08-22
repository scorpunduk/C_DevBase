/*
	программа для перевода текста из нестандартных кодировок в UTF-8
	принимает 2 аргумента:
	- текстовый файл в одной из нестандартных кодировок
	- имя текстового файла, куда следует записать результат перевода
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "unicodeparser.h"

// вызов интерфейса для выбора опций.
// так как функция может вызываться рекурсивно я решил, что переменные лучше вынести
void input_choice();

// указатель на функцию для обработки символов
// значение присваивается в функции input_choice
uint32_t(*parse_func)(char);

//  в эту переменную записывается выбор 
char encoding_input = 0;

// строка меню с выбором опций
char input_string[] = "Please, input source encoding:\n"
		"1 - cp1251\n"
		"2 - koi-8\n"
		"3 - iso-8859-5\n"
		"q - exit\n"
		">> ";

//  строка сообщения о неправильном выборе
char input_undefined[] = "\n...input undefined, try again...\n";

//  флаг, определяющий выбор опции выхода из меню
char input_quit = 0;


//	---------------------------------------------------------------------------

int main(int argc, char * argv[])
{
	setlocale (LC_ALL, "en_US.UTF-8");

	FILE * textfile;
	FILE * outputfile;
	int current_byte = 0;
	unsigned char byte_for_write = 0;
	uint32_t unicode_symbol = 0;
	char utf_code[5] = {0};

	if(argc != 3)
	{
		printf("Argumetns exception: program takes 2 argument:\n"
			"- existing textfile\n"
			"- output file name\n");
		exit(EXIT_FAILURE);
	}

	if((textfile = fopen(argv[1], "r")) == NULL)
	{
		printf("Can`t open file %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	int name_length = strlen(argv[2]);

	// инициализировать массив, длина котрого задана переменной,
	// почему-то нельзя
	char output_filename[name_length + 5];

	// если не выставить терминальный ноль, будет ошибка
	output_filename[0] = '\0';
	
	char output_res[] = ".txt";	
	strncat(output_filename, argv[2], name_length);	
	strncat(output_filename, output_res, 4);
	

	if((outputfile = fopen(output_filename, "w+b")) == NULL)
	{
		fprintf(stdout, "Can`t open or create file %s\n", argv[2]);
		fclose(textfile);
		exit(EXIT_FAILURE);
	}

	input_choice();

	if(input_quit)
	{
		printf("\nProgramm was interrupted\n");
		return 0;
	}

	// вывод перекодированного текста в указанный файл
	while(1)
	{		
		current_byte = fgetc(textfile);
		if(current_byte == EOF)		
			break;

		if(current_byte <= 0x7F)
		{			
			fwrite(&current_byte, sizeof(char), 1, outputfile);
			continue;
		}

		unicode_symbol = parse_func(current_byte);	
		set_utf_code(unicode_symbol, utf_code);
		
		for(int i = 0; i < 5; i++)
		{
			if(utf_code[i] == '\0') break;
			fwrite(&utf_code[i], sizeof(char), 1, outputfile);
		}
	}
	
	fseek(textfile, 0, SEEK_SET);

	// вывод перекодированного текста в консоль
	while(1)
	{		
		current_byte = fgetc(textfile);
		if(current_byte == EOF)
			break;

		if(current_byte <= 0x7F)
		{			
			printf("%c", current_byte);
			continue;
		}

		unicode_symbol = parse_func(current_byte);	
		set_utf_code(unicode_symbol, utf_code);		
		printf("%s", utf_code);		
	}

	fclose(textfile);
	fclose(outputfile);
	
	return 0;
}

//	---------------------------------------------------------------------------

// все переменные вынесены из функции, так как возможен рекурсивный вызов
void input_choice()
{
	char encoding_input = 0;
	printf("%s", input_string);
	encoding_input = getchar();
	if(encoding_input != '\n')
	getchar(); // сброс нажатия Enter
	switch(encoding_input)
	{
	case '1':
		{
			parse_func = cp1251_to_unicode;
			break;			
		}
	case '2':
		{
			parse_func = koi_to_unicode;
			break;
		}
	case '3':
		{
			parse_func = iso8895_5_to_unicode;
			break;
		}
	case 'q':
		{		
			input_quit = 1;
			break;
		}
	default:
		{
			printf("%s", input_undefined);
			input_choice();
		}
	}
}