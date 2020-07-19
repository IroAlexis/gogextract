#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
//#include <syslog.h>

#define NAME_DIR "game"
#define MODE_DIR 0755
#define BUFFER 512

#define ERR_FILE -1

#define FILESIZES "filesizes=\""
#define OFFSET    "offset=`head -n "
#define LABEL     "label=\""
#define TAG       " (GOG.com)\"\n"


int get_const(const char* path, const char* str, const int size)
{
	int    rslt;
	char   line[BUFFER];
	char*  occ;
	FILE*  stream;
	
	
	stream = fopen(path, "r");
	if (NULL != stream)
	{
		rslt = ERR_FILE;
		
		// Read the file line by line
		while (fgets(line, sizeof(line), stream) != NULL)
		{
			// Find the first occurence
			occ = strstr(line, str);
			if (NULL != occ)
			{
				// Convert string into integer
				if (sscanf(&occ[size], "%d", &rslt) != 1)
					fprintf(stderr, "=> Not integer in the string\n");
				
				// Needn't to be continue
				break;
			}
		}
		
		fclose(stream);
	}
	
	return rslt;
}


char* format_string(char* str, const char ch)
{
	unsigned long ix;
	
	for (ix = 0 ; ix < strlen(str) ; ix++)
	{
		if (0 != isupper(str[ix]))
			str[ix] = tolower(str[ix]);
		
		if (ch != ' ' && str[ix] == ' ')
			str[ix] = ch;
	}
	
	return str;
}


char* get_name_game(const char* path)
{
	char*         rslt;
	FILE*         stream;
	char          line[BUFFER];
	char*         occ;
	unsigned long l_occ;
	char          tmp[BUFFER];
	unsigned long l_tmp;
	unsigned long l_tag;
	unsigned long size;
	
	
	l_occ = strnlen(LABEL, sizeof(LABEL));
	l_tag = strnlen(TAG, sizeof(TAG));
	rslt = NULL;
	
	stream = fopen(path, "r");
	if (NULL != stream)
	{
		// Read the file line by line
		while (fgets(line, sizeof(line), stream) != NULL)
		{
			// Find the first occurence
			occ = strstr(line, LABEL);
			if (NULL != occ)
			{
				//syslog(LOG_INFO, "gogunpack:: %s", occ);
				strncpy(tmp, &occ[l_occ], sizeof(tmp));
				
				// Needn't to be continue
				break;
			}
		}
		fclose(stream);
		
		
		// Require format the string correctly
		l_tmp = strnlen(tmp, sizeof(tmp));
		size = l_tmp - l_tag;
		
		rslt = (char*) calloc(size + 1, sizeof(char));
		if (NULL != rslt)
		{
			rslt = strncpy(rslt, tmp, size);
			rslt[size] = '\0';
		}
	}
	
	return rslt;
}


long get_file_size(const char* path)
{
	FILE* stream;
	long  size;
	
	size = -1;
	
	stream = fopen (path, "r");
	if (NULL != stream)
	{
		// Go to end
		fseek(stream, 0, SEEK_END);
		size = ftell(stream);
		
		fclose(stream);
	}
	
	return size;
}


int main(int argc, char* argv[])
{
	// It is just a test for the moment
	int f_size;
	int o_size;
	char* file = NULL;
	
	file = get_name_game (argv[1]);
	file = format_string(file, '-');
	printf("%s\n", file);
	
	mkdir (file, MODE_DIR);
	
	free(file);
	
	o_size = get_const (argv[1], OFFSET, strlen(OFFSET));
	printf("script_lines: %d\n", o_size);
	f_size = get_const(argv[1], FILESIZES, strlen(FILESIZES));
	printf("MojoSetup archive size: %d\n", f_size);
	
	return EXIT_SUCCESS;
}

