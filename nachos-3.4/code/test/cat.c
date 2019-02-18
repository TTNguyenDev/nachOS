#include "syscall.h"
#define MAX_LENGTH 255

int main() {
	OpenFileId fileId;
	int i = 0;
	char *mChar;
	int fileSize;
	char filename[MAX_LENGTH];

	Print("------------TEST: CAT----------- \n");
	Scan(filename, MAX_LENGTH);
	
	fileId = Open(filename, 1);

	if (fileId == -1) {
		Print("Can't not open this file \n");
		return 0;	
	}
	
	fileSize = Seek(-1, fileId);
	i = Seek(0, fileId);

	while (i < fileSize) {
		Read(mChar, 1, 2);
		Print(mChar);
		i++;	
	}
	
	Close(fileId);
	return 0;
}
