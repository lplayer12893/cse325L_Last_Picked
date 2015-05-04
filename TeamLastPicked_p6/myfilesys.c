#include "myfilesys.h"
#include "disk.c"

open_file open_files[64];
fs_meta * first;

/* create an empty file system on the virtual disk with name disk_name */
int make_fs(char *disk_name){
    if(make_disk(disk_name) != 0)
        return -1;

    if(open_disk(disk_name) != 0)
        return -1;

    
	
	//TODO: initialize/write necessary meta-information for mounting
    return 0;
	//TODO: Returns 0 on success and -1 on failure
}

/* mounts a file system stored on a virtual disk with name disk_name */
int mount_fs(char *disk_name){
    if(open_disk(disk_name) != 0)
        return -1;

	//TODO: load the meta-info for file system operations below
	return 0;
    //TODO: Returns 0 on success and -1 on failure
}

/* unmounts the file system from a virtual disk with name disk_name */
int unmount_fs(char *disk_name){
	//TODO: write back all meta-info with changes

    if(close_disk(disk_name) != 0)
        return -1;

	//TODO: Returns 0 on success and -1 on failure
    return 0;
}

/* the file specified by name is opened for reading and writing, and the file descriptor is returned */
int fs_open(char *name)
{
	// First, find a file descriptor to use.
	int filedesc=0;
	while(filedesc != 64)
	{
		if (open_files[filedesc] == NULL)
			break;
		else
			filedesc++;
	}
	fs_meta * file = first;
	while(1)
	{
		if (strcmp(file->file_name,name) == 0)
		{
			// Found the file
			break;
		}
		else
		{
			if (file->next == NULL)
				break;
		}
	}
	if (file == NULL)
	{
		// Didn't find the file
		printf("Can't find file %s!\n",name);
		return -1;
	}
	open_files[filedesc] = malloc(sizeof(open_file));
	if (open_files[filedesc] == NULL)
	{
		perror("Can't open file (malloc step)");
		return -1;
	}
	open_files[filedesc]->file = file;
	open_files[filedesc]->offset = 0;

	return filedesc;
}

/* closes the file with the corresponding file descriptor fildes */
int fs_close(int fildes)
{
	if (open_files[fildes] == NULL)
	{
		printf("Can't close file %d because it isn't open!\n",fildes);
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
		printf("Tried to create filename %s but it's too long! (15 char maximum).\n",name);
		return -1;
	}
	fs_meta * file = first;
	while(file != NULL)
	{
		if (file->file_name == NULL)
		{
			break;
		}
		else
		{
			file = file->next;
		}
	}
	if (file == NULL)
	{
		// Couldn't find space.
		printf("Can't find space!\n");
		return -1;
	}
	// This is a workable file.
	file->bytes_here = 0;
	// this one doesn't change
	//file->data = name;
	file->file_name = name;
	file->frag_next = NULL;
	file->total_bytes = 0;

	return 0;
}

/* deletes the file with name name */
int fs_delete(char *name)
{
	fs_meta * file = first;
	while(file != NULL)
	{
		if (strcmp(file->file_name,name) == 0)
		{
			// Found the file
			break;
		}
		else
		{
			file = file->next;
		}
	}

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
