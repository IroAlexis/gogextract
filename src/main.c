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
		fprintf(stderr, "Usage: %s FILE\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	//mkdir(file, MODE_DIR);
	
	free(file);
	
	init_const_installer(argv[1], &o_size, &s_size, &f_size);
	
	extract_data(argv[1], "./unpacker.sh", 0, s_size);
	extract_data(argv[1], "./mojosetup.tar.gz", s_size, f_size);
	extract_bin(argv[1], "./data.zip", s_size + f_size);
	
	extract_game_standalone("./data.zip");
	
	return EXIT_SUCCESS;
}

