/*
	программа переводит текст из заданной кодировки в UTF-8
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>

int main()
{
	/*
	setlocale (LC_ALL, "ru_RU.UTF-8");
	setlocale (LC_NUMERIC, "C");
	*/
	char b[3] = {0x04, 0x10, '\0'};
	printf("%s\n", b);
	//printf("%d\n", 0x0410);
	//printf("%x\n", 'А');

	//setlocale (LC_ALL, "ru_RU.UTF-8");
	setlocale (LC_ALL, "en_US.UTF-8");
	char string [] = { 0xD0, 0x90, 0x20, 0xD0, 0x91,'\0'};
	printf("%s\n", string);

	printf("Мир, \n");
	//printf("\U000000D0\U00000090");

	/*
	int number = 0x0410;
	printf("\n%", '_');
	printf("Пока, мир\n");
	printf("\04\10\n");

	printf("%x", 53392);

	wchar_t character = 'А';
	wprintf("%c", character);
	printf("\U00000410");
	*/


	return 0;
}