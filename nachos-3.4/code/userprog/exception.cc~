// exception.cc
//    Entry point into the Nachos kernel from user programs.
//    There are two kinds of things that can cause control to
//    transfer back to here from user code:
//
//    syscall -- The user code explicitly requests to call a procedure
//    in the Nachos kernel.  Right now, the only function we support is
//    "Halt".
//
//    exceptions -- The user code does something that the CPU can't handle.
//    For instance, accessing memory that doesn't exist, arithmetic errors,
//    etc.
//
//    Interrupts (which can also cause control to transfer from user
//    code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

//Define area
#define MAXFILELENGTH 255

//----------------------------------------------------------------------
// ExceptionHandler
//     Entry point into the Nachos kernel.  Called when a user program
//    is executing, and either does a syscall, or generates an addressing
//    or arithmetic exception.
//
//     For system calls, the following is the calling convention:
//
//     system call code -- r2
//        arg1 -- r4
//        arg2 -- r5
//        arg3 -- r6
//        arg4 -- r7
//
//    The result of the system call, if any, must be put back into r2.
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//    "which" is the kind of exception.  The list of possible exceptions
//    are in machine.h.
//----------------------------------------------------------------------

// IncreasePC help OS increase program counter,
// previousValue = currentValue
// currentValue = nextValue
// nextValue += 4
void IncreasePC() {
    machine->registers[PrevPCReg] = machine->registers[PCReg];
    machine->registers[PCReg] = machine->registers[NextPCReg];
    machine->registers[NextPCReg] += 4;
}

char* User2System(int virAddr, int limit) {
    int index;
    int oneChar;
    char* kernelBuf = new char[limit + 1];
    
    if (!kernelBuf)
        return kernelBuf;
    
    memset(kernelBuf, 0, limit + 1);
    
    for (index = 0; index < limit; index++) {
        machine -> ReadMem(virAddr + index, 1, &oneChar);
        kernelBuf[index] = (char)oneChar;
        
        if (oneChar == 0)
            break;
    }
    return kernelBuf;
}

int System2User(int virtAddr, int length, char* buf) {
    if (length < 0)
        return -1;
    if (length == 0)
        return length;
    
    int i = 0;
    int oneChar = 0;
    
    do {
        oneChar = (int)buf[i];
        machine->WriteMem(virtAddr + i, 1, oneChar);
        i++;
    } while (i < length && oneChar != 0);
    
    return i;
}


