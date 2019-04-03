#include "syscall.h"

#define MAX_LENGTH 255

int main() 
{
	OpenFileId sourceFile1, sourceFile2, destinationFile;
	int i = 0, fileSize1, fileSize2;
	char *mChar;
	char source1[MAX_LENGTH], source2[MAX_LENGTH], destination[MAX_LENGTH];

	PrintString("Enter file 1 name: \n");
	ReadString(source1, MAX_LENGTH);
	sourceFile1 = Open(source1, 1);
	if (sourceFile1 == -1) 
	{
		PrintString("This file is read-only file \n");
		return 0;
	}

	PrintString("Enter file 2 name: \n");
	ReadString(source2, MAX_LENGTH);
	sourceFile2 = Open(source2, 1);
	if (sourceFile2 == -1) 
	{
		PrintString("This file is read-only file \n");
		return 0;
	}

	PrintString("Enter destination file name: \n");
	ReadString(destination, MAX_LENGTH);
	
	destinationFile = Open(destination, 0);
	if (destinationFile == -1) {
		PrintString("File doesn't existed, Want to create new file? (y/n) \n");
		ReadString(mChar, MAX_LENGTH);

		if (mChar[0] == "n" || mChar[0] == "N") return 0;
		else {
			Create(destination);
			destinationFile = Open(destination, 0);
		}
	}	

	fileSize1 = Seek(-1, sourceFile1); // coppy file 1
	Seek(0, sourceFile1);

	Seek(-1, destinationFile);
	while (i < fileSize1) 
	{
		Read(mChar, 1, sourceFile1);
		Write(mChar, 1, destinationFile);
		i++;
	}

	i = 0;

	fileSize2 = Seek(-1, sourceFile2); // coppy file 2
	Seek(0, sourceFile2);

	Seek(-1, destinationFile);
	while(i < fileSize2)
	{
		Read(mChar, 1, sourceFile2);
		Write(mChar, 1, destinationFile);
		i++;
	}
	

	Close(sourceFile1);
	Close(sourceFile2);
	Close(destinationFile);
	return 0;
}
