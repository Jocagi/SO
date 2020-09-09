#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	char ch, source_file[20], target_file[20];
	FILE *source, *target;

	//printf("Ingresa el nombre del archivo a copiar:\n");
	//gets(source_file);

	//source = fopen(source_file, "r");

	source = fopen(argv[1], "r");

	if( source == NULL)
	{
	printf("No existe el archivo a copiar");
	exit(EXIT_FAILURE);
	}

        //printf("Ingresa el nombre del archivo a reemplazar:\n");
        //gets(target_file);

        //target = fopen(target_file, "w");

	target = fopen(argv[2], "w");

        if( target == NULL)
        {
	fclose(source);
        printf("No existe el directorio destino");
        exit(EXIT_FAILURE);
        }

	while( ( ch = fgetc(source) ) != EOF )
		fputc(ch, target);

	printf("File copied succesfully.\n");

	fclose(source);
	fclose(target);

	return 0;
}
