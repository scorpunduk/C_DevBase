gcc -c parser_check.c
gcc -c unicodeparser.c
gcc parser_check.o unicodeparser.o -o parser_check.exe
parser_check.exe