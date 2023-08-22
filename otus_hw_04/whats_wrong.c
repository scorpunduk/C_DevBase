#include <stdio.h>
#include <string.h>

int main(int argc, char * argv[])
{

	int name_length = strlen(argv[1]);
	char output_filename[30] = {0};
	char output_res[] = ".txt";
	strncat(output_filename, argv[1], name_length);
	strncat(output_filename, output_res, 4);

	printf(output_filename);

	return 0;
}