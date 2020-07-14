#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

// Unable to open a file
#define ERR_FILE -1
#define BUFFER 512

#define S_FILESIZES "filesizes="
#define C_NBR_FSZES 11 // <-'
#define S_OFFSET "offset=`head -n "
#define C_NBR_OF 16 // <----'

int get_const(const char* name, const char* str, const int n)
{
	FILE*  stream;
	int    res;
	int    value;
	char   line[BUFFER];
	char*  occ;
	
	stream = fopen (name, "r");
	if (NULL == stream)
		return ERR_FILE;
	
	// Read the file line by line
	while (fgets(line, sizeof(line), stream) != NULL)
	{
		// Find the first occurence
		occ = strstr(line, str);
		if (NULL != occ)
		{
			// Convert string into integer
			res = sscanf(&occ[n], "%d", &value);
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
	
	res = get_const(argv[1], S_FILESIZES, C_NBR_FSZES);
	printf("%d\n", res);
	res = get_const (argv[1], S_OFFSET, C_NBR_OF);
	printf("%d\n", res);
	
	return EXIT_SUCCESS;
}

