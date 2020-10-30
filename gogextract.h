/*
 * gogextract.h
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
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
//#include <syslog.h>

#define FILESIZES "filesizes=\""
#define OFFSET    "offset=`head -n "
#define LABEL     "label=\""
#define TAG       " (GOG.com)\"\n"


/*!
 * @brief Get the number of lines of the script unpacker.sh
 * @param path The file path, can't be NULL
 * @param str  Reccurent data allowing to have the number of lines, can't be NULL
 * @param size Buffer
 * @return The number of lines, -1 otherwise
 **/
int get_script_const(const char* path, const char* str, const int size);


/*!
 * @brief Format the game title in the Unix style (without space and special chars)
 * @param str Title game, can't be NULL
 * @param ch  Replace the special chars with the following char (may be destined to disappear)
 * @return
 **/
char* format_string(char* str, const char ch);


/*!
 * @brief Find the game title inside the script unpacker.sh
 * @param path The file path, can't be NULL
 * @return The game title
 **/
char* get_name_game(const char* path);


/*!
 * @brief Get the script size in bytes
 * @param path  The file path, can't be NULL
 * @param l_end The last line of the script
 * @return The script size
 **/
long get_script_size(const char* path, const long l_end);


/*!
 * @brief Extract a data file of GOG installer
 * @param src  The file path source, can't be NULL
 * @param dest The file path destination, can't be NULL
 * @param pos  The begin position inside GOG installer
 * @param size The size file that be extract
 **/
void extract_data(const char* src, const char* dest,
					const long pos, const long size);


/*!
 * @brief Extract the binary part (`data.zip`) of the GOG installer
 * @param src  The file path source, can't be NULL
 * @param dest The file path destination, can't be NULL
 * @param pos  The begin position of the binary part in the GOG installer
 **/
void extract_bin(const char* src, const char* dest, const long pos);

