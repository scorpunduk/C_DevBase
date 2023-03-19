/*
	программа определяет файл *.jpg и производит в нём поиск скрытых архивов
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "zipdive.h"

int main(int argc, char * argv[])
{
	FILE * zipfile;
	unsigned int archivedFilesCount = 0;
	int file_has_hidden_zip = 0;

    if(argc != 2){
        printf("Arguments exception: program takes 1 argument (filepath)\n");
        exit(EXIT_FAILURE);
    }

	if((zipfile = fopen(argv[1], "rb")) == NULL)
 	{
        printf("Can't open file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // если файл не является jpg
    if(!isJPG(zipfile, 1))
    {
    	printf("File is not jpg!");
        exit(EXIT_FAILURE);
    }
 
    printf("File volume: %llu bytes\n", get_file_bytes_value(zipfile,0));
 
    file_has_hidden_zip = hasZIPinside(zipfile, 1);

    if(!file_has_hidden_zip)
    {
    	printf("File hasn't hidden zip-archive inside\n");
    	return 0;
    }

    archivedFilesCount =
    	count_local_file_header(zipfile);
    
    // массив указателей на LocalFileHeader
    LocalFileHeader * lfhArray[archivedFilesCount];

    printf("Files in archieve: %u\n", archivedFilesCount);

    define_LocalFileHeader_array(
    	zipfile,
    	lfhArray,
    	archivedFilesCount);

    for(unsigned i = 0; i < archivedFilesCount; i++)
    {
    	printf("%s\n", lfhArray[i]->filename);
    }

    // освобождение памяти
    for(unsigned i = 0; i < archivedFilesCount; i++)
    {
        // следует ли освобождать память для отдельных полей структуры, таких как указатели на строки?
        // или достаточно только освободить саму структуру?
        // может ли быть такое, что указатели затрутся, а участок со строками останется занимать память?
        free(lfhArray[i]->filename);
        free(lfhArray[i]->extraField);
        free(lfhArray[i]);
    }

	return 0;
}