//Handle All Exeption Type
void ExceptionHandler(ExceptionType which) {
    int type = machine->ReadRegister(2);
    
    switch (which) {
            
        case NoException:
            return;
            
        case PageFaultException:
            printf("No valid translation found");
            interrupt->Halt();
            break;
            
        case ReadOnlyException:
            printf("Write attempted to page marked read-only");
            interrupt->Halt();
            break;
            
        case BusErrorException:
            printf("Translation resulted in an invalid physical address");
            interrupt->Halt();
            break;
            
        case AddressErrorException:
            printf("Unaligned reference or one that was beyond the end of the address space");
            interrupt->Halt();
            break;
            
        case OverflowException:
            printf("Integer overflow in add or sub.");
            interrupt->Halt();
            break;
            
        case IllegalInstrException:
            printf("Unimplemented or reserved instr.");
            interrupt->Halt();
            break;
            
        case NumExceptionTypes:
            printf("NumException types");
            interrupt->Halt();
            break;
            
        case SyscallException:
            switch (type) {
                    
                    //default syscall of nachOS
                case SC_Halt:
                    printf("Shut Down \n");
                    interrupt->Halt();
                    return;
                    
                    //void Create(char *name);
                    //this func copy string name from userSpace to SystemSpace using "char* User2System(int virAddr, int limit)" func. return 0 if success else return -1
                case SC_Create: {
                    char* filename = User2System(machine->ReadRegister(4), MAXFILELENGTH + 1);
                    
                    //memory not enough for create new file
                    if (!filename) {
                        printf("Not Enought Memory \n");
                         machine -> WriteRegister(2, -1);
                    } else if (!fileSystem->Create(filename, 0)) {
                        printf("Can't create file now \n");
                        machine -> WriteRegister(2, -1);
                    } else if (strlen(filename) == 0) {
                        printf("File not found \n");
                        machine -> WriteRegister(2, -1);
                    } else {
                        printf("Create file Success \n");
                        machine -> WriteRegister(2, 0);
                    }
                    delete[] filename;
                    break;
                }
                    
                    //OpenFileId Open(char *name, int type);
                    //return OpenFileId(int value) if success else return -1
                case SC_Open: {
                    int bufAddr = machine->ReadRegister(4);
                    int mType = machine->ReadRegister(5);
                    char *filename;
                    
                    // if out of range from 0...10 return -1
                    if (fileSystem->index > 10 || fileSystem->index < 0) {
                        machine->WriteRegister(2, -1);
                        delete[] filename;
                        break;
                    }
                    
                    //stdin, stdout mode
                    filename = User2System(bufAddr, MAXFILELENGTH + 1);
                    if (strcmp(filename, "stdin") == 0) {
                        printf("Stdin mode\n");
                        machine->WriteRegister(2, 0);
                        delete[] filename;
                        break;
                    }
                    if (strcmp(filename, "stdout") == 0) {
                        printf("Stdout mode\n");
                        machine->WriteRegister(2, 1);
                        delete[] filename;
                        break;
                    }
                    
                    //open file success
                    if ((fileSystem->openf[fileSystem->index] = fileSystem->Open(filename, mType)) != NULL) {
                        
                        printf("\nOpen file success '%s' \n", filename);
                        machine->WriteRegister(2, fileSystem->index - 1);
                    } else {
                        printf("Can not open file '%s' \n", filename);
                        machine->WriteRegister(2, -1);
                    }
                    
                    delete[] filename;
                    break;
                }
                    
                    //void Close(OpenFileId id);
                    //close file with fileId. If file's already open.
                case SC_Close: {
                    int fileID = machine->ReadRegister(4);
                    
                    if (fileID >= 0 && fileID <= 9) {
                        if (fileSystem->openf[fileID]) {
                            printf("Close file success\n");
                            delete fileSystem->openf[fileID];
                            fileSystem->openf[fileID] = NULL;
                            machine->WriteRegister(2, 0);
                            break;
                        } else
                            printf("File was not opened\n");
                    } else
                        printf("File is not existed \n");
                    
                    machine->WriteRegister(2, -1);
                    break;
                }
                    
                    //int Read(char *buffer, int size, OpenFileId id);
                case SC_Read: { //y chang
                    int id = machine->ReadRegister(6);
                    
                    //Handle all error when reading file
                    //check id range, only handle when id in range 0...10
                    if (id < 0 || id > 10) {
                        printf("ID out of range \n");
                        machine->WriteRegister(2, -1);
                        IncreasePC();
                        return;
                    } else if (fileSystem->openf[id] == NULL) {
                        printf("File doesn't exist \n");
                        machine->WriteRegister(2, -1);
                        IncreasePC();
                        return;
                    } else if (fileSystem->openf[id]->type == 3) {
                        printf("Type stdout, can't read this file \n");
                        machine->WriteRegister(2, -1);
                        IncreasePC();
                        return;
                    }
                    
                    //-------------Readable file -------------
                    int virtAddr = machine->ReadRegister(4);
                    int numberOfChar = machine->ReadRegister(5);
                    
                    int oldPos = fileSystem->openf[id]->GetCurrentPos();
                    char *buf = User2System(virtAddr, numberOfChar);
                    
                    //type stdin
                    if (fileSystem->openf[id]->type == 2) {
                        int size = gSynchConsole->Read(buf, numberOfChar); //byte actually read
                        System2User(virtAddr, size, buf); //copy string from SysSpace to USpace
                        machine->WriteRegister(2, size);
                        delete buf;
                        IncreasePC();
                        return;
                    } else if (fileSystem->openf[id]->Read(buf, numberOfChar) > 0) {
                        int newPos = fileSystem->openf[id]->GetCurrentPos();
                        System2User(virtAddr, newPos - oldPos, buf);
                        machine->WriteRegister(2, newPos - oldPos);
                    }  else {
                        machine->WriteRegister(2, -2);
                    }
                    delete buf;
                    IncreasePC();
                    return;
                }
                    
                    
                    //void Write(char *buffer, int size, OpenFileId id);
                case SC_Write: {
                    int id = machine->ReadRegister(6);
                    
                    //check id range, only handle when id in range 0...10
                    if (id < 0 || id > 10) {
                        printf("Out of id range \n");
                        machine->WriteRegister(2, -1);
                        IncreasePC();
                        return;
                    } else if (fileSystem->openf[id] == NULL) {
                        printf("File doesn't exist \n");
                        machine->WriteRegister(2, -1);
                        IncreasePC();
                        return;
                    } else if (fileSystem->openf[id]->type == 1 || fileSystem->openf[id]->type == 2) {  //handle type == 1 -> OnlyRead, type == 2 Stdin
                        printf("Can't Write because type of this file is only-read or stdin \n");
                        machine->WriteRegister(2, -1);
                        IncreasePC();
                        return;
                    }
                    
                    //----------------Type can write area-----------------
                    
                    int virtAddr = machine->ReadRegister(4);
                    int charcount = machine->ReadRegister(5);
                    
                    int OldPos = fileSystem->openf[id]->GetCurrentPos();
                    char *buf = User2System(virtAddr, charcount);
                    
                    
                    //type write-read
                    if (fileSystem->openf[id]->type == 0) {
                        if (fileSystem->openf[id]->Write(buf, charcount) > 0) {
                            int NewPos = fileSystem->openf[id]->GetCurrentPos();
                            machine->WriteRegister(2, NewPos - OldPos);
                            delete buf;
                            IncreasePC();
                            return;
                        }
                    }
                    
                    //type stdout
                    if (fileSystem->openf[id]->type == 3) {
                        int i = 0;
                        while(buf[i] != 0 && buf[i] != '\n') {
                            gSynchConsole->Write(buf + i, 1);
                            i++;
                        }
                        
                        buf[i] = '\n';
                        gSynchConsole->Write(buf + i, 1);
                        machine->WriteRegister(2, i - 1);
                        delete buf;
                        IncreasePC();
                        return;
                        
                    }
                }
                    
                case SC_Seek: {
                    
                    int id = machine->ReadRegister(5);
                    
                    //check id range, only handle when id in range 0...10
                    if (id < 0 || id > 10) {
                        printf("Out of id range \n");
                        machine->WriteRegister(2, -1);
                        IncreasePC();
                        return;
                    } else if (fileSystem->openf[id] == NULL) {
                        printf("File doesn't exist \n");
                        machine->WriteRegister(2, -1);
                        IncreasePC();
                        return;
                    } else if (id == 0 || id == 1) {
                        printf("Can't use seek func on console \n");
                        machine->WriteRegister(2, -1);
                        IncreasePC();
                        return;
                    }
                    
                    //---------------Seek------------------
                    int pos = machine->ReadRegister(4);
                    pos = (pos == -1) ? fileSystem->openf[id]->Length() : pos;
                    if (pos > fileSystem->openf[id]-> Length() || pos < 0) {
                        printf("Can't seek file to this position, out of position index \n");
                        machine->WriteRegister(2, -1);
                    } else {
                        fileSystem->openf[id]->Seek(pos);
                        machine->WriteRegister(2, pos);
                    }
                    IncreasePC();
                    return;
                }
                    
                    //void Print(char buf[]);
                    //print string from console
                case SC_PrintString: {
                    int address; //containt address of buffer on register 4
                    address = machine->ReadRegister(4);
                    
                    char* buff; //containt string
                    buff = User2System(address, 255); //copy string from UserSpace to SystemSpace
                    
                    int length = 0; //length of string
                    while (buff[length]) length++;
                    
                    //use  method write in class SynchConsole to print string to console
                    gSynchConsole->Write(buff, length + 1);
                    
                    //Delete Pointer
                    delete buff;
                    IncreasePC();
                    return;
                }
                    
                    //void Scan(char* buffer, int length);
                    //scan from console
                case SC_ReadString: {
                    //containt address of buffer on register 4
                    int address;
                    address = machine->ReadRegister(4);
                    
                    //Get string length by reading register 5
                    int lengthOfString = 0;
                    lengthOfString = machine->ReadRegister(5);
                    
                    char* buff;
                    buff = User2System(address, lengthOfString); //copy string from UserSpace to SystemSpace
                    //use  method read in class SynchConsole to read string to console
                    gSynchConsole->Read(buff, lengthOfString);
                    
                    //copy string from systemSpace to userSpace to print to console
                    System2User(address, lengthOfString, buff);
                    
                    delete buff;
                    IncreasePC();
                    return;
                }
                    
                    
                    //int ReadInt();
                    //read integer from console
                case SC_ReadInt: {
                    char *buff = new char [MAXFILELENGTH + 1];
                    int size = gSynchConsole->Read(buff, MAXFILELENGTH);
                    int number = 0;
                    
                    //check isNegative
                    bool isNegative = false;
                    
                    if(buff[0] == '-')
                        isNegative = true;
                    
                    //wrong input
                    for (int i = isNegative ? 1 : 0; i < size; i++) {
                        if (buff[i] == '.') {
                            printf("\n Float is not allow \n");
                            machine->WriteRegister(2, 0);
                            IncreasePC();
                            delete buff;
                            return;
                        } else if (buff[i] < 48 || buff[i] > 57) {
                            printf("\n Bad Input\n");
                            machine->WriteRegister(2, 0);
                            IncreasePC();
                            delete buff;
                            return;
                        }
                    }
                    
                    //input value is an integer
                    for (int i = isNegative ? 1 : 0; i < size; i++)
                        number = number * 10 + (int)(buff[i] - 48);
                    
                    number = isNegative ? number *= -1 : number;
                    
                    machine->WriteRegister(2, number);
                    IncreasePC();
                    delete buff;
                    return;
                }
                    
                    //void PrintInt(int number)
                    //Print out an integer on screen
                case SC_PrintInt: {
                    int number = machine->ReadRegister(4);
                    
                    bool isNegative = false;
                    int length = 0;
                    
                    if (number < 0) {
                        isNegative = true;
                        number *= -1;
                    } else if (number == 0)
                        length = 1;
                    
                    int temp = number;
                    
                    while(temp) {
                        length++;
                        temp /= 10;
                    }
                    
                    int firstIndex = isNegative ? 1 : 0;
                    
                    char *buff = new char[MAXFILELENGTH + 1];
                    for (int i = firstIndex + length - 1; i >= firstIndex; i--) {
                        buff[i] = (char)((number % 10) + 48);
                        number /= 10;
                    }
                    
                    if (isNegative) {
                        buff[0] = '-';
                        buff[length + 1] = 0;
                        gSynchConsole->Write(buff, length + 1);
                        delete buff;
                        IncreasePC();
                        return;
                    }
                    
                    buff[length] = 0;
                    gSynchConsole->Write(buff, length);
                    delete buff;
                    IncreasePC();
                    return;
                }
                    
                    // char ReadChar()
                    // Print out an integer on screen
                case SC_ReadChar: {
                    char* buff = new char[MAXFILELENGTH];
                    int numberOfCharacter = gSynchConsole->Read(buff, MAXFILELENGTH);
                    
                    if(numberOfCharacter > 1) {
                        printf("Only char is allow!");
                        machine->WriteRegister(2, 0);
                    } else if(numberOfCharacter == 0) {
                        printf("Null isn't allow");
                        machine->WriteRegister(2, 0);
                    } else {
                        char c = buff[0];
                        machine->WriteRegister(2, c);
                    }
                    delete buff;
                    break;
                }
                    
                    //void PrintChar(char character)
                    //Print out an integer on screen
                case SC_PrintChar: {
                    char c = (char)machine->ReadRegister(4);
                    gSynchConsole->Write(&c, 1);
                    machine->WriteRegister(2, 0);
                    IncreasePC();
                    break;
                }
            }
            IncreasePC();
            break;
    }
}

