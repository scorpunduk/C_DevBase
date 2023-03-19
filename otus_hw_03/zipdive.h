#include <stdio.h>
#include <stdint.h>

// сигнатура начала стуктуры LocalFileHeader
#define ZIP_LFH_SIGN 0x04034b50

// количество байт
#define ZIP_LFH_BYTES 46

// сигнатура начала структуры CentralDirectoryFileHeader
#define ZIP_CDFH_SIGN 0x02014b50

/*  сигнатура начала структуры EndOfCentralDirectory
    всегда присутствует в zip-файле,
    даже если он пустой. Единственный верный признак,
    так как иные сигнатуры могут различаться */
#define ZIP_EOCD_SIGN 0x06054b50

// сигнатура начала файла jpg
#define JPG_START_SIGN 0xD8FF
 
// сигнатура завершения файла jpg
#define JPG_END_SIGN 0xD9FF 


typedef struct LocalFileHeader_Struct LocalFileHeader;
struct LocalFileHeader_Struct
{
    // Обязательная сигнатура, равна 0x04034b50
    uint32_t signature;
    // Минимальная версия для распаковки
    uint16_t versionToExtract;
    // Битовый флаг
    uint16_t generalPurposeBitFlag;
    // Метод сжатия (0 - без сжатия, 8 - deflate)
    uint16_t compressionMethod;
    // Время модификации файла
    uint16_t modificationTime;
    // Дата модификации файла
    uint16_t modificationDate;
    // Контрольная сумма
    uint32_t crc32;
    // Сжатый размер
    uint32_t compressedSize;
    // Несжатый размер
    uint32_t uncompressedSize;
    // Длина название файла
    uint16_t filenameLength;
    // Длина поля с дополнительными данными
    uint16_t extraFieldLength;

    // ошибочно полагал, что далее находятся всего лишь 2 поля
    // с указателями на имя файла и какие-то дополнительные
    // данный, сейчас становится понятно, что это указатель
    // на массив заданной длины.
    // Иначе говоря - адреса этих указателей - не записаны в сам
    // файл. При парсинге придётся отдельно выделять место под
    // содержимое массива байт под них
    // и похоже, что придётся читать в них сожержимое
    // после поля extraFieldLength

    // Название файла (размером filenameLength)
    // (Указатель на строковый массив указанного размера)
    uint8_t * filename;

    // Дополнительные данные (размером extraFieldLength)
    // (Указатель на строковый массив указанного размера)
    uint8_t * extraField;
    /*
    */
};


typedef struct CentralDirectoryFileHeader_Struct CentralDirectoryFileHeader;
struct CentralDirectoryFileHeader_Struct
{
    // Обязательная сигнатура, равна 0x02014b50 
    uint32_t signature;
    // Версия для создания
    uint16_t versionMadeBy;
    // Минимальная версия для распаковки
    uint16_t versionToExtract;
    // Битовый флаг
    uint16_t generalPurposeBitFlag;
    // Метод сжатия (0 - без сжатия, 8 - deflate)
    uint16_t compressionMethod;
    // Время модификации файла
    uint16_t modificationTime;
    // Дата модификации файла
    uint16_t modificationDate;
    // Контрольная сумма
    uint32_t crc32;
    // Сжатый размер
    uint32_t compressedSize;
    // Несжатый размер
    uint32_t uncompressedSize;
    // Длина название файла
    uint16_t filenameLength;
    // Длина поля с дополнительными данными
    uint16_t extraFieldLength;
    // Длина комментариев к файлу
    uint16_t fileCommentLength;
    // Номер диска
    uint16_t diskNumber;
    // Внутренние аттрибуты файла
    uint16_t internalFileAttributes;
    // Внешние аттрибуты файла
    uint32_t externalFileAttributes;
    // Смещение до структуры LocalFileHeader
    uint32_t localFileHeaderOffset;
    // Имя файла (длиной filenameLength)
    uint8_t *filename;
    // Дополнительные данные (длиной extraFieldLength)
    uint8_t *extraField;
    // Комментарий к файла (длиной fileCommentLength)
    uint8_t *fileComment;
};


typedef struct EOCD_Struct EOCD;
struct EOCD_Struct
{
    // Обязательная сигнатура, равна 0x06054b50
    uint32_t signature;
    // Номер диска
    uint16_t diskNumber;
    // Номер диска, где находится начало Central Directory
    uint16_t startDiskNumber;
    // Количество записей в Central Directory в текущем диске
    uint16_t numberCentralDirectoryRecord;
    // Всего записей в Central Directory
    uint16_t totalCentralDirectoryRecord;
    // Размер Central Directory
    uint32_t sizeOfCentralDirectory;
    // Смещение Central Directory
    uint32_t centralDirectoryOffset;
    // Длина комментария
    uint16_t commentLength;
    // Комментарий (длиной commentLength)
    uint8_t *comment;
};


typedef struct DataDescriptor_Struct DataDescriptor;
struct DataDescriptor_Struct
{
    // Необязательная сигнатура, равна 0x08074b50
    uint32_t signature;
    // Контрольная сумма
    uint32_t crc32;
    // Сжатый размер
    uint32_t compressedSize;
    // Несжатый размер
    uint32_t uncompressedSize;
};


//*************************************************************************************************
int isJPG(FILE * file, int view_info);


//*************************************************************************************************
/*  возвращает позицию последнего байта завершающей сигнатуры jpg (до EOF)
    0 если сигнатура не была найдена
*/
long long unsigned jpgEndPosition(FILE * file);


///*************************************************************************************************
/* подсчитывает количество байт в файле
*/
long long unsigned get_file_bytes_value(FILE * file, int view_info);


//*************************************************************************************************
int hasZIPinside(FILE * file, int view_info);


//*************************************************************************************************
// возвращает количество сигнатур LocalFileHeader в архиве
unsigned count_local_file_header(FILE * file);


//*************************************************************************************************
// заполнение массива структур
void define_LocalFileHeader_array(
    FILE * file, 
    LocalFileHeader * lfhArray[],
    unsigned int arrayLength);


//*************************************************************************************************
// заполняет массив именами заархивированных файлов
void fill_lcf_signature_array(FILE * file, int * signatureArray);


//*************************************************************************************************
// парсинг данных из zip-архива в структуру LocalFileHeader_Struct
// 
void parse_LocalFileHeader(FILE * zipfile, LocalFileHeader * targetStruct, int signaturePos);

//*************************************************************************************************
// функция считывает байты из массива в поле структуры
// Предусловия передачи массива
// 1. массив хранит элементы типа unsigned char
// 2. длина массива равна количеству байт в упакованной структуре
// 3. массив объявлен и проинициализирован из источника данных (файл)
// 
// position - указатель на индекс с которого начинается считывание байт из массива в структуру
// изменяется в функции через прибавление fieldSize - размер в байтах заполняемого поля
// поэтому важно запускать функцию для всех полей заполняемой структуры последовательно
void fill_struct_fields
    (unsigned char * bytesArray, int * position, void * startFieldPointer, int fieldSize);