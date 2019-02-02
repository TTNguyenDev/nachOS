#include "syscall.h"
#include "copyright.h"

#define MAX_LENGTH 255

int main() {
	//Create file with name input from console
	int mFileId;
	char filename[MAX_LENGTH];
	Print("---------TEST: CREATE FILE-----------\n");
	Print("Enter filename: ");

	mFileId = Open("stdin", 2);
	if (mFileId != -1) {
		int length = Read(filename, MAX_LENGTH, mFileId);

		if (length <= 1) 
			Print("File name must be more than 1 character \n");
		else {
			Create(filename);
			Print("Create file success\n");				
		}	
		Close(mFileId);
	}
	return 0;

	//CreateFile with name
	//Create("defaultName");
}