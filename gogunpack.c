#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

// Unable to open a file
#define ERR_FILE -1
#define BUFFER 512

#define STR_FILESIZES "filesizes="
#define F_COUNT 11 // <-----'
#define STR_OFFSET "offset=`head -n "
#define O_COUNT 16 // <------'

int get_const(const char* path, const char* str, const int count)
{
	FILE*  stream;
	int    res;
	int    value;
	char   line[BUFFER];
	char*  occ;
	
	
	stream = fopen (path, "r");
	if (NULL == stream)
		return ERR_FILE;
	
	value = ERR_FILE;
	
	// Read the file line by line
	while (fgets(line, sizeof(line), stream) != NULL)
	{
		// Find the first occurence
		occ = strstr(line, str);
		if (NULL != occ)
		{
			// Convert string into integer
			res = sscanf(&occ[count], "%d", &value);
			if (res != 1)
				fprintf(stderr, "=> Not integer in the string\n");
			
			// Needn't to be continue
			break;
		}
	}
	
	fclose(stream);
	
	
	return value;
}

int main(int argc, char* argv[])
{
	int res;
	
	res = get_const(argv[1], STR_FILESIZES, F_COUNT);
	printf("%d\n", res);
	res = get_const (argv[1], STR_OFFSET, O_COUNT);
	printf("%d\n", res);
	
	return EXIT_SUCCESS;
}

