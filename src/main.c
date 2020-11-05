/*
 * main.c
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


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "extract_installer.h"
#include "extract_zip.h"

#define NAME_DIR "game"
#define MODE_DIR 0755


int main(int argc, char* argv[])
{
	// It is just a test for the moment
	long  f_size;
	long  o_size;
	long  s_size;
	char* file = NULL;
	
	if (argc != 2)
	{
		fprintf(stderr, "Usage: ./gogextract FILE\n");
		return EXIT_FAILURE;
	}
	
	//mkdir(file, MODE_DIR);
	
	free(file);
	
	o_size = get_script_const(argv[1], OFFSET, strlen(OFFSET));
	if (o_size > 0)
		fprintf(stdout, "Script lines: %ld\n", o_size);
	else if (o_size == 0)
	{
		fprintf(stderr, "gogextract: Invalid GOG installer, script size is much too small...\n");
		return EXIT_FAILURE;
	}
	else
	{
		fprintf(stderr, "gogextract: Error opening file %s: No such file or directory\n", argv[1]);
		return EXIT_FAILURE;
	}
	
	fprintf(stdout, "Makeself script size: ");
	fflush(stdout);
	s_size = get_script_size(argv[1], o_size);
	if (s_size != -1)
		fprintf(stdout, "%ld\n", s_size);
	else
	{
		// Impossible to be here but you never know
		fprintf (stderr, ":gogextract:get_script_size: Problem with the line number\n");
		return EXIT_FAILURE;
	}
	
	fprintf(stdout, "MojoSetup archive size: ");
	fflush(stdout);
	f_size = get_script_const(argv[1], FILESIZES, strlen(FILESIZES));
	fprintf(stdout, "%ld\n", f_size);
	
	extract_data(argv[1], "./unpacker.sh", 0, s_size);
	extract_data(argv[1], "./mojosetup.tar.gz", s_size, f_size);
	extract_bin(argv[1], "./data.zip", s_size + f_size);
	
	extract_game_standalone("./data.zip");
	
	return EXIT_SUCCESS;
}

