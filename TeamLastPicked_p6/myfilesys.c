#include "myfilesys.h"

open_file open_files[32];
fs_meta files[64];

void fillBlockAndOffset(int i);

/* create an empty file system on the virtual disk with name disk_name */
int make_fs(char *disk_name)
{
	// First, call make disk.
	if (make_disk(disk_name) != 0)
		return -1;

//	if (open_disk(disk_name) != 0)
//		return -1;

// Mount it so we can write the metadata
	mount_fs(disk_name);

	int i = 0;
	for (i = 0; i < 64; i++)
	{
		files[i].file_name[0] = 0x0;
		files[i].block = 0;
		files[i].offset = 0;
		files[i].size = -1;
	}

	// Unmount it so it saves.
	unmount_fs(disk_name);

	return 0;
}

/* mounts a file system stored on a virtual disk with name disk_name */
int mount_fs(char *disk_name)
{
	// Open the disk
	if (open_disk(disk_name) != 0)
		return -1;
	char * buffer = malloc(BLOCK_SIZE);
	if (buffer == NULL)
	{
		perror("Cannot mount fs! Malloc error");
		return -1;
	}
	// Copy our structure from disk into memory
	int i = 0;
	block_read((i % BLOCK_SIZE), buffer);
	memcpy(files, buffer, sizeof(fs_meta) * 64);

	free(buffer);

	for (i = 0; i < 32; i++)
	{
		open_files[i].file_num = -1;
	}

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

	// Copy our structure into the buffer
	memcpy(buffer, files, sizeof(fs_meta) * 64);
	// Write it to disk
	block_write(0, buffer);
	free(buffer);

	// Close all open files.
	for (i = 0; i < 32; i++)
	{
		open_files[i].file_num = -1;
	}

	// Remove all entries in the files table
	memset(files, 0x0, sizeof(fs_meta) * 64);

	// Close disk
	if (close_disk(disk_name) != 0)
		return -1;

	return 0;
}

