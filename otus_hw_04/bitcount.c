#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "unicodeparser.h"

int main()
{

	#if 0

	/* 	границы диапазонов числовых кодов символов по unicode
		по ним определяется количество байт в последовательности UTF-8
		для кодирования символа	
	*/
	uint32_t octet_cap_1 = 0x7F;
	uint32_t octet_cap_2 = 0x7FF;
	uint32_t octet_cap_3 = 0xFFFF;
	uint32_t octet_cap_4 = 0x10FFFF;

	uint32_t octet_cap[] = 
	{ 
		octet_cap_1,
		octet_cap_2,
		octet_cap_3,
		octet_cap_4
	};

	char size = sizeof(octet_cap)/sizeof(octet_cap[0]);

	for(int i = 0; i < size; i++)
		printf("%x\n", octet_cap[i]);	

	//uint32_t symbol_1251 = 0x410;
	uint32_t code = 0xF2F2;

	unsigned int another_symbol_1251 = 0x1FFFF;

	char bytes_in_utf_code = count_octet_number(code);

	
	printf("compare = %d\n", 0x0410 & 0x0080);

	printf("required bytes = %d\n", bytes_in_utf_code);


	uint32_t unicode = 0x10AC90;
	uint32_t unicode_bin = 	0b00000000000100001010110010010000;
	uint32_t mask_0 = 		0b00000000000000000000000000111111;

	char mask_standart = 0b00111111;
	char mask_2 = 0b00011111;
	char mask_3 = 0b00001111;
	char mask_4 = 0b00000111;

	printf("%d\n", unicode == unicode_bin);

	// 00000000000 100 001010 110010 010000;

	uint32_t byte_one = unicode_bin & mask_0;	
	uint32_t byte_two = (unicode_bin >> 6) & mask_0;	
	uint32_t byte_three = (unicode_bin >> 12) & mask_0;
	uint32_t byte_four = (unicode_bin >> 18) & mask_0;
	

	printf("1 byte shift result = %d (%d)\n", byte_one, 	(char)byte_one);
	printf("2 byte shift result = %d (%d)\n", byte_two, 	(char)byte_two);
	printf("3 byte shift result = %d (%d)\n", byte_three, 	(char)byte_three);
	printf("4 byte shift result = %d (%d)\n", byte_four, 	(char)byte_four);



	

	char first = (((char)code) & octet_clean[0]) | octet_template[0];

	char second = code >> bitset_shifts[1];
	second = second & octet_clean[1];
	second = second | octet_template[1];
	printf("%x %x\n", (unsigned char)second, (unsigned char)first);

	#endif

	uint32_t code = 0xF2F2;

	char utf_symbol[5];
	//uint32_t code = 0xF2F2;

	set_utf_code(code, utf_symbol);

	if(count_octet_number(code) == 2)
		printf("%x %x\n", (unsigned char)utf_symbol[0], (unsigned char)utf_symbol[1]);

	if(count_octet_number(code) == 3)
		printf("%x %x %x\n", (unsigned char)utf_symbol[0], (unsigned char)utf_symbol[1], (unsigned char)utf_symbol[2]);		

	return 0;
}

//https://ru.stackoverflow.com/questions/944051/%D0%92%D1%8B%D0%B2%D0%BE%D0%B4-%D0%B4%D0%B2%D0%BE%D0%B8%D1%87%D0%BD%D0%BE%D0%B3%D0%BE-%D1%87%D0%B8%D1%81%D0%BB%D0%B0-%D0%B2-%D0%A1%D0%98