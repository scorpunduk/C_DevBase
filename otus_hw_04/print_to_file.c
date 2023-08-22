/*
	пример вывода в текстовый файл в кодировке UTF-8
	на выходе текстовый файл output.txt с содержанием АБВ
*/

#include <stdio.h>
#include <stdlib.h>
//#include <string.h>

int main()
{
	FILE * output;

	if((output = fopen("output.txt", "w+b")) == NULL)
	{
		fprintf(stdout, "Can`t open file\n");
	}

	unsigned char symbol_first = 0xD0;
	unsigned char symbol_second = 0x90;

	// указатель
	fwrite(&symbol_first, sizeof(char), 1, output);
	fwrite(&symbol_second, sizeof(char), 1, output);

	symbol_second++;

	fwrite(&symbol_first, sizeof(char), 1, output);
	fwrite(&symbol_second, sizeof(char), 1, output);

	symbol_second++;

	fwrite(&symbol_first, sizeof(char), 1, output);
	fwrite(&symbol_second, sizeof(char), 1, output);

	fclose(output);	

	return 0;
}