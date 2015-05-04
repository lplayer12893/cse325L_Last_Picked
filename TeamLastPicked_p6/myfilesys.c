#include "myfilesys.h"

open_file open_files[32];
fs_meta files[64];

void fillBlockAndOffset(int i);
void shiftBlocks(int startBlock, int startOffset, int numShift);

/**
 * Creates a new filesystem
 * @tested
 * @param disk_name name of the file to write our virtual filesystem on top of
 * @return -1 on failure, 0 on success.
 */
int make_fs(char *disk_name)
{
	// First, call make disk.
	if (make_disk(disk_name) != 0)
		return -1;

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

/**
 * Mounts a virtual filesystem
 * @tested
 * @param disk_name name of file that contains our disk
 * @return 0 on success, -1 on failure
 */
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

/**
 * Unmounts a filesystem. Writes all metadata
 * @tested
 * @param disk_name file name that our virtual disk sits on
 * @return 0 on success, -1 on failure
 */
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

/**
 * opens a file by name
 * @tested
 * @param name name of file to open, must be < 15 characters
 * @return file descriptor to that file with offset set to 0, -1 on failure
 */
int fs_open(char *name)
{
	// First, check to make sure file name isn't too long
	if (strlen(name) > 15)
	{
		return -1;
	}

	// Next, find a file descriptor to use.
	int filedesc = 0;
	while (filedesc < 32)
	{
		if (open_files[filedesc].file_num == -1)
			break;
		else
			filedesc++;
	}
	if (filedesc == 32)
	{
		// Too many open files
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
		// printf("Couldn't find file.\n");
		// printAllFiles();
		return -1;
	}

	// Open the file, and set the offset to 0.
	open_files[filedesc].file_num = filenum;
	open_files[filedesc].offset = 0;

	return filedesc;
}

/**
 * Closes a file
 * @tested
 * @param fildes file descriptor to close
 * @return 0 on success, -1 on failure
 */
int fs_close(int fildes)
{
	// Check to make sure it's open
	if (open_files[fildes].file_num == -1)
	{
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
		return -1;
	}
	// First, make sure this file name doesn't exist.
	int i = 0;
	for (i = 0; i < 32; i++)
	{
		if (strcmp(files[i].file_name, name) == 0)
		{
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
		return -1;
	}
	// This is a workable file.
	fillBlockAndOffset(i);

	files[i].size = 0;
	files[i + 1].size = -1;
	memcpy(files[i].file_name, name, strlen(name) + 1);
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
					// printf("Cannot delete file: it is open in file descriptor %d!\n", j);
					return -1;
				}
			}
			// Delete the file
			// First, shift the actual data
			shiftBlocks(files[i].block, files[i].offset, files[i].size);
			// Next, shift the remaining open file descriptors
			for (j = 0; j < 32; j++)
			{
				if (open_files[j].file_num > i)
				{
					open_files[j].file_num--;
				}
			}
			// Finally, shift the remaining fs_metas
			for (j = i; j < 63; j++)
			{
				// printf("Copying entry %d to %d\n", j + 1, j);
				files[j].block = files[j + 1].block;
				memcpy(files[j].file_name, files[j + 1].file_name, 16);
				files[j].offset = files[j + 1].offset;
				files[j].size = files[j + 1].size;
			}
			files[63].file_name[0] = 0x0;
			files[63].size = -1;

			// Finally, shift all data blocks to conform to right sizes.
			for (j = i; j < 64; j++)
			{
				fillBlockAndOffset(j);
			}
			return 0;
		}
		else
		{
			i++;
		}
	}
	// printf("Cannot delete file: cannot find it!\n");
	return -1;
}

/* Attempts to read nbyte bytes from a given file */
int fs_read(int fildes, void *buf, size_t nbyte)
{
	int num_read = 0;
	open_file *filedes = &(open_files[fildes]);
	fs_meta actual_file = files[filedes->file_num];

	if (nbyte > actual_file.size - filedes->offset)
	{
		nbyte = actual_file.size - filedes->offset;
		// printf("nbyte read is bigger than file. setting to %d\n", nbyte);
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
			// block_write(block, buffer);
			block++;
			block_read(block, buffer);
		}
		memcpy(buf + num_read, buffer + pos, 1);
		// printf("Copying into buf[%d] from block %d position %d (byte copied is 0x%02x, '%c')\n", num_read, block, pos,*((char *)(buf+num_read)),((char*)(buf+num_read))[0]);
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
			if (files[filedes->file_num + 1].size == -1)
			{
				// Next block is empty, just expand;
			}
			else
			{
				printf("LOL Dynamically expanding file. Size of my file is %d, next file is %d\n", actual_file->size, files[filedes->file_num + 1].size);
				return -1;
			}
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
		// printf("Writing byte %d to block %d position %d (%02x)\n", num_written, block, pos,*(char*)(buf+num_written));
		memcpy(buffer + pos, buf + num_written, 1);
		num_written++;
		pos++;
	}
	filedes->offset += num_written;
	block_write(block, buffer);
	free(buffer);
	buffer = NULL;
	if (actual_file->size < filedes->offset)
		actual_file->size = filedes->offset;
	// printf("actual_file: name: %s, size %d\n", actual_file->file_name, actual_file->size);
	return num_written;

	return -1;
}

