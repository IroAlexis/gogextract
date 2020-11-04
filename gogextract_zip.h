#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <regex.h>
#include <errno.h>
#include <zip.h>

#define SIZE 1024


/*!
 * @brief Convert the file attributes manipulate with libzip in unix file permissions
 * @param attributes File attributes
 * @return Unix file permissions
 **/
int attr_to_unix_perm(const zip_uint32_t attributes);


/*!
 * @brief Extract only the game since the archive
 * @param path Archive path
 * @return 0 if it is a success, -1 otherwise
 **/
int extract_game_standalone(const char* path);


/*!
 * @brief Create a directory
 * @param dir Directory name
 * @param perm Permission to be assigned
 * @return 0 if it is success, -1 otherwise
 **/
int safe_create_dir(const char* dir, const int perm);