/* the file specified by name is opened for reading and writing, and the file descriptor is returned */
int fs_open(char *name)
{
	// First, find a file descriptor to use.
	int filedesc = 0;
	while (filedesc <= 32)
	{
		if (open_files[filedesc].file_num == -1)
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
		if (strcmp(files[filenum].file_name, name) == 0)
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

	// Open the file, and set the offset to 0.
	open_files[filedesc].file_num = filenum;
	open_files[filedesc].offset = 0;

	return filedesc;
}

/* closes the file with the corresponding file descriptor fildes */
int fs_close(int fildes)
{
	// Check to make sure it's open
	if (open_files[fildes].file_num == -1)
	{
		printf("Can't close file %d because it isn't open!\n", fildes);
		return -1;
	}
	else
	{
		// Close it
		open_files[fildes].file_num = -1;
		return 0;
	}
}

/* creates a new file with name name */
int fs_create(char *name)
{
	// Check file name
	if (strlen(name) > 15)
	{
		printf("Tried to create filename %s but it's too long! (15 char maximum).\n", name);
		return -1;
	}
	// First, make sure this file name doesn't exist.
	int i = 0;
	for (i = 0; i < 32; i++)
	{
		if (strcmp(files[i].file_name, name) == 0)
		{
			printf("File exists.\n");
			return -1;
		}
	}
	// Next, find an adequate file descriptor for this file.
	i = 0;
	while (i < 64)
	{
		if (files[i].size == -1)
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
	fillBlockAndOffset(i);

	files[i].size = -1;
	memcpy(files[i].file_name, name, strlen(name));
	return 0;
}

/* deletes the file with name name */
int fs_delete(char *name)
{
	int i = 0;
	while (i < 64)
	{
		if (strcmp(files[i].file_name, name) == 0)
		{
			// Found the file
			// Make sure it isn't open.
			int j = 0;
			for (j = 0; j < 32; j++)
			{
				if (open_files[j].file_num == i)
				{
					printf("Cannot delete file: it is open in file descriptor %d!\n", j);
					return -1;
				}
			}
			// Delete the file
			// First, open it, truncate, then close.
			int des = fs_open(name);
			fs_truncate(des, 0);
			fs_close(des);
			files[i].file_name[0] = 0x0;
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
	int num_read = 0;
	open_file *filedes = &(open_files[fildes]);
	fs_meta actual_file = files[filedes->file_num];

	if (nbyte > actual_file.size + filedes->offset)
	{
		nbyte = actual_file.size - filedes->offset;
		printf("nbyte read is bigger than file. setting to %d", nbyte);
	}
	int block = actual_file.block;
	int pos = actual_file.offset + filedes->offset;

	while (pos > BLOCK_SIZE)
	{
		block++;
		pos -= BLOCK_SIZE;
	}
	char * buffer = malloc(BLOCK_SIZE);
	if (buffer == NULL)
	{
		perror("Cannot read (malloc step)");
		return -1;
	}
	block_read(block, buffer);

	while (num_read < nbyte)
	{
		if (pos == BLOCK_SIZE)
		{
			pos = 0;
			block_write(block, buffer);
			block++;
			block_read(block, buffer);
		}
		printf("Copying byte %d from block %d position %d\n", num_read, block, pos);
		memcpy(buf + num_read, buffer + pos, 1);
		num_read++;
		pos++;
	}
	filedes->offset += num_read;
	free(buffer);
	buffer = NULL;
	return num_read;
}

/* Attempts to write nbyte bytes from a given file */
int fs_write(int fildes, void *buf, size_t nbyte)
{
	open_file *filedes = &(open_files[fildes]);
	fs_meta *actual_file = &(files[filedes->file_num]);
	int pos = actual_file->offset + filedes->offset;
	int block = actual_file->block;
	int num_written = 0;
	char * buffer = malloc(BLOCK_SIZE);
	if (buffer == NULL)
	{
		perror("Can't write (malloc step)");
		return -1;
	}
	if (actual_file->size != -1)
	{
		if (filedes->offset + nbyte > actual_file->size)
		{
			//TODO: handle dynamically expanding file size
			// Too much writing, truncate and make new.
			printf("LOL Dynamically expanding file");
			return -1;
		}
	}
	while (pos >= BLOCK_SIZE)
	{
		block++;
		pos -= BLOCK_SIZE;
	}
	while (num_written < nbyte)
	{
		if (pos == BLOCK_SIZE)
		{
			pos = 0;
			block_write(block, buffer);
			block++;
			block_read(block, buffer);
		}
		printf("Writing byte %d to block %d position %d\n", num_written, block, pos);
		memcpy(buffer + pos, buf + num_written, 1);
		num_written++;
		pos++;
	}
	filedes->offset += num_written;
	block_write(block, buffer);
	free(buffer);
	buffer = NULL;
	actual_file->size = num_written;
	printf("actual_file: name: %s, size %d\n", actual_file->file_name, actual_file->size);
	return num_written;

	return -1;
}

/* returns the current size of a given file */
int fs_get_filesize(int fildes)
{
	if (open_files[fildes].file_num == -1)
	{
		printf("Cannot get file size! It hasn't been open.\n");
		return -1;
	}
	else
		return files[open_files[fildes].file_num].size;
}

/* sets the file pointer to the argument offset */
int fs_lseek(int fildes, off_t offset)
{
	if (open_files[fildes].file_num == -1)
	{
		printf("Tried to seek on file, but it wasn't open!");
		return -1;
	}
	else
	{
		if (offset > files[open_files[fildes].file_num].size)
		{
			printf("Tried to seek past end of file!");
			return -1;
		}
		else
		{
			open_files[fildes].offset = offset;
			return 0;
		}
	}
}

/* truncates a given file to length bytes in size */
int fs_truncate(int fildes, off_t length)
{
	if (open_files[fildes].file_num == -1)
	{
		printf("Cannot truncate a file that isn't open.\n");
		return -1;
	}
	if (files[open_files[fildes].file_num].size < length)
	{
		printf("Cannot truncate a file to make it bigger! Old size: %d, requested size: %d\n", files[open_files[fildes].file_num].size, (int) length);
		return -1;
	}
	else
	{
		char * block1 = malloc(BLOCK_SIZE);
		char * block2 = malloc(BLOCK_SIZE);
		if ((block1 == NULL) || (block2 == NULL))
		{
			perror("Cannot truncate on malloc step");
			return -1;
		}
		fs_meta file = files[open_files[fildes].file_num];

		int i = open_files[fildes].file_num + 1;

		// Move the actual data
		int num_move = file.size - length;
		block_read(file.block, block1);
		memcpy(block1 + file.offset + length, block1 + file.offset + file.size, BLOCK_SIZE - (file.offset + file.size));
		memcpy(block1 + (BLOCK_SIZE - num_move), block2, num_move);
		block_write(file.block, block1);
		while (i < DISK_BLOCKS - 1)
		{
			block_read(i, block1);
			block_read(i + 1, block2);
			memcpy(block1, block1 + num_move, BLOCK_SIZE - num_move);
			memcpy(block1 + num_move, block2, num_move);
			block_write(i, block1);
		}
		block_read(i, block1);
		memcpy(block1, block1 + num_move, BLOCK_SIZE - num_move);
		block_write(i, block1);

		file.size = length;
		while (i < 64)
		{
			// Reset the block sizes
			if (files[i].size != -1)
			{
				fillBlockAndOffset(i);
			}
			else
				break;
		}
		return 0;
	}
}

/**
 * Sets the block size and offset for a new file, given the last file.
 */
void fillBlockAndOffset(int i)
{
	if (i == 0)
	{
		files[i].block = DATA_START;
		files[i].offset = 0;
	}
	else
	{
		files[i].block = files[i - 1].block;
		files[i].offset = files[i - 1].offset;
		files[i].offset += files[i - 1].size;
		while (files[i].offset > BLOCK_SIZE)
		{
			files[i].offset -= BLOCK_SIZE;
			files[i].block++;
		}
	}

}

fs_meta getFile(int i)
{
	return files[i];
}

open_file getFileDesc(int i)
{
	return open_files[i];
}
