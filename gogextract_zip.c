#include "gogextract_zip.h"


int safe_create_dir(const char* dir)
{
	if (mkdir(dir, 0755) < 0)
	{
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}


int extract_game_standalone(const char* path)
{
	zip_int64_t      ix;
	zip_int64_t      len;
	zip_int64_t      fd;
	zip_int64_t      nbrf;
	zip_uint64_t     sum;
	zip_t*           stream;
	struct zip_file* z_file;
	struct zip_stat  z_stat;
	
	char        buffer[SIZE];
	const char* npath;
	regex_t     preg;
	
	if (regcomp(&preg, "^data/noarch/.+", REG_EXTENDED))
		return EXIT_FAILURE;
	
	stream = zip_open(path, ZIP_RDONLY, NULL);
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
			npath = z_stat.name + 12;
			len = strlen(npath);
			
			if (npath[len - 1] == '/')
				safe_create_dir(npath);
			else
			{
				z_file = zip_fopen_index(stream, ix, 0);
				if (!z_file)
				{
					fprintf(stderr, "gogextract: error zip open index\n");
					return EXIT_FAILURE;
				}
				
				npath = z_stat.name + 12;
				//printf("%s\n", npath);
				
				fd = open(npath, O_RDWR | O_TRUNC | O_CREAT, 0644);
				if (fd < 0)
				{
					fprintf(stderr, "gogextract: error open file descriptor %s\n", z_stat.name);
					return EXIT_FAILURE;
				}

				sum = 0;
				while (sum != z_stat.size)
				{
					len = zip_fread(z_file, buffer, SIZE);
					if (len < 0)
					{
						fprintf(stderr, "gogextract: error extract file\n");
						return EXIT_FAILURE;
					}
					write(fd, buffer, len);
					sum += len;
				}
				close(fd);
			}
		}
	}
	
	zip_close(stream);
	regfree(&preg);
	
	return EXIT_SUCCESS;
}

