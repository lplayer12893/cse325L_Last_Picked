#include "myfilesys.h"
#include "disk.c"

open_file open_files[32];
fs_meta files[64];

void fillBlockAndInt(int i);

/* create an empty file system on the virtual disk with name disk_name */
int make_fs(char *disk_name)
{
	if (make_disk(disk_name) != 0)
		return -1;

	if (open_disk(disk_name) != 0)
		return -1;

	return 0;
}

/* mounts a file system stored on a virtual disk with name disk_name */
int mount_fs(char *disk_name)
{
	if (open_disk(disk_name) != 0)
		return -1;
	char * buffer = malloc(BLOCK_SIZE);
	if (buffer == NULL)
	{
		perror("Cannot mount fs! Malloc error");
		return -1;
	}

	int i = 0;
	while (i < (sizeof(fs_meta) * 64))
	{
		block_read((i % BLOCK_SIZE), buffer);
		memcpy(files + i, buffer, BLOCK_SIZE);
		i += BLOCK_SIZE;
	}
	free(buffer);

	return 0;
}

/* unmounts the file system from a virtual disk with name disk_name */
int unmount_fs(char *disk_name)
{
	int i = 0;
	char * buffer = malloc(BLOCK_SIZE);
	if (buffer == NULL)
	{
		perror("Cannot unmount fs! Malloc error");
		return -1;
	}

	while (i < (sizeof(fs_meta) * 64))
	{
		memcpy(files + i, buffer, BLOCK_SIZE);
		block_write((i % BLOCK_SIZE), buffer);
		i += BLOCK_SIZE;
	}
	free(buffer);

	if (close_disk(disk_name) != 0)
		return -1;

	return 0;
}

/* the file specified by name is opened for reading and writing, and the file descriptor is returned */
int fs_open(char *name)
{
	// First, find a file descriptor to use.
	int filedesc = 0;
	while (filedesc != 32)
	{
		if (open_files[filedesc] == NULL)
			break;
		else
			filedesc++;
	}
	if (filedesc == 32)
	{
		// Too many open files
		printf("Too many open files.");
		return -1;
	}
	int filenum = 0;
	while (filenum < 64)
	{
		if (strcmp(files[filenum], name) == 0)
		{
			// Found the file
			break;
		}
		else
		{
			filenum++;
		}
	}
	if (filenum == 64)
	{
		// Didn't find the file
		printf("Can't find file %s!\n", name);
		return -1;
	}
	open_files[filedesc] = malloc(sizeof(open_file));
	if (open_files[filedesc] == NULL)
	{
		perror("Can't open file (malloc step)");
		return -1;
	}
	open_files[filedesc]->file_num = filenum;
	open_files[filedesc]->offset = 0;

	return filedesc;
}

/* closes the file with the corresponding file descriptor fildes */
int fs_close(int fildes)
{
	if (open_files[fildes] == NULL)
	{
		printf("Can't close file %d because it isn't open!\n", fildes);
		return -1;
	}
	else
	{
		free(open_files[fildes]);
		open_files[fildes] = NULL;
		return 0;
	}
}

/* creates a new file with name name */
int fs_create(char *name)
{
	if (strlen(name) > 15)
	{
		printf("Tried to create filename %s but it's too long! (15 char maximum).\n", name);
		return -1;
	}
	// First, make sure this file name doesn't exist.
	int i = 0;
	for (i = 0; i < 32; i++)
	{
		if (strcmp(files[i]->file_name, name) == 0)
		{
			printf("File exists.\n");
			return -1;
		}
	}
	// Next, find an adequate file descriptor for this file.
	i = 0;
	while (i < 64)
	{
		if (files[i] == NULL)
		{
			break;
		}
		else
		{
			i++;
		}
	}
	if (i == 64)
	{
		// Couldn't find space.
		printf("Too many files!\n");
		return -1;
	}
	// This is a workable file.
	int block, offset;
	fillBlockAndInt(i);

	files[i]->size = 0;
	return 0;
}

/* deletes the file with name name */
int fs_delete(char *name)
{
	int i = 0;
	while (i < 64)
	{
		if (strcmp(files[i]->file_name, name) == 0)
		{
			// Found the file
			// Make sure it isn't open.
			for (int j = 0; j < 32; j++)
			{
				if (open_files[j]->file_num == i)
				{
					printf("Cannot delete file: it is open in file descriptor %d!\n", j);
					return -1;
				}
			}
			files[i]->file_name = 0x0;
			return 0;
		}
		else
		{
			i++;
		}
	}
	printf("Cannot delete file: cannot find it!\n");
	return -1;
}

/* Attempts to read nbyte bytes from a given file */
int fs_read(int fildes, void *buf, size_t nbyte)
{
	//TODO: read nbyte bytes from file into buf. Function assumes *buf is big enough
	//TODO: stop reading at eof
	//TODO: implicitly increment the file pointer by bytes read
	//TODO: Returns number of bytes read on success and -1 on failure
}

/* Attempts to write nbyte bytes from a given file */
int fs_write(int fildes, void *buf, size_t nbyte)
{
	//TODO: write nbyte bytes of data to the referenced file from buf. buf size > nbytes assumed
	//TODO: handle dynamically expanding file size
	//TODO: if not enough disk space, write as much as possible
	//TODO: implicitly increment the file pointer by bytes written
	//TODO: Returns number of bytes written on success, -1 on failure. 0 on full disk
}

/* returns the current size of a given file */
int fs_get_filesize(int fildes)
{
	//TODO: returns file size on success, -1 on failure
}

/* sets the file pointer to the argument offset */
int fs_lseek(int fildes, off_t offset)
{
	//TODO: set the file pointer to the given offset
	//TODO: implement error catch if set beyond eof
	//TODO: Returns 0 on success and -1 on failure
}

/* truncates a given file to length bytes in size */
int fs_truncate(int fildes, off_t length)
{
	//TODO: truncate file fildes to length bytes
	//TODO: free truncated blocks
	//TODO: constraint - truncate cannot extend a file
	//TODO: Returns 0 on success and -1 on failure
}

/**
 * Sets the block size and offset for a new file, given the last file.
 */
void fillBlockAndInt(int i)
{
	if (i == 0)
	{
		files[i]->block = 0;
		files[i]->offset = 0;
	}
	else
	{
		files[i]->block = files[i - 1]->block;
		files[i]->offset = files[i - 1]->offset;
		files[i]->offset += files[i - 1]->size;
		while (files[i]->offset > BLOCK_SIZE)
		{
			files[i]->offset -= BLOCK_SIZE;
			files[i]->block++;
		}
	}

}
