/*
 * extract_installer.h
 * Copyright (C) 2020-2021 Alexis Peypelut <peypeluta@live.fr>
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


#define FILESIZES "filesizes=\""
#define OFFSET    "offset=`head -n "
#define LABEL     "label=\""
#define TAG       " (GOG.com)\"\n"


/*!
 * @brief Extract the binary part (`data.zip`) of the GOG installer
 * @param src  The file path source, can't be NULL
 * @param dest The file path destination, can't be NULL
 * @param pos  The begin position of the binary part in the GOG installer
 **/
void extract_bin(const char* src, const char* dest, const long pos);


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
 * @brief Find the game title inside the script unpacker.sh
 * @param path The file path, can't be NULL
 * @return The game title
 **/
char* get_name_game(const char* path);


/*!
 * @brief Initializes the installer constants
 * @param path   File path source, can't be NULL
 * @param o_size Number of lines of the script unpacker.sh
 * @param s_size Makeself script size
 * @param f_size MojoSetup archive size
 * @return 0 if it is good, -1 otherwise
 **/
int init_const_installer(const char* path, long* o_size,
						 long* s_size, long* f_size);

