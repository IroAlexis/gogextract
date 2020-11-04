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


int attr_to_unix_perm(const zip_uint32_t attributes);

int extract_game_standalone(const char* path);

int safe_create_dir(const char* dir, const int perm);