/**
 * returns the current size of a given file
 * @tested
 * @param fildes descriptor to use
 * @return -1 on fail, file size otherwise
 */
int fs_get_filesize(int fildes)
{
	if (open_files[fildes].file_num == -1)
	{
		return -1;
	}
	else
		return files[open_files[fildes].file_num].size;
}

/**
 * sets the file pointer to the argument offset
 * @tested
 * @param fildes descriptor
 * @param offset absolute offset to seek to
 */
int fs_lseek(int fildes, off_t offset)
{
	if (open_files[fildes].file_num == -1)
	{
		return -1;
	}
	else
	{
		if (offset > files[open_files[fildes].file_num].size)
		{
			// printf("Tried to seek past end of file!");
			return -1;
		}
		else
		{
			open_files[fildes].offset = offset;
			return 0;
		}
	}
}

/**
 * truncates a given file to length bytes in size
 * @tested
 * @param fildes fildes to truncate
 * @param length new length
 * @return 0 if successful, -1 if not.
 */
int fs_truncate(int fildes, off_t length)
{
	if (open_files[fildes].file_num == -1)
	{
		// printf("Cannot truncate a file that isn't open.\n");
		return -1;
	}
	if (files[open_files[fildes].file_num].size < length)
	{
		// printf("Cannot truncate a file to make it bigger! Old size: %d, requested size: %d\n", files[open_files[fildes].file_num].size, (int) length);
		return -1;
	}
	if (length < 0)
	{
		// printf("Cannot truncate a file to negative size!");
		return -1;
	}
	else
	{
		int startBlock = files[open_files[fildes].file_num].block;
		int startOffset = files[open_files[fildes].file_num].offset + length;
		while (startOffset > BLOCK_SIZE)
		{
			startBlock++;
			startOffset -= BLOCK_SIZE;
		}

		shiftBlocks(startBlock, startOffset, length-1);
		files[open_files[fildes].file_num].size = length;
		int i = fildes;
		for (i = fildes; i < 63; i++)
			fillBlockAndOffset(i);
		if (open_files[fildes].offset > length)
			open_files[fildes].offset = length;
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
		if (files[i - 1].size == -1)
			files[i].offset = files[i - 1].offset;
		else
			files[i].offset += files[i - 1].size;
		while (files[i].offset > BLOCK_SIZE)
		{
			files[i].offset -= BLOCK_SIZE;
			files[i].block++;
		}
	}

}

void shiftBlocks(int startBlock, int startOffset, int numShift)
{
	// printf("shiftBlocks called with %d,%d,%d\n",startBlock,startOffset,numShift);
	if (numShift == 0)
		return;
	int block1 = startBlock;
	int offset1 = startOffset;
	int block2 = startBlock;
	int offset2 = startOffset + numShift;
	char *buffer1 = NULL, *buffer2 = NULL;

	buffer1 = malloc(BLOCK_SIZE);
	buffer2 = malloc(BLOCK_SIZE);

	if ((buffer1 == NULL) || (buffer2 == NULL))
	{
		perror("Can't shift blocks");
		return;
	}

	while (offset2 >= BLOCK_SIZE)
	{
		block2++;
		offset2 -= BLOCK_SIZE;
	}

	// printf("reading blocks %d and %d\n",block1,block2);
	block_read(block1, buffer1);
	block_read(block2, buffer2);
	int num_copy = 0;
	while (block2 < DISK_BLOCKS)
	{
		// Figure out how much we need to copy.
		if (offset1 > offset2)
		{
			num_copy = BLOCK_SIZE - offset1;
		}
		else
		{
			num_copy = BLOCK_SIZE - offset2;
		}
		// printf("copying %d bytes: %d.%d -> %d.%d\n",num_copy,block2,offset2,block1,offset1);
		// Copy the memory
		memcpy(buffer1 + offset1, buffer2 + offset2, num_copy);

		// Advance the offsets
		offset1 += num_copy;
		offset2 += num_copy;
		// See if we need to advance a block
		if (offset1 == BLOCK_SIZE)
		{
			block_write(block1, buffer1);
			block1++;
			offset1 = 0;
			block_read(block1, buffer1);
		}
		if (offset2 == BLOCK_SIZE)
		{
			block_write(block2, buffer2);
			block2++;
			offset2 = 0;
			if (block2 < DISK_BLOCKS)
				block_read(block2, buffer2);
		}
	}
	// We reached the end, so zero out what's left.
	memset(buffer1 + offset1, 0x0, BLOCK_SIZE - offset1);
	block_write(block1, buffer1);
	block1++;
	memset(buffer1, 0x0, BLOCK_SIZE);
	while (block1 < DISK_BLOCKS)
	{
		block_write(block1, buffer1);
		block1++;
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

void printAllFiles()
{
	int i = 0, j = 0;
	int empties = 0;
	for (i = 0; i < 64; i++)
	{
		if ((files[i].file_name[0] == 0x0) && (files[i].size == -1))
		{
			// Empty file
			empties++;
		}
		else
		{
			printf("File %d: %s (", i, files[i].file_name);
			for (j = 0; j < 16; j++)
				printf("%02x ", files[i].file_name[j]);
			printf(")\n");
			printf("\tBlock %d, Offset %d\n", files[i].block, files[i].offset);
			printf("\tSize: %d\n", files[i].size);
		}

	}
	printf("%d empty files.\n", empties);
}
