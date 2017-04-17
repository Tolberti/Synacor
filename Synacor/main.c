#include <stdio.h>

#include <stdlib.h>

#define Word unsigned short int // The VM addresses 16 bits at a time, neater to call it a word than an unsigned uint each time.

int main(int argc, char **argv)
{
    Word opcode; //The correct opcode being executed.
    unsigned int IsRunning = 1; // Using this as a boolean - as long as it's 1, the VM's on the go!
    unsigned int ProgramCounter = 0; //The current instruction we're executing.
    Word Memory[32775] = {0}  ; // Defines the main memory, 32768 values + 8 registers
    Word *Registers[8] = {  // Less cumbersome to have an array of pointers pointing to the addresses of the registers
        &Memory[32768],
        &Memory[32769],
        &Memory[32770],
        &Memory[32771],
        &Memory[32772],
        &Memory[32773],
        &Memory[32774],
        &Memory[32775]
    };
    Word *Stack = (Word *) calloc(4,2); // The stack. Initially allocated with enough memory for 2 addresses. Will reallocate on the fly.
    int StackIndex = 0; // The index of the "top" of the stack, and as such also the total size of it.
    int *StackIndexPtr = &StackIndex;
    MallocCheck(Stack); // Make sure that the Stack was allocated okay, exit the program otherwise. This'll pop up a lot.
    FILE *BinFile; // The pointer for the binary input file
    BinFile = fopen("C:\\Users\\Josh\\Desktop\\Synacor\\challenge.bin", "rb"); // Open as read-only, binary
    fread(Memory, sizeof(Word), 32768, BinFile); //Reads the first 32768 Words from the Binary File, and dumps them in main memory. Hopefully.

    while(IsRunning)
    {
        printf("$%u: ", ProgramCounter);
        opcode = Memory[ProgramCounter]; //Set the opcode to whatever's currently in the memory at PC address. 
        Word* JumpAddress;
        Word* TempValue1;
        Word* TempValue2;
        Word* TempValue3;

        switch(opcode)
        {
            case 0: // Terminate program and virtual machine.
            IsRunning = 0;
            printf("Exiting.");
            break;
            
            case 1: // Set
        
            TempValue1 = &Memory[ProgramCounter+1]; // The address of the register we're setting
            TempValue2 = &Memory[ProgramCounter+2]; // The address of the value we're setting the register to
            printf("Setting %u to %u\n", *TempValue1, *TempValue2);
            Memory[*TempValue1] = *TempValue2;
            printf("%u is now %u\n", *TempValue1, *TempValue1);
            ProgramCounter += 3;
            break;
            
            case 2: // Push        
            TempValue1 = &Memory[ProgramCounter+1]; // The value we're pushing
            RegisterCheck(TempValue1, Registers);
            Push(Stack, StackIndexPtr, TempValue1);
            ResizeStack(StackIndex, Stack);            
            printf("\n Current top stack value: %d %u\n", StackIndex-1, Stack[StackIndex]);
            ProgramCounter += 2;
            break;
            
            case 3: // Pop
            
            TempValue1 = &Memory[ProgramCounter+1]; // The address to pop to
            RegisterCheck(TempValue1, Registers);
            Pop(Stack, StackIndexPtr, TempValue1);
            ResizeStack(StackIndex, Stack);
            ProgramCounter += 2;
            break;
            
            case 4: // Eq
            TempValue1 = &Memory[ProgramCounter+1]; // The address at which to store the result
            TempValue2 = &Memory[ProgramCounter+2]; // The first number to compare
            TempValue3 = &Memory[ProgramCounter+3]; // The second number to compare
            RegisterCheck(TempValue2, Registers);
            RegisterCheck(TempValue3, Registers);
            if (*TempValue2 == *TempValue3)
            {
                Memory[*TempValue1] = 1;
            }
            else{
                    Memory[*TempValue1] = 0;
            }
            ProgramCounter += 4;            
            break;

            case 5: // gt
            TempValue1 = &Memory[ProgramCounter+1]; // The address at which to store the result
            TempValue2 = &Memory[ProgramCounter+2]; // The first number to compare
            TempValue3 = &Memory[ProgramCounter+3]; // The second number to compare
            RegisterCheck(TempValue2, Registers);
            RegisterCheck(TempValue3, Registers);
            if (*TempValue2 > *TempValue3)
            {
                Memory[*TempValue1] = 1;
            }
            else{
                    Memory[*TempValue1] = 0;
            }
            ProgramCounter += 4;            
            break;
            
            case 6: // Jump 
            
            ProgramCounter = Memory[ProgramCounter+1];
            break;
            
            case 7: // Jump if nonzero
            
            JumpAddress = &Memory[ProgramCounter+2];
            TempValue1 = &Memory[ProgramCounter+1]; // The address to check the nonzero-ness of
            RegisterCheck(JumpAddress, Registers);    
            RegisterCheck(TempValue1, Registers);                                                 
            printf("Checking value at %u for nonzero jump to %u: %u\n", ProgramCounter+1, *JumpAddress, *TempValue1);                            
            if ((Memory[ProgramCounter+1]) != 0)
            {
                printf("Jumping to %u\n", Memory[ProgramCounter+2]);               
                ProgramCounter = Memory[ProgramCounter+2];
            }
            else
            {
                ProgramCounter += 3;
            }
            break;
            
            case 8: // Jump if zero
            
            JumpAddress = &Memory[ProgramCounter+2];
            TempValue1 = &Memory[ProgramCounter+1]; // The address to check the zero-ness of
            RegisterCheck(JumpAddress, Registers);    
            RegisterCheck(TempValue1, Registers);   
            printf("Checking value at %u for zero jump to %u: %u\n", ProgramCounter+1, Memory[ProgramCounter+2], Memory[ProgramCounter+1]);    

            if ( (Memory[ProgramCounter+1])== 0)
            {
                printf("Jumping to %u\n", Memory[ProgramCounter+2]);
                ProgramCounter = Memory[ProgramCounter+2];
            }
            
            else
            {
                ProgramCounter += 3;
            }
            break;
            
            case 9: // ADD
            TempValue1 = &Memory[ProgramCounter+1]; // The address we'll be storing the result in
            TempValue2 = &Memory[ProgramCounter+2]; // The first value in our add operation
            TempValue3 = &Memory[ProgramCounter+3]; // The second value in our add operation
            RegisterCheck(TempValue2, Registers);
            RegisterCheck(TempValue3, Registers);
            Memory[*TempValue1] = (*TempValue2 + *TempValue3) % 32768;
            ProgramCounter += 4;
            break;
            
            case 10 : // Multiply
            TempValue1 = &Memory[ProgramCounter+1]; // The address we'll be storing the result in
            TempValue2 = &Memory[ProgramCounter+2]; // The first value in our mult operation
            TempValue3 = &Memory[ProgramCounter+3]; // The second value in our mult operation
            RegisterCheck(TempValue2, Registers);
            RegisterCheck(TempValue3, Registers);
            Memory[*TempValue1] = (*TempValue2 * *TempValue3) % 32768;
            ProgramCounter += 4;
            break;            
            
            case 11 : // Modulo
            TempValue1 = &Memory[ProgramCounter+1]; // The address we'll be storing the result in
            TempValue2 = &Memory[ProgramCounter+2]; // The first value in our mod operation
            TempValue3 = &Memory[ProgramCounter+3]; // The second value in our mod operation
            RegisterCheck(TempValue2, Registers);
            RegisterCheck(TempValue3, Registers);
            Memory[*TempValue1] = (*TempValue2 % *TempValue3);
            ProgramCounter += 4;
            break;       
            
            case 12: // AND
            TempValue1 = &Memory[ProgramCounter+1]; // The address to store the result in
            TempValue2 = &Memory[ProgramCounter+2]; // The first value to AND
            TempValue3 = &Memory[ProgramCounter+3]; // The second value to bitwise AND
            RegisterCheck(TempValue2, Registers);
            RegisterCheck(TempValue3, Registers);
            Memory[*TempValue1] = (*TempValue2 & *TempValue3);
            ProgramCounter += 4;
            break;
            
            case 13: // OR
            TempValue1 = &Memory[ProgramCounter+1]; // The address to store the result in
            TempValue2 = &Memory[ProgramCounter+2]; // The first value to AND
            TempValue3 = &Memory[ProgramCounter+3]; // The second value to bitwise AND
            RegisterCheck(TempValue2, Registers);
            RegisterCheck(TempValue3, Registers);
            Memory[*TempValue1] = (*TempValue2 | *TempValue3);
            ProgramCounter += 4;
            break;
            
            case 14: // NOT
            TempValue1 = &Memory[ProgramCounter+1]; // The address to store the result in
            TempValue2 = &Memory[ProgramCounter+2]; // The value to inverse
            RegisterCheck(TempValue2, Registers);
            Memory[*TempValue1] = ~*TempValue2;
            Memory[*TempValue1] %= 32768;
            ProgramCounter += 3;
            break;
            
            case 15: // RMEM: Read memory
            TempValue1 = &Memory[ProgramCounter+1]; // The address to store at
            TempValue2 = &Memory[ProgramCounter+2]; // The address to read from         
            RegisterCheck(TempValue2, Registers);
            Memory[*TempValue1] = Memory[*TempValue2];
            ProgramCounter += 3;
            break;
            
            case 16: // WMEM: Write memory
            TempValue1 = &Memory[ProgramCounter+1]; // The address to store at
            TempValue2 = &Memory[ProgramCounter+2]; // The read value
            RegisterCheck(TempValue1, Registers);
            Memory[*TempValue1] = *TempValue2;
            ProgramCounter += 3;
            break;
            
            
            case 17: // CALL
            Stack[StackIndex] = Memory[ProgramCounter+2];
            ProgramCounter = Memory[ProgramCounter+1];
            StackIndex++;
            ResizeStack(StackIndex, Stack);
            break;
            
            case 18: // Return
            if (StackIndex == 0)
            {
                printf("ERROR: EMPTY STACK.\n");
                IsRunning = 0;
            }
            else
            { 
                printf("Popped %u from stack index %u (RETURN)\n", Stack[StackIndex], StackIndex);
                ProgramCounter = Stack[StackIndex];
                ResizeStack(StackIndex, Stack);
                Stack[StackIndex] = 0;
                StackIndex--;
            }
            break;

            
            case 19: // Reads a byte from the next address and prints it to the terminal as an ASCII character.
            TempValue1 = &Memory[ProgramCounter+1]; // Address of the character to print
            printf(TempValue1);
            ProgramCounter += 2;
            break;
            
            case 21: //No operation - NOOP
            ProgramCounter++;                                         
            break;
            
            default: // If it's not any valid opcode, abort the VM.
            printf("ILLEGAL OPCODE %u AT %u\n", opcode, ProgramCounter);
            IsRunning = 0;
            
        }
    }
    
    return;
}
    
