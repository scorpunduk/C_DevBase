/*
    ��������� ��������� ����� ������� ������ jpg �������
    windows cmd -> folder -> program.exe *.jpg
    ubuntu ctrl+alt+t -> folder -> ./program *.jpg
    https://g-soft.info/articles/4887/kak-napisat-i-zapustit-programmu-na-yazyke-c-v-linux
*/

#include <stdio.h>
#include <stdlib.h>
#include "filedivingtools.h"

int main(int argc, char * argv[])
{
    FILE * target;
    int execute_further = 0;
    int file_has_hidden_zip = 0;
    long long unsigned position = 0;
    char choice = 0;

    // ���� � ��������� ������ ������� ������ ���������
    if(argc != 2)
    {
        printf("Using program: %s\n", argv[0]);
        printf("Please, launch program with file parameter\n");
        printf("Template for launch:\n");
        printf("on win: <program folder path> <program name> <space> <file for check>\n");
        printf("on linux: <program folder path> ./ <program name> <space> <file for check>\n");
        printf("or just drage and drop jpg on program\n");
        exit(EXIT_FAILURE);
    }

    // ���� ���������� ������� ����
    if((target = fopen(argv[1], "rb")) == NULL)
    {
        printf("Can't open file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // ���� ���� �� �������� jpg
    if(!isJPG(target, 1))
    {
        exit(EXIT_FAILURE);
    }

    printf("File volume: %llu bytes\n", get_file_bytes_value(target,0));

    file_has_hidden_zip = hasZIPinside(target, 1);

    // ���� ������ ������� zip-����� ��� ����� ������� � ����� � ����������
    if(file_has_hidden_zip)
    {
        printf("**************************************************\n");
        printf("Do you want to pull out hidden zip-archive? (y/n)");
        while((choice = getchar()) != EOF)
        {
            fflush(stdin);
            if(choice != 'y' && choice != 'n')
                printf("...please, input only \"y\" or \"n\"\n");
            else break;
        }
        if(choice == 'y')
        {
            position = jpgEndPosition(target);
            pullOutHiddenZip(target, (long unsigned)position);
            printf("ZIP-archive was pulled out. Check program folder.\n");
        }
    }

    fclose(target);
    printf("Press any key to quite.\n");
    getchar();
    return 0;
}
