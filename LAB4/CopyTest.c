#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	char ch, source_file[100], target_file[100];
	FILE *source, *target;

	//Input source file
	source = fopen(argv[1], "r");

	if( source == NULL)
	{
	printf("No existe el archivo a copiar\n");
	exit(EXIT_FAILURE);
	}

	//Input destination file
	strcpy(target_file, argv[2]);
	strcat(target_file, "/");
	strcat(target_file, argv[1]);
	target = fopen(target_file, "w");

        if( target == NULL)
        {
	fclose(source);
        printf("No existe el directorio destino\n");
        exit(EXIT_FAILURE);
        }

	while( ( ch = fgetc(source) ) != EOF )
		fputc(ch, target);

	printf("File copied succesfully.\n");

	fclose(source);
	fclose(target);

	return 0;
}
