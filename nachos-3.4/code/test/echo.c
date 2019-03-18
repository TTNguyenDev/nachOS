#include "syscall.h"
#define MAX_LENGTH 255

int main() {
	int stdin, stdout;
	char buffer[MAX_LENGTH];
	int length;
	
	PrintString("-----------TEST: ECHO---------------\n");
	PrintString(" - Enter what you want to PrintString to console: ");
	
	stdin = Open("stdin", 2);
	
	if (stdin != -1) {
		length = Read(buffer, MAX_LENGTH, stdin);
		
		if (length != -1 && length != -2) {
			stdout = Open("stdout", 3);
			if (stdout != -1) {
				PrintString("Result: ");
				Write(buffer, length, stdout);
				Close(stdout);		
			}
		} 	
		Close(stdin);
	}
	return 0;
}
