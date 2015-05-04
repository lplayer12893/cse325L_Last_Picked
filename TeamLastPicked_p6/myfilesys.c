#include "myfilesys.h"
#include "disk.c"

/* create an empty file system on the virtual disk with name disk_name */
int make_fs(char *disk_name){
}

/* mounts a file system stored on a virtual disk with name disk_name */
int mount_fs(char *disk_name){
}

/* unmounts the file system from a virtual disk with name disk_name */
int unmount_fs(char *disk_name){
}

/* the file specified by name is opened for reading and writing, and the file descriptor is returned */
int fs_open(char *name){
}

/* closes the file with the corresponding file descriptor fildes */
int fs_close(int fildes){
}

/* creates a new file with name name */
int fs_create(char *name){
}

/* deletes the file with name name */
int fs_delete(char *name){
}

/* Attempts to read nbyte bytes from a given file */
int fs_read(int fildes, void *buf, size_t nbyte){
}

/* Attempts to write nbyte bytes from a given file */
int fs_write(int fildes, void *buf, size_t nbyte){
}

/* returns the current size of a given file */
int fs_get_filesize(int fildes){
}

/* sets the file pointer to the argument offset */
int fs_lseek(int fildes, off_t offset){
}

/* truncates a given file to length bytes in size */
int fs_truncate(int fildes, off_t length){
}
