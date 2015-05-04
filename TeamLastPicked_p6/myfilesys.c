#include "myfilesys.h"
#include "disk.c"

/* create an empty file system on the virtual disk with name disk_name */
int make_fs(char *disk_name){
	//TODO: invoke make_disk(disk_name)
	//TODO: open this disk
	//TODO: initialize/write necessary meta-information for mounting
	//TODO: Returns 0 on success and -1 on failure
}

/* mounts a file system stored on a virtual disk with name disk_name */
int mount_fs(char *disk_name){
	//TODO: open the disk
	//TODO: load the meta-info for file system operations below
	//TODO: Returns 0 on success and -1 on failure
}

/* unmounts the file system from a virtual disk with name disk_name */
int unmount_fs(char *disk_name){
	//TODO: write back all meta-info with changes
	//TODO: close disk
	//TODO: Returns 0 on success and -1 on failure
}

/* the file specified by name is opened for reading and writing, and the file descriptor is returned */
int fs_open(char *name){
	//TODO: open file for r/w
	//TODO: get file descriptor
	//TODO: set file offset/seek pointer to start of file
	//TODO: implement storage for up to 32 file descriptors
	//TODO: Returns non-negative fildes on success and -1 on failure
}

/* closes the file with the corresponding file descriptor fildes */
int fs_close(int fildes){
	//TODO: close file filedes
    //TODO: Returns 0 on success and -1 on failure
}

/* creates a new file with name name */
int fs_create(char *name){
    //TODO: create file in fs
    //TODO: constrain file name to 15 characters
    //TODO: constrain file limit to 64 files
    //TODO: Returns 0 on success and -1 on failure
}

/* deletes the file with name name */
int fs_delete(char *name){
    //TODO: delete file in fs
    //TODO: remove file's meta-info
    //TODO: constraint - file must be closed to be deleted (no fildes to file exists)
    //TODO: Returns 0 on success and -1 on failure
}

/* Attempts to read nbyte bytes from a given file */
int fs_read(int fildes, void *buf, size_t nbyte){
    //TODO: read nbyte bytes from file into buf. Function assumes *buf is big enough
    //TODO: stop reading at eof
    //TODO: implicitly increment the file pointer by bytes read
    //TODO: Returns number of bytes read on success and -1 on failure
}

/* Attempts to write nbyte bytes from a given file */
int fs_write(int fildes, void *buf, size_t nbyte){
    //TODO: write nbyte bytes of data to the referenced file from buf. buf size > nbytes assumed
    //TODO: handle dynamically expanding file size
    //TODO: if not enough disk space, write as much as possible
    //TODO: implicitly increment the file pointer by bytes written
    //TODO: Returns number of bytes written on success, -1 on failure. 0 on full disk
}

/* returns the current size of a given file */
int fs_get_filesize(int fildes){
    //TODO: returns file size on success, -1 on failure
}

/* sets the file pointer to the argument offset */
int fs_lseek(int fildes, off_t offset){
    //TODO: set the file pointer to the given offset
    //TODO: implement error catch if set beyond eof
    //TODO: Returns 0 on success and -1 on failure
}

/* truncates a given file to length bytes in size */
int fs_truncate(int fildes, off_t length){
    //TODO: truncate file fildes to length bytes
    //TODO: free truncated blocks
    //TODO: constraint - truncate cannot extend a file
    //TODO: Returns 0 on success and -1 on failure
}
