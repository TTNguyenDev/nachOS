#include "syscall.h"

int main() {
	PrintString("\t####################### HELP ########################\n");
	PrintString("\t#  Group: DAT VU                                    #\n");
	PrintString("\t#  Member:                                          #\n");
	PrintString("\t# 	Nguyen Trong Triet   - 1753113              #\n");
	PrintString("\t# 	Nguyen Vuong Dat Vu  - 1753131              #\n");
	PrintString("\t# 	Truong Tran Hai Yen  - 1753139              #\n");
	PrintString("\t####################### HCMUS #######################\n");
	
	PrintString("\t * Text: Program allow user enter a string \n \t which has maximum length is 255. \n \t The input will be loaded by ReadString syscall. \n \t After that the program will print out that string \n \t to the console with PrintString syscall.\n");
	PrintString("\t * Sort: Program let user input an array of string. \n \t Using ReadInt syscall to read the input \n \t and use Bubble sort algorithm to sort that array. \n \t Finally the program will print out \n \t the sorted array to console with PrintInt syscall.\n");
	
	return 0;
}
