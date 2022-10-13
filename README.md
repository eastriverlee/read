# read

## overview

this is a minimal low-level c program that read a file at once.  
typically reading a file in c is done through multiple function calls.  
some use `getline(3)`, which is fine, but often is too bloated for doing such a simple task.  

using system calls `lseek(2)` and `read(2)`, twice and once respectively,  
after calling `open(2)`, another system call,
function `read_whole(int, char **)` handles most of the real-world scenarios.  

## limitation

this function cannot handle *(ridiculously)* large files;  
to be precise, a file with its size bigger than `ULONG_MAX - 1` bytes *(about 4.3GB)*.  

since this allocates the whole file to one `char *` type variable,  
naturally, `malloc((size_t)-1)` is the maximum size it can hold.  
*(including *`NUL`* character at the end of the string)*  

as aforementioned, unless you are modifying a video file manually,  
exeeding the limit is very very unlikely.  

## code
```c
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdlib.h>

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
		exit(1); //malloc failed
	if (read(file, *content, filesize) == ERROR)
		exit(1); //read failed
	(*content)[filesize] = 0;
}

static inline void open_at(int *where, char *filename, int flags)
{
	*where = open(filename, flags);
	if (*where == ERROR)
		exit(1); //open failed
}
```

## usage
```c
#include <stdio.h>

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
```

```bash
cc read.c
./a.out read.c
```
this prints `read.c`
