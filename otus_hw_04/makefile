FLAGS= -Wall -Wextra -Wpedantic -std=c11
PROGRAM = unicodedecoder
DIR = obj

$(PROGRAM) : parseToUTF8.o unicodeparser.o
		gcc -o $(PROGRAM) $(FLAGS) parseToUTF8.o unicodeparser.o

parseToUTF8.o : parseToUTF8.c unicodeparser.h
		gcc -c $(FLAGS) parseToUTF8.c

unicodeparser.o : unicodeparser.c unicodeparser.h
		gcc -c $(FLAGS) unicodeparser.c

# --------------------------------------------------------------

clean :
		rm -r $(DIR)
fold :
		mkdir $(DIR)
		mv $(PROGRAM) ./$(DIR)
		mv *.o ./$(DIR)
