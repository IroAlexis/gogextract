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


void print_help()
{
	fprintf(stdout, "Usage: gogextract [OPTION...] FILE\n");
	fprintf(stdout, " 'gogextract' unpacks GOG Linux installer or direct install the game\n");
	fprintf(stdout, " without going through it\n\n");
	fprintf(stdout, "Note: The GOG Linux installer is a '.sh' file contains 'unpacker.sh',\n");
	fprintf(stdout, "'mojosetup.tar.gz' and 'data.zip'.\n\n");
	fprintf(stdout, "Examples:\n");
	fprintf(stdout, " gogextract gog_installer.sh         # Extract all files from gog_installer.sh\n");
	fprintf(stdout, " gogextract -g . gog_installer.sh    # Install the game inside the current path\n\n");
	fprintf(stdout, "Game standalone extraction:\n");
	fprintf(stdout, "  -g, --game-standalone        install the game in the path passed in argument,\n");
	fprintf(stdout, "                               if it exists\n");
	fprintf(stdout, "  -d, --dir-game-standalone    create the folder game passed in argument if it\n");
	fprintf(stdout, "                               doesn't exist and install the game in it (WIP)\n");
	fprintf(stdout, "Others options:\n");
	fprintf(stdout, "  -h, --help                   give this help list\n");
	fprintf(stdout, "  -v, --version                print program version\n");
	fprintf(stdout, "Mandatory arguments to long options are mandatory for short options too.\n");
}


int main(int argc, char* argv[])
{
	int   opt;
	int   longindex;
	int   flag;
	long  o_size;
	long  s_size;
	long  f_size;
	char* d_game = NULL;
	
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
				d_game = (char*) calloc(strlen(optarg) + 1, sizeof(char));
				strncpy(d_game, optarg, strlen(optarg) + 1);
				break;
			case 'v':
				fprintf(stdout, "%s-0.1\n", argv[0]);
				flag = -1;
				break;
			case 'h':
				flag = -1;
				print_help();
				break;
			default:
				flag = 0;
		}
	}
	
	if (flag >= 0)
	{
		init_const_installer(argv[argc - 1], &o_size, &s_size, &f_size);
		
		if (flag == 0)
		{
			extract_data(argv[argc - 1], "./unpacker.sh", 0, s_size);
			extract_data(argv[argc - 1], "./mojosetup.tar.gz", s_size, f_size);
		}
		
		if (flag == 1)
		{
			extract_bin(argv[argc - 1], "./data.zip", s_size + f_size);
		
			if (d_game[strlen(d_game) - 1] != '/')
				d_game = strncat(d_game, "/", strlen("/") + 1);

			extract_game_standalone("./data.zip", d_game);
		}
	}
	free(d_game);
	
	return EXIT_SUCCESS;
}

