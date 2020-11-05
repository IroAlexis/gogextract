/*
 * extract_installer.c
 * Copyright (C) 2020 Alexis Peypelut <peypeluta@live.fr>
 *
 * gogextract is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * gogextract is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "extract_installer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
//#include <syslog.h>

#define SIZE     1024
#define ERR_FILE -1

int get_script_const(const char* path, const char* str, const int size)
{
	int   rslt;
	char  line[SIZE];
	char* occ;
	FILE* stream;
	
	
	rslt = ERR_FILE;
	
	stream = fopen(path, "r");
	if (NULL != stream)
	{
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
	
	// If we found not LABEL, we return NULL but problem for later ?
	return rslt;
}


long get_script_size(const char* path, const long l_end)
{
	FILE* stream;
	long  ix;
	long  size;
	char  line[512];
	
	size = -1;
	
	if (l_end != 0)
	{
		// Can't open in binary mode
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


void extract_bin(const char* src, const char* dest, const long pos)
{
	FILE* s_stream;
	FILE* d_stream;
	char  buffer[SIZE];
	long  f_size;
	long  cur;
	
	// FIXME Exceed disk size ?
	d_stream = fopen(dest, "wb");
	if (NULL != d_stream)
	{
		s_stream = fopen(src, "rb");
		if (NULL != s_stream)
		{
			// Find the file size
			fseek(s_stream, 0, SEEK_END);
			f_size = ftell(s_stream);
			
			// Set the position in the source stream
			fseek(s_stream, pos, SEEK_SET);
			
			// Prevention for not to go beyond the EOF
			for (cur = ftell(s_stream);
				 cur + SIZE < f_size;
				 cur = ftell(s_stream))
			{
				fread(buffer, SIZE, 1, s_stream);
				fwrite(buffer, SIZE, 1, d_stream);
			}
			
			// Save the data rest
			if (cur < f_size)
			{
				fread(buffer, f_size - cur, 1, s_stream);
				fwrite(buffer, f_size - cur, 1, d_stream);
			}
			fclose(s_stream);
			fclose(d_stream);
		}
	}
}

