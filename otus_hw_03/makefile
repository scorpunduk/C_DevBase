FLAGS= -Wall -Wextra -Wpedantic -std=c11
PROGRAM = findzip
DIR = obj

$(PROGRAM) : zipdive.o zipfuncs.o
		gcc -o $(PROGRAM) $(FLAGS) zipdive.o zipfuncs.o

zipdive.o : zipdive.c zipdive.h
		gcc -c $(FLAGS) zipdive.c

zipfuncs.o : zipfuncs.c zipdive.h
		gcc -c $(FLAGS) zipfuncs.c

# https://qna.habr.com/q/771017
# пример компиляции в директорию, я всё равно его не понимаю
# оставляю на будущее
clean :
		rm -r $(DIR)
fold :
		mkdir $(DIR)
		mv $(PROGRAM) ./$(DIR)
		mv *.o ./$(DIR)