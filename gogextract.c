#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
//#include <syslog.h>

#define NAME_DIR "game"
#define MODE_DIR 0755
#define SIZE     1024

#define ERR_FILE -1

#define FILESIZES "filesizes=\""
#define OFFSET    "offset=`head -n "
#define LABEL     "label=\""
#define TAG       " (GOG.com)\"\n"


int get_script_const(const char* path, const char* str, const int size)
{
	int   rslt;
	char  line[SIZE];
	char* occ;
	FILE* stream;
	
	
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
	char          line[SIZE];
	char*         occ;
	unsigned long l_occ;
	char          tmp[SIZE];
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


long get_script_size(const char* path, const long l_end)
{
	FILE* stream;
	long  ix;
	long  size;
	char  line[SIZE];
	
	// TODO Revise the return
	size = -1;
	
	// Require through the entire script at least once
	// and need open not in binary mode
	stream = fopen(path, "r");
	if (NULL != stream)
	{
		ix = 1;
		
		while (fgets(line, sizeof(line), stream) != NULL)
		{
			if (ix == l_end)
				size = ftell(stream);
			
			ix++;
		}
		
		fclose(stream);
	}
	
	return size;
}


void extract_data(const char* src, const char* dest,
				  const long pos, const long size)
{
	FILE* s_stream;
	FILE* d_stream;
	char  buffer[SIZE];
	long  r_bloc;
	long  nmemb;
	long  end;
	
	// Split max lenght
	nmemb = size / SIZE;
	end = pos + size;
	
	// FIXME Exceed disk size ?
	d_stream = fopen(dest, "wb");
	if (NULL != d_stream)
	{
		s_stream = fopen(src, "rb");
		if (NULL != s_stream)
		{
			// Set the position in the source stream
			fseek(s_stream, pos, SEEK_SET);
			
			for (r_bloc = 1; r_bloc <= nmemb; r_bloc++)
			{
				fread(buffer, SIZE, 1, s_stream);
				fwrite(buffer, SIZE, 1, d_stream);
			}
			
			if (ftell(s_stream) < end)
			{
				fread(buffer, size % SIZE, 1, s_stream);
				fwrite(buffer, size % SIZE, 1, d_stream);
			}
			
			fclose(s_stream);
			fclose(d_stream);
		}
	}
}


int main(int argc, char* argv[])
{
	// It is just a test for the moment
	int   f_size;
	int   o_size;
	long  s_size;
	char* file = NULL;
	
	file = get_name_game(argv[1]);
	file = format_string(file, '-');
	fprintf(stdout, "%s\n", file);
	
	//mkdir(file, MODE_DIR);
	
	free(file);
	
	o_size = get_script_const(argv[1], OFFSET, strlen(OFFSET));
	fprintf(stdout, "Script lines: %d\n", o_size);
	
	fprintf(stdout, "Makeself script size: ");
	fflush(stdout);
	s_size = get_script_size(argv[1], o_size);
	fprintf(stdout, "%ld\n", s_size);
	
	fprintf(stdout, "MojoSetup archive size: ");
	fflush(stdout);
	f_size = get_script_const(argv[1], FILESIZES, strlen(FILESIZES));
	fprintf(stdout, "%d\n", f_size);
	
	extract_data(argv[1], "./script.sh", 0, s_size);
	extract_data(argv[1], "./mojosetup.tar.gz", s_size, f_size);
	
	return EXIT_SUCCESS;
}

