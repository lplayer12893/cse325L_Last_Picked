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

	int i = 0;
	int files[40];
	fs_meta file;
	// First, check to see if it exists already, so we don't burn a hard drive out.

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
	file = getFile(0);
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

	// Try to create a file with name too big
	if (fs_create("1234567890123456") != -1)
		error("Cannot create file with 15 character filename");
	success("Too Long of file name");

	// Try to create duplicate file
	if (fs_create("test1") != -1)
		error("fs_create allowed duplicate file names");
	success("Duplicate files");

	// Try and create too many files
	char * fname = malloc(7);
	for (i = 0; i < 70; i++)
	{

		sprintf(fname, "file%d", i);
		if (fs_create(fname) == -1)
		{
			if (i < 63)
				error("fs_create couldn't make file %d", i);
		}
		else
		{
			if (i >= 63)
				error("fs_create allowed too many files!");
		}
	}
	success("File count");

	// Delete all the files we made
	for (i = 0; i < 63; i++)
	{
		sprintf(fname, "file%d", i);
		if (fs_delete(fname) == -1)
		{
			error("fs_delete couldn't delete file %d", i);
		}
	}
	printAllFiles();
	// Try to open too many files
	for (i = 0; i < 40; i++)
	{
		files[i] = fs_open("test1");
		if (files[i] == -1)
		{
			if (i < 32)
				error("Couldn't open file descriptor #%d.", i);
		}
		else
		{
			if (i >= 32)
				error("fs_open allowed too many files to be opened!");
		}
	}
	success("fs_open, too many files");

	for (i = 0; i < 40; i++)
	{
		files[i] = fs_close(files[i]);
		if (files[i] != 0)
		{
			if (i <= 32)
				error("Couldn't close file descriptor #%d.", i);

		}
	}
	success("fs_close, %d files.", i);

	// Try to find a file that doesn't exist.
	files[0] = fs_open("asdf");
	if (files[0] != -1)
		error("fs_open found a file that didn't exist (number %d)", files[0]);
	success("fs_open file not found");

	// Try to find a file that is named too long
	files[0] = fs_open("1234567890123456");
	if (files[0] != -1)
		error("fs_open tried looking for a file that was too long.");
	success("fs_open file too long");

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
		error("Size is wrong in meta info, %d but it should be 5", file.size);
	if (getFileDesc(filedes).offset != 5)
		error("Offset is wrong after write (%d, but it should be 5)", getFileDesc(filedes).offset);
	success("wrote to file");

	// Check filesize
	if (fs_get_filesize(filedes) != 5)
		error("Size is wrong in function, %d, should be 5", fs_get_filesize(filedes));
	if (fs_get_filesize(filedes + 1) != -1)
		error("Got size of %d from filesize of non-open file (should be -1)", fs_get_filesize(filedes + 1));
	success("fs_get_filesize");

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
	if (fs_lseek(filedes + 1, 0) != -1)
		error("fs_lseek allowed us to seek a non-open file!");
	success("fs_lseek");

	// Read from the file, check to make sure it is ok.
	char * test = malloc(5);
	fs_lseek(filedes, 0);
	if (fs_read(filedes, test, 5) != 5)
		error("Cannot read 5 bytes from filedes test1");
	if (fs_read(filedes, test, 5) != 0)
		error("fs_read let us read past end of file");
	if (strcmp(test, "abcd") != 0)
		error("4 bytes from file test 1 are not right!");
	success("fs_read");

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
	if (fs_close(filedes2) == -1)
		error("Couldn't close filedes2");

	// Create a 2nd file
	if (fs_create("test2") != 0)
		error("fs_create couldn't create file test2");
	if (getFile(1).block != DATA_START)
		error("2nd file block is wrong! (%d, should be %d)", getFile(1).block, DATA_START);
	if (getFile(1).offset != 5)
		error("2nd file offset is wrong! (%d, should be 5)", getFile(1).offset);
	if (getFile(1).size != 0)
		error("2nd file size is wrong! (%d, should be 0)", getFile(1).size);
	success("Created 2nd file");

	filedes2 = fs_open("test2");
	if (filedes2 == -1)
		error("Couldn't open 2nd file!");
	if (getFileDesc(filedes2).file_num != 1)
		error("File descriptor file number is wrong, %d but should be 1", getFileDesc(filedes2).file_num);
	success("Opened 2nd file");

	// Write to the file
	num_written = fs_write(filedes2, "ABCDEFG", 8);
	if (num_written != 8)
		error("Cannot write 8 bytes to file test2 (wrote %d bytes)", num_written);
	file = getFile(1);
	if (file.size != 8)
		error("Size is wrong, %d but it should be 8", file.size);
	if (getFileDesc(filedes2).offset != 8)
		error("Offset is wrong after write (%d, but it should be 8)", getFileDesc(filedes2).offset);
	success("wrote to 2nd file 1st time");

	// Read from 2nd file
	test2 = realloc(test2, 10);
	fs_lseek(filedes2, 0);
	numread = fs_read(filedes2, test2, 8);
	if (numread != 8)
		error("Couldn't read from 2nd file. read %d bytes, should be 8", numread);
	if (strcmp(test2, "ABCDEFG") != 0)
		error("Read from 2nd file is wrong, >%s< should be >ABCDEFG<", test2);
	// Check first file after write
	fs_lseek(filedes, 0);
	if (fs_read(filedes, test, 5) != 5)
		error("Cannot read file 3rd time");
	if (strcmp(test, "efgh") != 0)
		error("File contents don't match after 2nd file written. (>%s<, should be >abcd<)", test);
	success("wrote to 2nd file 2nd time");

	// Write to first file, make sure 2nd file doesn't change
	fs_lseek(filedes, 0);
	num_written = fs_write(filedes, "1234", 5);
	fs_lseek(filedes2, 0);
	numread = fs_read(filedes2, test2, 8);
	if (numread != 8)
		error("Couldn't read from 2nd file. read %d bytes, should be 8", numread);
	if (strcmp(test2, "ABCDEFG") != 0)
		error("Read from 2nd file is wrong, >%s< should be >ABCDEFG<", test2);
	success("write to 1st file in bounds doesn't affect 2nd");

	// Truncate File
	fs_lseek(filedes, 5);
	fs_lseek(filedes2, 5);
	if (fs_truncate(filedes, 3) != 0)
		error("Couldn't truncate file1 to size 3");
	if (getFileDesc(filedes).offset != 3)
		error("Truncate didn't move file pointer to EOF.");
	if (getFileDesc(filedes2).offset != 5)
		error("Truncate moved 2nd file pointer to %d (should be 5)", getFileDesc(filedes2).offset);
	if (fs_lseek(filedes, 0) != 0)
		error("Cannot lseek 1st file after truncate");
	if (fs_lseek(filedes2, 0) != 0)
		error("Cannot lseek 2nd file after truncate");
	numread = fs_read(filedes, test, 10);
	if (numread != 3)
		error("fs_read read %d bytes, but should be 3 since we truncated", numread);
	test[numread] = 0x0;
	if (strcmp(test, "123") != 0)
		error("after truncate, first file contents are wrong. >%s<, should be >123<", test);
	numread = fs_read(filedes2, test2, 10);
	if (numread != 8)
		error("after truncate, 2nd file length is wrong. %d, should be 8", numread);
	if (strcmp(test2, "ABCDEFG") != 0)
		error("after truncate, 2nd file contents are wrong. >%s<, should be >ABCDEFG<", test2);
	success("fs_truncate");
	// Delete File

	if (fs_delete("blah") == 0)
		error("fs_delete deleted a file that didn't exist!");
	if (fs_delete("test1") == 0)
		error("fs_delete deleted test1 even though it was open!");
	fs_close(filedes);
	filedes = -1;
	if (fs_delete("test1") != 0)
		error("fs_delete won't delete test1");

	success("fs_delete");

	// One file larger than block bound

	// Make another file

	// Make one file in a block, second file cross block bound

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
	printAllFiles();
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
