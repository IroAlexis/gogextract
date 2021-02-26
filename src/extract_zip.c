/*
 * extract_zip.c
 * Copyright (C) 2020-2021 Alexis Peypelut <peypeluta@live.fr>
 *                         Thibault Peypelut <thibault.pey@gmail.com>
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


#include "extract_zip.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <regex.h>
#include <zip.h>


#define SIZE 1024


/*!
 * @brief Convert the file attributes manipulate with libzip in unix file permissions
 * @param attributes File attributes
 * @return Unix file permissions
 **/
int attr_to_unix_perm(const zip_uint32_t attributes)
{
	int mask = (1 << 10) - 1;
	int perm = (attributes & (mask << 16)) >> 16;
	
	return perm;
}


int safe_create_dir(const char* dir, const int perm)
{
	if (mkdir(dir, perm) < 0)
	{
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}


int extract_game_standalone(const char* f_zip, const char* dest)
{
	zip_int64_t      ix;
	zip_int64_t      len;
	zip_int64_t      fd;
	zip_int64_t      nbrf;
	zip_uint64_t     sum;
	zip_uint32_t     attributes;
	zip_t*           stream;
	struct zip_file* z_file;
	struct zip_stat  z_stat;
	
	int         perm;
	char        buffer[SIZE];
	char*       tmp = NULL;
	const char* p_index;
	regex_t     preg;
	
	if (regcomp(&preg, "^data/noarch/.+", REG_EXTENDED))
		return EXIT_FAILURE;
	
	stream = zip_open(f_zip, ZIP_RDONLY, NULL);
	if (NULL == stream)
		return EXIT_FAILURE;
	
	nbrf = zip_get_num_entries(stream, ZIP_FL_UNCHANGED);
	//fprintf(stdout, "%ld\n", nbrf);
	
	for (ix = 0; ix < nbrf; ix++)
	{
		if (zip_stat_index(stream, ix, ZIP_FL_UNCHANGED, &z_stat) != 0)
			continue;
		
		if (!regexec(&preg, z_stat.name, 0, NULL, 0))
		{
			// Get attributes entry
			zip_file_get_external_attributes(stream, ix, ZIP_FL_UNCHANGED, NULL, &attributes);
			// Convert attibutes to unix permission
			perm = attr_to_unix_perm(attributes);
			
			// Place the pointer on the file path where it is intresting for us
			p_index = z_stat.name + 12;
			
			// Build the path for create the files and folders
			tmp = (char*) calloc(sizeof(char), strlen(dest) + strlen(p_index) + 1);
			tmp = strncat(tmp, dest, strlen(dest));
			tmp = strncat(tmp, p_index, strlen(p_index) + 1);
			fprintf(stderr, "[-] info:: %s\n", tmp);
			
			// Checks if the entry is a directory
			if (attributes & (1 << 30))
				safe_create_dir(tmp, perm);
			else
			{
				// Open index archive (file)
				z_file = zip_fopen_index(stream, ix, 0);
				if (!z_file)
				{
					fprintf(stderr, "[-] err:: Error zip open index\n");
					return EXIT_FAILURE;
				}
				
				// Create the uncompressed file
				fd = open(tmp, O_RDWR | O_TRUNC | O_CREAT, perm);
				if (fd < 0)
				{
					fprintf(stderr, "[-] err:: Error open file descriptor %s\n", z_stat.name);
					return EXIT_FAILURE;
				}

				sum = 0;
				while (sum != z_stat.size)
				{
					len = zip_fread(z_file, buffer, SIZE);
					if (len < 0)
					{
						fprintf(stderr, "[-] err:: Error extract file\n");
						return EXIT_FAILURE;
					}
					write(fd, buffer, len);
					sum += len;
				}
				close(fd);
				zip_fclose(z_file);
			}
			
			free(tmp);
		}
	}
	
	zip_close(stream);
	regfree(&preg);
	
	return EXIT_SUCCESS;
}

