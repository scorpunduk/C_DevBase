gcc -c parseToUTF8.c
gcc -c unicodeparser.c
gcc parseToUTF8.o unicodeparser.o -o parse.exe