/*
	программа для проверки работы функций из "модуля" unicodeparser
	а также возможностей передачи функции через указатель
	https://metanit.com/c/tutorial/5.11.php
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "unicodeparser.h"

int main()
{
	setlocale (LC_ALL, "en_US.UTF-8");

	// объявление указателя на функцию
	uint32_t (*parse_func)(char);

	char cp_ = 0xC0 + 63;
	char iso = 0xB0;
	char koi = 0xFD;

	char cp__addition = 0xA8 + 0x10;
	char iso_addition = 0xA1 + 0x50;
	char koi_addition = 0xA3 + 0x10;

	uint32_t unicode_symbol = 0;

	char utf_code[5] = {0};

	// присвоение функции указателю на функцию
	parse_func = cp1251_to_unicode;
	// использование указателя на функцию
	unicode_symbol = parse_func(cp_);	
	set_utf_code(unicode_symbol, utf_code);
	printf("%s\n", utf_code);

	unicode_symbol = iso8895_5_to_unicode(iso);
	set_utf_code(unicode_symbol, utf_code);
	printf("%s\n", utf_code);

	unicode_symbol = koi_to_unicode(koi);
	set_utf_code(unicode_symbol, utf_code);
	printf("%s\n", utf_code);


	unicode_symbol = parse_func(cp__addition);
	set_utf_code(unicode_symbol, utf_code);
	printf("%s\n", utf_code);

	unicode_symbol = iso8895_5_to_unicode(iso_addition);
	set_utf_code(unicode_symbol, utf_code);
	printf("%s\n", utf_code);

	unicode_symbol = koi_to_unicode(koi_addition);
	set_utf_code(unicode_symbol, utf_code);
	printf("%s\n", utf_code);

}