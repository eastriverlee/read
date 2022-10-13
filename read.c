#include <sys/fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static inline size_t get_filesize(int file)
{
	size_t filesize = lseek(file, 0, SEEK_END);

	lseek(file, 0, SEEK_SET);
	return filesize;
}

#define ERROR -1
static void read_whole(int file, char **content)
{
	const size_t filesize = get_filesize(file);
	char *to_free;

	*content = malloc(filesize + 1);
	if (*content)
		_exit(1); //malloc failed
	if (read(file, *content, filesize) == ERROR)
		_exit(1); //read failed
	(*content)[filesize] = 0;
}

static inline void open_at(int *where, char *filename, int flags)
{
	*where = open(filename, flags);
	if (*where == ERROR)
		_exit(1); //open failed
}

int main(int argc, char **argv)
{
	char *filename, *content;
	int file;

	if (argc == 2)
	{
		filename = argv[1];
		open_at(&file, filename, O_RDONLY);
		read_whole(file, &content);
		puts(content);
	}
}
