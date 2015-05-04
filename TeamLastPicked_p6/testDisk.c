#include "myfilesys.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void error(char * str, ...);
void success(char * str, ...);

int main(int argc, char ** argv)
{
	// This is where we add our own tests. Make will automatically run these. 
	// To error out, just call the error function.

	// Check our test file system.
	if (make_fs("test.fs") == -1)
		error("Cannot make file system!");
	success("make_fs");
	if (mount_fs("test.fs") == -1)
		error("Cannot mount file system!");
	success("mount_fs");
	if (unmount_fs("test.fs") == -1)
		error("Cannot unmount file system!");
	success("unmount_fs");

	// Next, make sure all files are empty.
	int i = 0;
	for (i = 0; i < 64; i++)
	{
		fs_meta file = getFile(i);
		if (file.block != 0)
			error("File %d does not have block set to 0", i);
		if (file.file_name[0] != 0x0)
			error("File %d does not have name set to 0x0 (set to %s)", i, file.file_name);
		if (file.offset != 0)
			error("File %d does not have offset set to 0", i);
		if (file.size != 0)
			error("File %d does not have size set to 0 (set to %d)", i, file.size);
	}
	success("initial file system cleared, %d files checked", i);

	// Next, make a file.
	mount_fs("test.fs");
	// Next, make sure all files are empty.
	fs_meta file = getFile(0);
	i = 0;
	for (i = 0; i < 64; i++)
	{
		file = getFile(i);
		if (file.block != 0)
			error("File %d does not have block set to 0", i);
		if (file.file_name[0] != 0x0)
			error("File %d does not have name set to 0x0 (set to %s)", i, file.file_name);
		if (file.offset != 0)
			error("File %d does not have offset set to 0", i);
		if (file.size != -1)
			error("File %d does not have size set to -1 (set to %d)", i, file.size);
	}
	success("initial file system re-opened, %d files checked", i);

	if (fs_create("test1") == -1)
		error("Cannot create file test1.");
	file = getFile(0);
	if (file.block != DATA_START)
		error("First file doesn't have right block");
	if (file.offset != 0)
		error("First file doesn't have right offset");
	success("fs_create");

	// Open the file.
	int filedes = fs_open("test1");
	if (filedes == -1)
		error("Cannot open file test1");
	success("fs_open");

	// Close the file
	if (fs_close(filedes) == -1)
		error("Cannot close file test1");
	success("fs_close");

	// Check to make sure we can close and open the disk, and the file stuff stays.
	if (unmount_fs("test.fs") == -1)
		error("Cannot unmount fs 2nd time");
	if (mount_fs("test.fs") == -1)
		error("Cannot mount fs 2nd time");
	filedes = fs_open("test1");
	if (filedes == -1)
		error("Cannot open file test1 after remount");
	success("remount file persistence");

	// Write to the file
	int num_written = fs_write(filedes, "abcd", 5);
	if (num_written != 5)
		error("Cannot write 4 bytes to file test1 (wrote %d bytes)", num_written);
	file = getFile(0);
	if (file.size != 5)
		error("Size is wrong, %d but it should be 5", file.size);
	if (getFileDesc(filedes).offset != 5)
		error("Offset is wrong after write (%d, but it should be 5)", getFileDesc(filedes).offset);
	success("wrote to file");

	// Check lseek
	if (fs_lseek(filedes, 0) == -1)
		error("Cannot fs_lseek to 0");
	if (getFileDesc(filedes).offset != 0)
		error("Offset is wrong after fs_lseek1 (%d, should be 0)", getFileDesc(filedes).offset);
	if (fs_lseek(filedes, 1) == -1)
		error("Cannot fs_lseek to 1");
	if (getFileDesc(filedes).offset != 1)
		error("Offset is wrong after fs_lseek2 (%d, should be 1)", getFileDesc(filedes).offset);
	if (fs_lseek(filedes, 2) == -1)
		error("Cannot fs_lseek 2 more");
	if (getFileDesc(filedes).offset != 2)
		error("Offset is wrong after fs_lseek3 (%d, should be 2)", getFileDesc(filedes).offset);
	if (fs_lseek(filedes, 5) == -1)
		error("Cannot lseek to end of file!");
	if (getFileDesc(filedes).offset != 5)
		error("Offset is wrong after fs_lseek4 to end of file (%d, should be 5)", getFileDesc(filedes).offset);
	if (fs_lseek(filedes, 6) != -1)
		error("fs_lseek allowed us to go past end of file!");
	if (getFileDesc(filedes).offset != 5)
		error("Offset is wrong after improper fs_lseek5 (%d, should be 5)", getFileDesc(filedes).offset);
	if (fs_lseek(filedes+1,0) != -1)
		error("fs_lseek allowed us to seek a non-open file!");
	success("fs_lseek");

	// Read from the file, check to make sure it is ok.
	char * test = malloc(5);
	fs_lseek(filedes,0);
	if (fs_read(filedes, test, 5) != 5)
		error("Cannot read 5 bytes from filedes test1");
	success("read from filedes");
	if (strcmp(test, "abcd") != 0)
		error("4 bytes from file test 1 are not right!");
	success("contents correct");

	// Unmount filesystem, remount, read file.
	if (unmount_fs("test.fs") == -1)
		error("Cannot unmount fs 3rd time");
	if (mount_fs("test.fs") == -1)
		error("Cannot remount fs 3rd time");
	filedes = fs_open("test1");
	if (filedes == -1)
		error("Cannot open file 3rd time");
	if (fs_read(filedes, test, 5) != 5)
		error("Cannot read file 3rd time");
	if (strcmp(test, "abcd") != 0)
		error("File contents don't match after remount. (>%s<, should be >abcd<", test);
	success("File persistence");

	// Overwrite the file
	if (fs_lseek(filedes, 0) == -1)
		error("Cannot fs_lseek 2nd time!");
	num_written = fs_write(filedes, "efgh", 5);
	if (num_written != 5)
		error("Cannot overwrite file, num_written = %d, should be 5", num_written);
	if (fs_lseek(filedes, 0) == -1)
		error("Cannot fs_lseek 3rd time!");
	if (fs_read(filedes, test, 5) != 5)
		error("Cannot read 5 bytes from filedes test1");
	if (strcmp(test, "efgh") != 0)
		error("4 bytes from file test1 are not right after overwrite!");
	success("overwrite successful, %d bytes were overwritten and compared", 5);

	// Create a 2nd file descriptor pointing to the first file, and check read independence.
	fs_lseek(filedes, 0);
	int filedes2 = fs_open("test1");
	if (filedes2 == -1)
		error("Cannot open 2nd file descriptor");
	if (filedes == filedes2)
		error("File descriptors are the same!");
	char * test2 = malloc(5);
	int numread, numread2;

	if ((numread = fs_read(filedes, test, 2)) != 2)
		error("read1 read %d bytes, should be 2", numread);
	test[2] = 0x0;
	if (fs_lseek(filedes2, 2) != 0)
		error("couldn't lseek 2 bytes");
	if ((numread2 = fs_read(filedes2, test2, 3)) != 3)
		error("read2 read %d bytes, should be 3", numread2);

	if (strcmp(test, "ef") != 0)
		error("test should be >ef<, but it's >%s<", test);
	if (strcmp(test2, "gh") != 0)
		error("test2 should be >gh< but it's >%s<", test2);
	success("2 file descriptor and independence");

	return 0;
}

/**
 * Prints an error and stops the makefile.
 * @param str string describing the error.
 */
void error(char * str, ...)
{
	va_list arglist;
	fprintf(stderr, "\033[22;41;30mERROR:\033[0m ");
	va_start(arglist, str);
	vfprintf(stderr, str, arglist);
	va_end(arglist);
	fprintf(stderr, "\n");
	exit(1);
}

/**
 * Prints a success message to keep track of where you are.
 * @param str string describing the test.
 */
void success(char * str, ...)
{
	va_list arglist;
	printf("\033[22;00;32m   OK: \033[0m ");
	va_start(arglist, str);
	vprintf(str, arglist);
	va_end(arglist);
	printf("\n");
}
