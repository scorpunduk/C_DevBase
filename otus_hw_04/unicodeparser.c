/*
	определение функций из unicodeparser.h
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "unicodeparser.h"


// 	-------------------------------------------------------------------------------------
int count_octet_number(uint32_t unicode_value)
{
	// это никогда не будет меняться
	// int size = sizeof(octet_cap)/sizeof(octet_cap[0]);
	for(int i = 0; i < 4; i++)	
		if(unicode_value < octet_cap[i])
			return i+1;
		
	return 0;
}



// 	-------------------------------------------------------------------------------------
void set_utf_code(uint32_t unicode_value, char * symbol_utf_code)
{
	// вычисление количества октетов
	int count = count_octet_number(unicode_value);

	// заполнение массива терминальными нулями
	for(int i = 0; i < 5; i++)
		symbol_utf_code[i] = '\0';
	
	// если код utf-8 однобайтовый, символ соответствует ASCII - номер по юникоду от 0 до 127,
	// никаких преобразований не нужно
	if(count == 1)
	{		
		symbol_utf_code[0] = (char)unicode_value;
		return;
	}
	
	// перетаскивание битов по шаблону
	for(int i = 1; i<=count; i++)
	{
		symbol_utf_code[count-i] = unicode_value >> bitset_shifts[i-1];
		symbol_utf_code[count-i] = symbol_utf_code[count-i] & octet_clean[0];
		symbol_utf_code[count-i] = symbol_utf_code[count-i] | octet_template[0];
	}

	// все октеты заполнены корректно, остаётся допилить старший октет
	symbol_utf_code[0] =
		(symbol_utf_code[0] & octet_clean[count-1]) | octet_template[count-1];

	return;		
}



//	--------------------------------------------------------------------------------------
uint32_t koi_to_unicode(char symbol)
{
	uint32_t unicode_symbol = 0x3F;
	for(int i = 0 ; i < 64; i++)
		if(koi8codes[i] == (unsigned char)symbol)
		{
			unicode_symbol = 0x0410 + i;
			return unicode_symbol;
		}
	for(int i = 0; i < 2; i++)
		if(koi8codes_addition[i] == (unsigned char)symbol)
			unicode_symbol = unicode_addition[i];
	return unicode_symbol;
}



//	--------------------------------------------------------------------------------------
uint32_t cp1251_to_unicode(char symbol)
{
	uint32_t unicode_symbol = 0;
	for(int i = 0; i < 2; i++)
		if(cp1251_addition[i] == (unsigned char)symbol)
		{			
			unicode_symbol = unicode_addition[i];
			return unicode_symbol;
		}
	unicode_symbol = (unsigned char)symbol + cp1251_unicode_diff;
	return unicode_symbol;
}



//	--------------------------------------------------------------------------------------
uint32_t iso8895_5_to_unicode(char symbol)
{
	uint32_t unicode_symbol = 0;
	for(int i = 0; i < 2; i++)
		if(iso8895_5_addition[i] == (unsigned char)symbol)
		{
			unicode_symbol = unicode_addition[i];
			return unicode_symbol;
		}
	unicode_symbol = (unsigned char)symbol + iso8895_5_unicode_diff;
	return unicode_symbol;
}
