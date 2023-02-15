#include <stdio.h>

// ��������� ������ ����� jpg
#define JPG_START_SIGN 0xFFD8

// ��������� ���������� ����� jpg
#define JPG_END_SIGN 0xFFD9

/*  ��������� ���������� zip-�����, ������ ������������ � zip-�����,
    ���� ���� �� ������. ������������ ������ �������,
    ��� ��� ���� ��������� ����� �����������
*/
#define ZIP_END_SIGN 0x504B0506



//*************************************************************************************************
// �������� ��������
void filedivingtools_init()
{
    printf("init success...");
}


//*************************************************************************************************
/* ���������� �������� �� ���� jpg
*/
int isJPG(FILE * file, int view_info)
{
    char signature[2] = {0, 0};
    short welded = 0;
    int result = 0;

    signature[1] = (char)fgetc(file);
    signature[0] = (char)fgetc(file);
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

    // ����� ������� �� ����� ����������� �������� ��������� ������������ �����
    // ��� ��� ����������� � ���������� ��������� ����� �� ����� �������
    fseek(file, 0, 0);
    return result;
}



//*************************************************************************************************
/*  ���������� ������� ���������� ����� ����������� ��������� jpg (�� EOF)
    0 ���� ��������� �� ���� �������
*/
long long unsigned jpgEndPosition(FILE * file)
{
    // ���� ��� ������ �������� ����� �����
    int current_byte = 0;

    // ������� ������ �� ������ ����� ��� ������ ���������
    long long unsigned byte_counter = 0;

    // ��������� ���� ��������� ���������
    long long unsigned end_sign_byte = 0;

    // ������ ��� ������ ������
    unsigned char sign_array[2] = {0x00, 0x00};

    // ���� ��� "�������" ������ � ����������� ��������
    // ��� ��������� � JPG_END_SIGN
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

        // �� ����� ���� ����� ����� ���� �� ������ ��������� ��������� ����
        // ���� �� ���������� �������� ����� ��������
        if(current_byte == 0xFF)
        {
            sign_array[1] = (unsigned char)current_byte;
            current_byte = fgetc(file);
            byte_counter++;
            if(current_byte == EOF)
            {
                end_sign_byte = 0;
                break;
            }
            sign_array[0] = (unsigned char)current_byte;
            result_sign = *(short *)(sign_array);
            if(result_sign == (short)JPG_END_SIGN) // ���������� ��� ��� ���������� int
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
/* ������������ ���������� ���� � �����
*/
long long unsigned int get_file_bytes_value(FILE * file, int view_info)
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

    // ����� ������� �� ����� ����������� �������� ��������� ������������ �����
    // ��� ��� ����������� � ���������� ��������� ����� �� ����� �������
    fseek(file,0,0);

    if(view_info)
    {

    }
    return byte_counter;
}



//*************************************************************************************************
/* ���������� �������� �� ���� ��������� ���������� zip-�����
*/
int hasZIPinside(FILE * file, int view_info)
{
    // ���� ��� ������ �������� ����� �����
    int current_byte = 0;

    // ������� ������ �� ������ ����� ��� ������ ���������
    long long unsigned byte_counter = 0;

    // ������ ���� ��������� ���������
    long long unsigned start_sign_byte = 0;

    // ��������� ���� ��������� ���������
    long long unsigned end_sign_byte = 0;

    // ������ ��� ������ ������
    unsigned char sign_array[4] = {0x00, 0x00, 0x00, 0x00};

    // ������ ��� ��������� � ��������� sign_array
    // *����� ����� ��� ���� �������� ��������, ����� ����������� ����
    // *��. ����
    char index = 0;

    // ���� ��� "�������" ������ � ������������� ��������
    // ��� ��������� � ZIP_END_SIGN
    int result_sign = 0x00000000;

    // ���� ��� ���������� �������� ����� � ���������� ����� �����
    // �� ���������� �����
    int eof_reached = 0;

    // �������� 1 ���� � ����� ������� ��������� ����� zip, 0 ���� �����
    int file_has_zip_end = 0;

    printf("------------------------------------------------------------\n");
    printf("Search zip-end signature...\n");

    while(1)
    {
        current_byte = fgetc(file);
        byte_counter++;
        if(current_byte == EOF)
            break;

        // ����� �������� ������������ ����� ����� ������� �����
        // ���������, ������������ ������ ��������� ��� ���� � �������
        // �� ������ � ������ � ���� �������� ��� ���������
        if(current_byte == 0x50)
        {
            //printf("%.2hhx", current_byte);
            //-1 ��� ��� ����� �����, � �� ���������� �����
            start_sign_byte = byte_counter - 1;
            sign_array[3] = current_byte;
            index = 2;
            while(index >= 0)
            {
                byte_counter++;
                current_byte = fgetc(file);
                if(current_byte == EOF) { eof_reached = 1; break; }
                //printf("%.2hhx", current_byte);
                sign_array[index--] = (unsigned char)current_byte;
            }
            //printf("\n");
            if(eof_reached) break;
            result_sign = *(int *)(sign_array);
            if(result_sign == ZIP_END_SIGN)
            {
                printf("---> Signature %X reached <---\n",result_sign);
                end_sign_byte = byte_counter - 1;
                file_has_zip_end = 1;
                break;
            }
        }
    }

    // ����� ����������� ���������
    if(view_info)
    {
        if(file_has_zip_end)
        {
            printf("File has zip-end signature: %x\n", ZIP_END_SIGN);
            printf("Start signature byte number: %llu\n", start_sign_byte);
            printf("End signature byte number: %llu\n", end_sign_byte);
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
/*  ������� ������ ����� ��� ���������� ������������ ���� ��� ������ ������
    ������ �� �������� ������� jpg-�����, ����������� zip-�����
*/
void pullOutHiddenZip(FILE * file, long unsigned start_position)
{
    fseek(file, start_position, 0);

    // ����� ���� ��� ������
    FILE * pulled_out_zip = fopen("pulledOut.zip","wb");

    // ���� ��� ������ �������� ����� �����
    int current_byte = 0;

    while(1)
    {
        current_byte = fgetc(file);
        if(current_byte == EOF)
            break;
        putc(current_byte, pulled_out_zip);
    }

    fseek(file, 0, 0);
    fclose(pulled_out_zip);
}
