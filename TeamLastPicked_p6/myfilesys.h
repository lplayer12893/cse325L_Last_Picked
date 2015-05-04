#ifndef _MYFILESYS_H_
#define _MYFILESYS_H_

#include "disk.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
		char file_name[16]; // String of file name, first character is 0x0 if not in use
		int block; // which block we need
		int offset; // the offset of the start of the file inside the block
		int size; // Size of this file, in bytes. Will always be contiguous
} fs_meta;

typedef struct
{
		int file_num; // The file that is open, array offset
		int offset; // The current offset.
} open_file;

#define DATA_START (DISK_BLOCKS / 2)

/********************************************************************************************************/
char * toString(fs_meta *);
int make_fs(char *disk_name); /* create an empty file system on the virtual disk with name disk_name  */
int mount_fs(char *disk_name); /* mounts a file system stored on a virtual disk with name disk_name    */
int unmount_fs(char *disk_name); /* unmounts the file system from a virtual disk with name disk_name */
int fs_open(char *name); /* the file specified by name is opened for reading and writing, and the    */
/* file descriptor is returned                                          */
int fs_close(int fildes); /* closes the file with the corresponding file descriptor fildes            */
int fs_create(char *name); /* creates a new file with name name                                        */
int fs_delete(char *name); /* deletes the file with name name                                          */
int fs_read(int fildes, void *buf, size_t nbyte); /* Attempts to read nbyte bytes from a given file   */
int fs_write(int fildes, void *buf, size_t nbyte); /* Attempts to write nbyte bytes from a given file  */
int fs_get_filesize(int fildes); /* returns the current size of a given file                         */
int fs_lseek(int fildes, off_t offset); /* sets the file pointer to the argument offset                 */
int fs_truncate(int fildes, off_t length); /* truncates a given file to length bytes in size           */
/********************************************************************************************************/

fs_meta getFile(int i);
open_file getFileDesc(int i);
void printAllFiles();
#endif
