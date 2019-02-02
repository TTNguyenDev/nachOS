#include "syscall.h"

#define MAX_LENGTH 255

int main() {
	int stdin;
	char filename[MAX_LENGTH];
	Print("---------TEST: TAO FILE-----------\n");
	Print("Enter filename: ");

	stdin = Open("stdin", 2);
	if (stdin != -1) {
		int length = Read(filename, MAX_LENGTH, stdin);

		if (length <= 1) 
			Print("file name must be more than 1 character \n");
		else {
			Create(filename);
			Print("Create file success\n");			
		}	
		Close(stdin);
	}
	return 0;
}
