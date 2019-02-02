#include "syscall.h"

#define MAX_LENGTH 255

int main() {
	OpenFileId sourceFile, destinationFile;
	int i = 0, fileSize;
	char *mChar;
	char source[MAX_LENGTH], destination[MAX_LENGTH];

	Print("Enter file name which you want to copy: \n");
	Scan(source, MAX_LENGTH);
	sourceFile = Open(source, 1);
	
	if (sourceFile == -1) {
		Print("This file is read-only file \n");
		return 0;
	}

	Print("Enter destination file name: \n");
	Scan(destination, MAX_LENGTH);
	
	destinationFile = Open(destination, 0);
	if (destinationFile == -1) {
		Print("File doesn't existed, Want to create new file? (y/n) \n");
		Scan(mChar, MAX_LENGTH);

		if (mChar[0] == "n" || mChar[0] == "N") return 0;
		else {
			Create(destination);
			destinationFile = Open(destination, 0);
		}
	}	
	fileSize = Seek(-1, sourceFile);
	Seek(0, sourceFile);
		
	Seek(-1, destinationFile);
	while (i < fileSize) {
		Read(mChar, 1, sourceFile);
		Write(mChar, 1, destinationFile);
		i++;
	}

	Close(sourceFile);
	Close(destinationFile);
	return 0;
}
