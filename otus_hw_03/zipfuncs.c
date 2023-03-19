/*	
	реализация функций zipdive.h
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "zipdive.h"


//*************************************************************************************************
/* определяет является ли файл jpg
*/
int isJPG(FILE * file, int view_info)
{
    char signature[2] = {0, 0};
    short welded = 0;
    int result = 0;
 
    signature[0] = (char)fgetc(file);
    signature[1] = (char)fgetc(file);
    welded = *(short *)(signature);
    result = welded == (short)JPG_START_SIGN ? 1 : 0;
 
    if(view_info)
    {
        if(result)
            printf
            ("Start signature of file matches *.jpg: %hx\n", welded);
        else
            printf
            ("Start signature of file doesn't matches *.jpeg: %hx\n", welded);
    }
 
    // перед выходом из файла обязательно сбросить положение считываемого байта
    // что даёт возможность в дальнейшем совершать обход по файлу сначала
    fseek(file, 0, 0);
    return result;
}
 
 
 
//*************************************************************************************************
/*  возвращает позицию последнего байта завершающей сигнатуры jpg (до EOF)
    0 если сигнатура не была найдена
*/
long long unsigned jpgEndPosition(FILE * file)
{
    // поле для записи текущего байта файла
    int current_byte = 0;
 
    // счётчик байтов от начала файла при поиске сигнатуры
    long long unsigned byte_counter = 0;
 
    // последний байт найденной сигнатуры
    long long unsigned end_sign_byte = 0;
 
    // массив для записи байтов
    unsigned char sign_array[2] = {0x00, 0x00};
 
    // поле для "склейки" байтов в двухбайтное значение
    // для сравнения с JPG_END_SIGN
    short result_sign = 0x0000;
 
    while(1)
    {
        current_byte = fgetc(file);
        byte_counter++;
        if(current_byte == EOF)
        {
            end_sign_byte = 0;
            break;
        }
 
        // на самом деле здесь можно было бы просто проверить следующий байт
        // хотя по количеству действий будет одинково
        if(current_byte == 0xFF)
        {
            sign_array[0] = (unsigned char)current_byte;
            current_byte = fgetc(file);
            byte_counter++;
            if(current_byte == EOF)
            {
                end_sign_byte = 0;
                break;
            }
            sign_array[1] = (unsigned char)current_byte;
            result_sign = *(short *)(sign_array);
            
            if(result_sign == (short)JPG_END_SIGN) //приведение - переменная int
            {
                end_sign_byte = byte_counter - 1;
                break;
            }
        }
    }
    fseek(file, 0, 0);
    return end_sign_byte;
}
 
 
 
//*************************************************************************************************
/* подсчитывает количество байт в файле
*/
long long unsigned get_file_bytes_value(FILE * file, int view_info)
{
    long long unsigned int byte_counter = 0;
    int current_byte = 0;
    while(1)
    {
        current_byte = fgetc(file);
        if(current_byte == EOF)
            break;
        byte_counter++;
    }
 
    // перед выходом из файла обязательно сбросить положение считываемого байта
    // что даёт возможность в дальнейшем совершать обход по файлу сначала
    fseek(file,0,0);
 
    if(view_info)
    {
 
    }
    return byte_counter;
}
 
 
 
//*************************************************************************************************
/* Определяет содержит ли файл сигнатуру завершения zip-файла
*/
int hasZIPinside(FILE * file, int view_info)
{
    // поле для записи текущего байта файла
    int current_byte = 0;
 
    // счётчик байтов от начала файла при поиске сигнатуры
    long long unsigned byte_counter = 0;
 
    // массив для записи байтов
    unsigned char sign_array[4] = {0x00, 0x00, 0x00, 0x00};    
 
    // поле для "склейки" байтов в четырёхбайтное значение
    // для сравнения с ZIP_END_SIGN
    int result_sign = 0x00000000;    
 
    // значение 1 если в файле найдена сигнатура конца zip, 0 если иначе
    int file_has_zip_end = 0;
 
    printf("------------------------------------------------------------\n");
    printf("\nSearch zip-end signature...\n");
 
    while(1)
    {        
        current_byte = fgetc(file);
        byte_counter++;
        if(current_byte == EOF)
            break;
 
        // когда значение прочитанного байта равно первому байту
        // сигнатуры, инициируется чтение следующих трёх байт и склейка
        // их вместе с первым в одно значение для сравнения
        if(current_byte == 0x50)
        {            
            sign_array[0] = (unsigned char)current_byte;
            sign_array[1] = (unsigned char)fgetc(file);
            sign_array[2] = (unsigned char)fgetc(file);
            sign_array[3] = (unsigned char)fgetc(file);

            result_sign = *(int *)sign_array;

            byte_counter += 3;

            if(result_sign == ZIP_EOCD_SIGN)
            {
                printf("\n---> Signature %#.8x reached <---\n",result_sign);
                file_has_zip_end = 1;
                break;
            }
        }
    }
 
    // вывод встроенного сообщения
    if(view_info)
    {
        if(file_has_zip_end)
        {
            printf("File has zip-end signature: %#.8x\n", ZIP_EOCD_SIGN);
        }
        else
        {
            printf("File hasn't zip-end signature. Checked bytes: %llu\n", byte_counter - 1);
        }
    }
 
    fseek(file,0,0);
    return file_has_zip_end;
}


