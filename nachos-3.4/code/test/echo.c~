#include "syscall.h"
#define MAX_LENGTH 255

int main() {
	int stdin, stdout;
	char buffer[MAX_LENGTH];
	int length;
	
	Print("-----------TEST: ECHO---------------\n");
	Print(" - Enter what you want to print to console: ");
	
	stdin = Open("stdin", 2);
	
	if (stdin != -1) {
		length = Read(buffer, MAX_LENGTH, stdin);
		
		if (length != -1 && length != -2) {
			stdout = Open("stdout", 3);
			if (stdout != -1) {
				Print("Result: ");
				Write(buffer, length, stdout);
				Close(stdout);		
			}
		} 	
		Close(stdin);
	}
	return 0;
}