void MallocCheck(void *MallocAddress) // Checks if a malloc was successful
{
    if (MallocAddress == NULL)
    {
        printf("ERROR: MEMORY ALLOCATION FAILED\n");
        exit(1);
    }
    else 
    {
        printf("Memory allocated okay.\n");
        return;
    }
}   

void RegisterCheck(Word* CheckValue, Word* Registers[]) // When a literal integer is between 32768 and 32775, it must instead refer to the value held in the corresponding
// register rather than the literal value. This function makes sure this is enforced. Lazy hack. Don't like.

{
   if (*CheckValue >= 32768)    
   {
       *CheckValue = *Registers[*CheckValue-32768];
   }
   return;
}

void ResizeStack(Word StackSize, Word* Stack)
{

    *Stack = (Word*) realloc(Stack, sizeof(Word)*(StackSize+1));
    MallocCheck(Stack);
    return;
}

void Push(Word* Stack, int* StackIndex, Word* Value)
{
    printf("\nStack index is %d\n", *StackIndex);
    printf("\npush1\n");    
        Stack[*StackIndex] = *Value;
    printf("\nStack index is %d\n", *StackIndex);    
    printf("\npush2\n");       
    (*StackIndex)++;
    printf("\nStack index is %d\n", *StackIndex);
    printf("\npush3\n");       
    return;
}

void Pop(Word* Stack, int* StackIndex, Word* Address)
{
    if(*StackIndex == 0)
    {
        printf("POP ERROR: STACK EMPTY\n");
        exit(1);
    }
    (*StackIndex)--;
    *Address = Stack[*StackIndex];
    Stack[*StackIndex] = 0;
    return;
}