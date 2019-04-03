#include "syscall.h"

#define MAX_LENGTH 255

int main() {
	OpenFileId sourceFile, destinationFile;
	int fileSize, count;
	char *mChar;
	char source[MAX_LENGTH], destination[MAX_LENGTH];

	PrintString("Enter file name which you want to reverse: \n");
	ReadString(source, MAX_LENGTH);
	sourceFile = Open(source, 1);
	
	if (sourceFile == -1) {
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
	
	fileSize = Seek(-1, sourceFile);

	Seek(0, sourceFile);
	for(count = 2; count <= fileSize; count++)
	{
		Seek(fileSize-count, sourceFile);
		Seek(-1, destinationFile);
		Read(mChar, 1, sourceFile);
		Write(mChar, 1, destinationFile);			
	}
	
	
	Close(sourceFile);
	Close(destinationFile);
	return 0;
}
