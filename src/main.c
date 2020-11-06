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
#include <getopt.h>

#include "extract_installer.h"
#include "extract_zip.h"

#define NAME_DIR "game"
#define MODE_DIR 0755


static struct option longopts[] =
{
	{"game-standalone", required_argument, NULL, 'g'},
	{"help",            no_argument,       NULL, 'h'},
	{"version",         no_argument,       NULL, 'v'},
	{NULL,              0,                 NULL, 0}
	
};


int main(int argc, char* argv[])
{
	int   opt;
	int   longindex;
	int   flag;
	long  f_size;
	long  o_size;
	long  s_size;
	//char* file = NULL;
	
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s FILE\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	//mkdir(file, MODE_DIR);
	//free(file);
	
	while ((opt = getopt_long(argc, argv, "g:hv", longopts, &longindex)) != -1)
	{
		switch (opt)
		{
			case 'g':
				flag = 1;
				break;
			case 'v':
				fprintf(stdout, "%s-0.1\n", argv[0]);
				flag = -1;
				break;
			case 'h':
				flag = -1;
				break;
			default:
				flag = 0;
		}
	}
	
	if (flag > 0)
	{
		init_const_installer(argv[argc - 1], &o_size, &s_size, &f_size);
		
		if (flag == 0)
		{
			extract_data(argv[argc - 1], "./unpacker.sh", 0, s_size);
			extract_data(argv[argc - 1], "./mojosetup.tar.gz", s_size, f_size);
		}
		
		extract_bin(argv[argc - 1], "./data.zip", s_size + f_size);
		
		if (flag == 1)
			extract_game_standalone("./data.zip");
	}
	return EXIT_SUCCESS;
}

