#ifndef _MYFILESYS_H_
#define _MYFILESYS_H_

typedef struct
{
	struct fs_meta *next; // Pointer to the next metadata
	struct fs_meta *prev; // Pointer to the previous metadata
	char *file_name; // String of file name, NULL if not in use
	int block; // which block we need
	int offset; // the offset of the start of the file inside the block
	int bytes_here; // Number of bytes in this bucket, never to exceed the block boundary
	struct fs_meta * frag_next; // Pointer to the next metadata if fragmented, NULL if not fragmented.
} fs_meta;

typedef struct
{
	fs_meta file; // The file that is open
	int offset; // The current offset.
} open_file;

/********************************************************************************************************/
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

#endif
