/*
 * extract_zip.h
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



/*!
 * @brief Extract only the game since the archive
 * @param f_zip Archive path
 * @param dest  Extraction destination of the game
 * @return 0 if it is a success, -1 otherwise
 **/
int extract_game_standalone (const char* f_zip, const char* dest);


/*!
 * @brief Create a directory
 * @param dir Directory name
 * @param perm Permission to be assigned
 * @return 0 if it is success, -1 otherwise
 **/
int safe_create_dir (const char* dir, const int perm);