//*************************************************************************************************
/* Определяет количество заархивированных файлов
*/
unsigned int count_local_file_header(FILE * file)
{
	unsigned int signatureCounter = 0;
	int currentByte = 0;
	unsigned char signatureBytes[4] = { 0, 0, 0, 0 };
	int signature = 0;

	if(ftell(file) != 0) 
		fseek(file, 0, 0);

	while(1)
	{
		currentByte = fgetc(file);
		if(currentByte == EOF)
			break;

		if(currentByte == 0x50)
		{		
			signatureBytes[0] = (unsigned char)currentByte;
			signatureBytes[1] = (unsigned char)fgetc(file);
			signatureBytes[2] = (unsigned char)fgetc(file);
			signatureBytes[3] = (unsigned char)fgetc(file);
			
			signature = *(int *)(signatureBytes);
			
			if(signature == ZIP_LFH_SIGN)
			{
				signatureCounter++;				
			}
		}
	}
	return signatureCounter;
}



//*************************************************************************************************
/* Заполняет массив указателей на структуру LocalFileHeader
*/
void define_LocalFileHeader_array(
	FILE * file, 
	LocalFileHeader * lfhArray[],
	unsigned int arrayLength)
{
	long int position = 0;
	int currentByte = 0;
	unsigned char signatureBytes[4] = { 0, 0, 0, 0 };
	int signature = 0;
	unsigned int index = 0;

	if(ftell(file) != 0) 
		fseek(file, 0, 0);

	while(1)
	{
		currentByte = fgetc(file);
		if(currentByte == EOF)
			break;

		if(currentByte == 0x50)
		{
			position = ftell(file) - 1;			
			signatureBytes[0] = (unsigned char)currentByte;
			signatureBytes[1] = (unsigned char)fgetc(file);
			signatureBytes[2] = (unsigned char)fgetc(file);
			signatureBytes[3] = (unsigned char)fgetc(file);
			
			signature = *(int *)(signatureBytes);
			
			if(signature == ZIP_LFH_SIGN)
			{
				LocalFileHeader * lfh = 
					(LocalFileHeader *)malloc(sizeof(LocalFileHeader));
				parse_LocalFileHeader(file, lfh, position);
				lfhArray[index] = lfh;
				index++;
				if(index == arrayLength)
					break;
			}
		}
	}
}



//*************************************************************************************************
/* Парсинг данных из файла в структуру LocalFileHeader
*/
void parse_LocalFileHeader(FILE * file, LocalFileHeader * lfh, int signaturePos)
{
    unsigned char bytesArray[46] = {0};
    long long unsigned postPosition = signaturePos;
    fseek(file, signaturePos, SEEK_SET);
    fread(bytesArray, sizeof(unsigned char), sizeof(bytesArray), file);
 
    int position = 0;
 
    fill_struct_fields
        (bytesArray, &position, &lfh->signature, sizeof(lfh->signature));   
    fill_struct_fields
        (bytesArray, &position, &lfh->versionToExtract, sizeof(lfh->versionToExtract));
    fill_struct_fields
        (bytesArray, &position, &lfh->generalPurposeBitFlag, sizeof(lfh->generalPurposeBitFlag));
    fill_struct_fields
        (bytesArray, &position, &lfh->compressionMethod, sizeof(lfh->compressionMethod));
    fill_struct_fields
        (bytesArray, &position, &lfh->modificationTime, sizeof(lfh->modificationTime));
    fill_struct_fields
        (bytesArray, &position, &lfh->modificationDate, sizeof(lfh->modificationDate));
    fill_struct_fields
        (bytesArray, &position, &lfh->crc32, sizeof(lfh->crc32));
    fill_struct_fields
        (bytesArray, &position, &lfh->compressedSize, sizeof(lfh->compressedSize));
    fill_struct_fields
        (bytesArray, &position, &lfh->uncompressedSize, sizeof(lfh->uncompressedSize));
    fill_struct_fields
        (bytesArray, &position, &lfh->filenameLength, sizeof(lfh->filenameLength));
    fill_struct_fields
        (bytesArray, &position, &lfh->extraFieldLength, sizeof(lfh->extraFieldLength));
    
    /*
    // указатели надо будет создать, их как таковых, в файле быть не может
    fill_struct_fields
        (bytesArray, &position, &lfh->filename, sizeof(lfh->filename));
    fill_struct_fields
        (bytesArray, &position, &lfh->extraField, sizeof(lfh->extraField)); 
    */
 
    // надо ли отдельно потом освобождать память от этих указателей? 
    uint8_t * currentFilename = (uint8_t *)malloc(lfh->filenameLength);
    uint8_t * currentExtraField = (uint8_t *)malloc(lfh->extraFieldLength);

    postPosition += position;

    fseek(file, postPosition, SEEK_SET);

    for(int i = 0; i < lfh->filenameLength; i++)
    {
    	currentFilename[i] = fgetc(file);
    }

    for(int i = 0; i < lfh->extraFieldLength; i++)
    {
    	currentExtraField[i] = fgetc(file);
    }

    lfh->filename = currentFilename;
    lfh->extraField = currentExtraField;
}


//*************************************************************************************************
/* Считывает байты в поле структуры
*/
void fill_struct_fields
    (unsigned char * bytesArray, int * position, void * startFieldPointer, int fieldSize)
    {
        unsigned char * bytePointer = (unsigned char *)startFieldPointer;
        for(int i = 0; i < fieldSize; i++)
        {
            *(bytePointer + i) = bytesArray[i + *position];
        }
        *position += fieldSize;
    }