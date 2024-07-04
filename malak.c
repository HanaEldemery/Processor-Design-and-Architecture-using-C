#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_LINES 100 // Maximum number of lines in the file
#define MAX_LENGTH 100 // Maximum length of each line

int registerFile [] = {0,1,1,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};

// int instructionMemory[] = {0000,0001,0010,0011,0100,0101,0110,0111,1000,1001,1010,1011,1100};
// int NumberofInstructions = sizeof(instructionMemory)/sizeof(int);
char* memory[2048];
int PC = 0;
int Flag =0;//false
char* fetch_string =NULL;
char* x=NULL;
//char* decode_string ="";
char* execute_string =NULL;
int Write_String=-1;
int clk=1;
char * opcode_before=NULL;
char * opcode_global=NULL;

int R1_before;
int R1_global;

int R2_before;
int R2_global;

int R3_before;
int R3_global;

int Immediate_before;
int Immediate_global;

int shamt_before;
int shamt_global;

int Address_before;
int Address_global;

int count_decode=0;
int count_execute=0;
int count_memory=0;
int count_writeback=0;

int DestinationDec1;
int DestinationDec2;
int DestinationExe1;
int DestinationExe2;
int DestinationMem;
int DestinationWB;

int flag_jumpwriteback=0;
int flag_jumpdecode=0;
int flag_jump=0;
int flag_decode=0; //false
int flag_execute=0;// false
int fetch_counter=1;
int decode_counter=1;
int execute_counter=1;
int memory_counter=1;
int writeBack_counter=1;
int textSize=0;
int exec_output=0;
char * opcode_mem=NULL;
int output_mem;
int DestinationReg_1;
int DestinationReg_2;
int DestinationReg_3;
int R2_mem;
int Immediate_mem;
int mem=1;
int writeback=1;
int outputwrite;
int iGlobal=0;

int binaryToDecimal(const char binary[]) {
 char* bin=strcpy(bin,binary);
 int length = strlen(bin);
 int decimal = 0;

 for (int i = 0; i < length; i++) {
 if (bin[i] == '1') {
 decimal = (decimal << 1) | 1;
 } else if (bin[i] == '0') {
 decimal <<= 1;
 } else {
 printf("Invalid input: non-binary character detected.\n");
 return 0;
 }
 }

 return decimal;
}
int binaryToDecimalIMM(char binary[]) {
 int length = strlen(binary);
 char bin[length + 1]; // Buffer to hold the binary string
 strcpy(bin, binary);

 int decimal = 0;
 int base = 1; // Base for the binary number
 int isNegative = (bin[0] == '1');

 // If the number is negative, perform two's complement
 if (isNegative) {
 // Perform 2's complement to convert to positive counterpart
 for (int i = length - 1; i >= 0; i--) {
 if (bin[i] == '1') {
 bin[i] = '0'; // Flip each bit
 } else {
 bin[i] = '1';
 }
 }
 // Add 1 to complete two's complement
 for (int i = length - 1; i >= 0; i--) {
 if (bin[i] == '0') {
 bin[i] = '1';
 break;
 } else {
 bin[i] = '0';
 }
 }
 }

 // Convert the binary number to decimal
 for (int i = length - 1; i >= (isNegative ? 1 : 0); i--) {
 if (bin[i] == '1') {
 decimal += base;
 }
 base *= 2; // Move the base to the left
 }

 // If the number is negative, make the result negative
 if (isNegative) {
 decimal = -decimal;
 }

 return decimal;
}


char* intToBinaryString(int num, int numBits) {
 char* binary = (char*)malloc(numBits + 1); // +1 for null terminator
 if (binary == NULL) {
 printf("Memory allocation failed!\n");
 exit(1);
 }

 // Convert num to binary representation
 for (int i = numBits - 1; i >= 0; i--) {
 binary[i] = (num & 1) ? '1' : '0'; // Extract the least significant bit
 num >>= 1; // Right shift to get the next bit
 }

 binary[numBits] = '\0'; // Null-terminate the string
 return binary;
}

char* RType(char instructionWhole[], char inputOpcode[]){
 char instruction[100]; // Mutable buffer to hold the instruction
 strcpy(instruction, instructionWhole);
 char *rest1;
 char *opcode;
 opcode = strtok(instruction, " ");
 rest1 = strtok(NULL, "");
 if (strcmp(inputOpcode, "0000")==0 || strcmp(inputOpcode, "0001")==0 || strcmp(inputOpcode, "0010")==0 || strcmp(inputOpcode, "0101")==0){
 char *r1;
 char *rest2;
 char *r2;
 char *r3;
 char rest1copy1[100];
 strcpy(rest1copy1, rest1);
 char rest1copy2[100]; // Mutable buffer to hold the instruction
 strcpy(rest1copy2, rest1copy1);
 r1 = strtok(rest1copy2, " ");
 rest2 = strtok(NULL, "");
 char rest2copy1[100];
 strcpy(rest2copy1, rest2);
 char rest2copy2[100]; // Mutable buffer to hold the instruction
 strcpy(rest2copy2, rest2copy1);
 r2 = strtok(rest2copy2, " ");
 r3 = strtok(NULL, "");
 int numr1=atoi(r1 + 1);
 int numr2=atoi(r2 + 1);
 int numr3=atoi(r3 + 1);
 char *concat1=intToBinaryString(numr1,5);
 char *concat2=intToBinaryString(numr2,5);
 char *concat3=intToBinaryString(numr3,5);
 size_t resultSize = strlen(inputOpcode) + strlen(concat1) + strlen(concat2) + strlen(concat3) + 13 + 1; // 13 for "0000000000000"
 char *result = (char*)malloc(resultSize);
 strcpy(result, inputOpcode);
 strcat(result, concat1);
 strcat(result, concat2);
 strcat(result, concat3);
 strcat(result, "0000000000000");
 return result;
 }
 else{
 char *r1;
 char *rest2;
 char *r2;
 char *shamt;
 char rest1copy1[100];
 strcpy(rest1copy1, rest1);
 char rest1copy2[100]; // Mutable buffer to hold the instruction
 strcpy(rest1copy2, rest1copy1);
 r1 = strtok(rest1copy2, " ");
 rest2 = strtok(NULL, "");
 char rest2copy1[100];
 strcpy(rest2copy1, rest2);
 char rest2copy2[100]; // Mutable buffer to hold the instruction
 strcpy(rest2copy2, rest2copy1);
 r2 = strtok(rest2copy2, " ");
 shamt = strtok(NULL, "");
 int numr1=atoi(r1 + 1);
 int numr2=atoi(r2 + 1);
 int numsh=atoi(shamt);
 printf("here: %i \n",numsh);
 char *concat1=intToBinaryString(numr1,5);
 char *concat2=intToBinaryString(numr2,5);
 char *concat3=intToBinaryString(numsh,13);
 size_t resultSize = strlen(inputOpcode) + strlen(concat1) + strlen(concat2) + strlen(concat3) + 5 + 1; // 13 for "0000000000000"
 char *result = (char*)malloc(resultSize);
 strcpy(result, inputOpcode);
 strcat(result, concat1);
 strcat(result, concat2);
 strcat(result, "00000");
 strcat(result, concat3);
 return result;
 }
 return "error";




}
char* JType(char inputOpcode[],char instructionWhole[]){
 char instruction[100];
 strcpy(instruction, instructionWhole);
 char *rest1;
 char *opcode;
 opcode = strtok(instruction, " ");
 rest1 = strtok(NULL, "");
 

 int address1= atoi(rest1);
 address1 = address1 | 0b0000000000000000000000000000;

 char* ret24= intToBinaryString(address1,28);

 size_t resultSize=33;
 char *result=(char*)malloc(resultSize);
 strcpy(result,inputOpcode);
 strcat(result,ret24);
 printf("print address now : %d",address1);
 return result;
}

char* helperIType(char x[], char y[]){
 char instruction[100]; // Mutable buffer to hold the instruction
 strcpy(instruction,y);

 char *rest;
 char *opcode;
 opcode = strtok(instruction, " ");
 rest= strtok(NULL, "");

 if(strcmp(x,"0011")==0){

 
 char *r1 = malloc(strlen(rest) + 1); // Allocate memory for r1
 char *IMM = malloc(strlen(rest) + 1); // Allocate memory for r2
 

 strcpy(r1, strtok(rest, " "));
 strcpy(IMM, strtok(NULL, " "));
 int regNumber= atoi(r1+1);
 sprintf(r1, "%d", regNumber);
 int h= atoi(r1);
 char * R1Bin = intToBinaryString(h,5);
 int u = atoi(IMM);
 char * IMMBin = intToBinaryString(u,18);
 size_t resultSize = 33; // 13 for "0000000000000"
 char *result = (char*)malloc(resultSize);
 strcpy(result, x);
 strcat(result,R1Bin);
 strcat(result, "00000");
 strcat(result, IMMBin);
 return result;
 }
 
 else{
 char *r1 = malloc(strlen(rest) + 1); // Allocate memory for r1
 char *r2 = malloc(strlen(rest) + 1); // Allocate memory for r2
 char *IMM = malloc(strlen(rest) + 1); // Allocate memory for r2

 strcpy(r1, strtok(rest, " "));
 strcpy(r2, strtok(NULL," "));
 strcpy(IMM, strtok(NULL, " "));
 int reg1Number= atoi(r1+1);
 int reg2Number= atoi(r2+1);
 sprintf(r1, "%d", reg1Number);
 sprintf(r2, "%d", reg2Number);
 int k= atoi(r1);
 char * R1Bin = intToBinaryString(k,5);
 int l=atoi(r2);
 char * R2Bin = intToBinaryString(l,5);
 int o= atoi(IMM);
 char * IMMBin = intToBinaryString(o,18);
 size_t resultSize = 33; // 13 for "0000000000000"
 char *result = (char*)malloc(resultSize);
 strcpy(result, x);
 strcat(result,R1Bin);
 strcat(result, R2Bin);
 strcat(result, IMMBin);
 return result;
 }
 return "error";

}

char* getOperator(char * instructionWhole){
 char* ret = malloc(4);
 char *result = malloc(33);
 char opcode[33];
 char inst[33];
 strcpy(inst,instructionWhole);
 strcpy(opcode, strtok(inst, " "));

 if(strcmp(opcode,"ADD")==0){
 ret="0000";
 
 return RType(instructionWhole,ret);
 }
 if(strcmp(opcode,"SUB")==0){
 ret="0001";
 return RType(instructionWhole,ret);

 }
 if(strcmp(opcode,"MUL")==0){
 ret="0010";
 return RType(instructionWhole,ret);

 }
 if(strcmp(opcode,"AND")==0){
 ret="0101";
 return RType(instructionWhole,ret);

 }
 if(strcmp(opcode,"LSL")==0){
 ret="1000";
 return RType(instructionWhole,ret);

 }
 if(strcmp(opcode,"LSR")==0){
 ret="1001";
 return RType(instructionWhole,ret);

 }
 if(strcmp(opcode,"MOVI")==0){
 ret="0011";
 result=helperIType(ret,instructionWhole);
 return result;
 }
 if(strcmp(opcode,"JEQ")==0){
 ret="0100";
 result=helperIType(ret,instructionWhole);
 return result;

 }
 if(strcmp(opcode,"XORI")==0){
 ret="0110";
 result=helperIType(ret,instructionWhole);
 return result;

 }
 if(strcmp(opcode,"MOVR")==0){
 ret="1010";
 result=helperIType(ret,instructionWhole);
 return result;

 }
 if(strcmp(opcode,"MOVM")==0){
 ret="1011";
 result=helperIType(ret,instructionWhole);
 return result;

 }
 
 if(strcmp(opcode,"JMP")==0){
 ret="0111";
 return JType(ret,instructionWhole);

 }

 return result;
}

void init(){

 for (int i = 0; i < 2048; i++) {
 memory[i] = NULL;
 }

 FILE* filePointer;
 char line[MAX_LENGTH]; // Buffer to hold each line read from the file
 char* lines[MAX_LINES]; // Array of strings to store each line
 int lineCount = 0; // Counter for the number of lines read
 int * operators[MAX_LINES];

 filePointer = fopen("Text.txt", "r");
 if (filePointer == NULL) {
 perror("Error opening file");
 }
 while (fgets(line, sizeof(line), filePointer) != NULL) {
 line[strcspn(line, "\n")] = '\0';
 lines[lineCount] = malloc(strlen(line) + 1);
 strcpy(lines[lineCount], line);

 lineCount++; // Increment the line count
 if (lineCount >= MAX_LINES) {
 fprintf(stderr, "Exceeded maximum number of lines\n");
 break;
 }
 
 }

 fclose(filePointer); // Close the file

 for (int i = 0; i < lineCount; i++) {
 textSize++;
 memory[i]=getOperator(lines[i]); 
 }
 //free(lines[lineCount]);
 return;
}
char* formSub(char str[], int start_index, int end_index) {
 int substring_length = end_index - start_index + 1;
 char* substring = (char*)malloc(substring_length + 1); 
 if (substring == NULL) {
 printf("Memory allocation failed!\n");
 exit(1);
 }
 for (int i = start_index, j = 0; i <= end_index; i++, j++) {
 substring[j] = str[i];
 }
 substring[substring_length] = '\0';
 return substring;
}


void memoryWriteRead(int value){ //mem

 printf("MemoryWriteRead instruction: %d \n", mem);
DestinationMem=DestinationExe2;
 mem++;
 if(strcmp(opcode_mem,"1010")==0){ //10 MOVR
 if(memory[value]==NULL){
 output_mem=0;
 printf("nMOVR Output is \n %d",output_mem);
 }
 else{
 output_mem= atoi(memory[value]);
 printf("MOVR Output is \n %d",output_mem);
 }
 
 }
 else{
 printf("the instruction is not MOVR !!!!\n");
 } 
 if(strcmp(opcode_mem,"1011")==0){ //11 MOVM
 char * value2=intToBinaryString(value,32);
 memory[(registerFile[R2_mem])+Immediate_mem]= value2; //change to memory
 printf("MOVM Output:\n %s",value2);
 }
 else{
 printf("the instruction is not MOVM !!!!\n");
 }
 //printf("\n opcodemem: %s",opcode_mem);
}
void writeBack(int value){ //wb
 printf("Write Back instruction %d \n", writeBack_counter);
 writeBack_counter++;
 DestinationWB=DestinationMem;
 printf("destination reg 3333::::::: %d \n", DestinationWB);
 if(DestinationWB > 0 && (strcmp(opcode_mem, "0100") != 0 || strcmp(opcode_mem, "0111") != 0 || strcmp(opcode_mem, "1011") != 0)){//change this
 registerFile[DestinationWB]= value;
 printf("Executed the writeback with opcodewb: %s \n",opcode_mem);
 printf("Wrote back in R1 \n");
 printf("The value expected is %d \n",value);
 printf("The value of R1_global is %d \n",DestinationWB);
 printf("The value of R1 is %d \n",registerFile[DestinationWB]);
 }
 else{
 printf("the instruction doesnt writeback!!!! \n");
 }
 
}

void decode(char binaryOperationCode[]){

 if(flag_decode==1){
 printf("decoding instruction %d \n",decode_counter);
 printf("R1Bin: %d\n", R1_global);
 printf("R2Bin: %d\n", R2_global);
 printf("R3Bin: %d\n", R3_global);
 printf("ImmediateBin: %d\n", Immediate_global);
 printf("shiftAmountBin: %d\n", shamt_global);
 printf("AddressBin: %d\n", Address_global);
 //printf("op code: %s \n", opcode_global);
 DestinationDec2=DestinationDec1;
 decode_counter++;
 }
 else{
 
 char operation[5];
 strncpy(operation, binaryOperationCode, 4);
 operation[4]='\0';
 strcpy(opcode_global,operation);

 char* R1= formSub(binaryOperationCode, 4, 8);

 int R1Bin= binaryToDecimal(R1) | 0b00000;
 R1_global=R1Bin; //r1 global=r1bin

 char* R2= formSub(binaryOperationCode, 9, 13);
 int R2Bin= binaryToDecimal(R2) | 0b00000;
 R2_global=R2Bin;

 char* R3= formSub(binaryOperationCode, 14, 18);
 int R3Bin= binaryToDecimal(R3) | 0b00000;
 R3_global=R3Bin;

 int ImmediateBin=0;
 char* Immediate= formSub(binaryOperationCode, 14, 31);
 if (strcmp(operation,"0011")==0 || strcmp(operation,"0100")==0 || strcmp(operation,"0110")==0 || strcmp(operation,"1010")==0 || strcmp(operation,"1011")==0){
 ImmediateBin= binaryToDecimalIMM(Immediate) | 0b000000000000000000;
 }
 else{
 ImmediateBin= binaryToDecimal(Immediate) | 0b000000000000000000;
 }
 Immediate_global=ImmediateBin;

 char* shiftAmount= formSub(binaryOperationCode, 20, 31);
 int shiftAmountBin= binaryToDecimal(shiftAmount) | 0b0000000000000;
 shamt_global=shiftAmountBin;
 DestinationDec1=R1Bin;

 char* Address= formSub(binaryOperationCode, 4, 31);
 printf("binaryyyyyyyyyop ");
 printf("Addressssssssssssssssssssss %s \n", Address);
 int AddressBin= binaryToDecimal(Address) | 0b0000000000000000000000000000; 
 printf("AddressBinnnnnnnnnnnnnnnnn %d \n", AddressBin);
 Address_global=AddressBin;
 printf("firstttt decoding instruction %d \n",decode_counter);
 printf("R1Bin: %d\n", R1Bin);
 printf("R2Bin: %d\n", R2Bin);
 printf("R3Bin: %d\n", R3Bin);
 printf("ImmediateBin: %d \n",ImmediateBin);
 printf("shiftAmountBin: %d\n", shiftAmountBin);
 printf("AddressBin: %d\n", AddressBin);

 }
 
}

int execute(char operation[], int R1Bin, int R2Bin, int R3Bin, int ImmediateBin, int shiftAmountBin, int AddressBin){
 int output; 
 if(flag_execute==1){
 DestinationExe2=DestinationExe1;
 printf("executing instruction gggggggggg %d \n",execute_counter);
 execute_counter++;
 if ((strcmp(operation,"0100")==0 && (registerFile[R1Bin]-registerFile[R2Bin])==0 ) || strcmp(operation,"0111")==0 ){
 printf("TESTINGGGGGG \n");
 printf("executing instruction gggg %d \n",execute_counter);
 //for mem 
 R2_mem=R2_before; 
 Immediate_mem=Immediate_before;
 //DestinationReg_3=DestinationReg_2;
 //flush
 x=NULL;
// fetch_string=NULL;
 execute_string=NULL;
 R1_global=0;
 R2_global=0;
 R3_global=0;
 Address_global=0;
 Immediate_global=0;
 shamt_global=0;
// opcode_global=NULL;
// opcode_before=NULL;
strcpy(opcode_before,"1111");
strcpy(opcode_global,"1111");
 R1_before=0;
 R2_before=0;
 R3_before=0;
 Address_before=0;
 Immediate_before=0;
 shamt_before=0;

 //jum flag = true
 flag_jump=1;
 flag_jumpdecode=1;
 flag_jumpwriteback=1;
 PC=exec_output;
 printf("\n \n ALLALALALALALALALALALLALALALALALALAL \n \n %d",PC);
 }
 
 }
 else{
 printf("executing instruction %d \n",execute_counter);
 //printf("\n \n %d efkokfeofeokefokfeokefok \n \n", R1Bin);
 DestinationExe1=DestinationDec2;
 //DestinationReg_1=R1Bin;
 // if(R1Bin == 0){ 
 // exec_output=0;
 // return 0;
 // }

 if(strcmp(operation,"0000")==0){ //0 ADD
 printf("Start executing ADD \n");
 output= registerFile[R2Bin] + registerFile[R3Bin];
 printf("OUTPUTTTTTTTTTT %d",output);
 exec_output=output;
 return output;
 }

 if(strcmp(operation,"0001")==0){ //1 SUB
 printf("Start executing SUB \n");
 output= registerFile[R2Bin] - registerFile[R3Bin];
 printf("OUTPUTTTTTTTTTT %d",output);
 exec_output=output;
 return output;
 }

 if(strcmp(operation,"0010")==0){ //2 MUL
 printf("Start executing MUL \n");
 output= registerFile[R2Bin] * registerFile[R3Bin];
 printf("OUTPUTTTTTTTTTT %d",output);
 exec_output=output;
 return output;
 }

 if(strcmp(operation,"0011")==0){ //3 MOVI
 printf("Start executing MOVI \n");
 output= ImmediateBin;
 printf("OUTPUTTTTTTTTTT %d",output);
 exec_output=output;
 return output;
 } 

 if(strcmp(operation,"0100")==0){ //4 JEQ
 printf("Start executing JEP \n");
 if(registerFile[R1Bin]-registerFile[R2Bin]==0){
 output= (PC-2) + ImmediateBin;
 printf("OUTPUTTTTTTTTTT %d",output);
 exec_output=output;
 return output;
 }
 }

 if(strcmp(operation,"0101")==0){ //5 AND
 printf("Start executing AND \n");
 output= registerFile[R2Bin] & registerFile[R3Bin];
 printf("OUTPUTTTTTTTTTT %d",output);
 exec_output=output;
 return output;
 }

 if(strcmp(operation,"0110")==0){ //6 XORI
 printf("Start executing XORI \n");
 output= registerFile[R2Bin] ^ ImmediateBin;
 printf("OUTPUTTTTTTTTTT %d",output);
 exec_output=output;
 return output;
 }

 if(strcmp(operation,"0111")==0){ //7 JMP
 printf("Start executing JMP \n");
 printf("awel PC fel jumpp %d \n",PC);
 int tempPC= (PC-2) & 0b11110000000000000000000000000000;
 printf("temp PC fel jumpp %d \n",tempPC);
 printf("address bin fel jumpp %d \n",AddressBin);
 output= (tempPC | 0b00000000000000000000000000000000) | (AddressBin);
 printf("OUTPUTTTTTTTTTT %d \n",output);
 
 exec_output=output;
 return output;
 }

 if(strcmp(operation,"1000")==0){ //8 LSL
 printf("Start executing LSL \n");
 output= registerFile[R2Bin] << shiftAmountBin;
 printf("OUTPUTTTTTTTTTT %d",output);
 exec_output=output;
 return output;
 }

 if(strcmp(operation,"1001")==0){ //9 LSR
 printf("Start executing LSR \n");
 output= registerFile[R2Bin] >> shiftAmountBin;
 printf("OUTPUTTTTTTTTTT %d",output);
 exec_output=output;
 return output;
 }
 if(strcmp(operation,"1010")==0){ //10 MOVR
 printf("Start executing MOVR \n");
 output= registerFile[R2Bin]+ ImmediateBin;
 exec_output=output;
 return output;
 }
 if(strcmp(operation,"1011")==0){ //11 MOVM
 printf("Start executing MOVM \n");
 output= registerFile[R1Bin];
 printf("OUTPUTTTTTTTTTT %d",output);
 exec_output=output;
 return output;
 }
 }
}

void fetch(){
 printf("memory pc: %s",memory[PC]);
 if(/*PC<textSize*/memory[PC]!=NULL){
 printf("fetching instruction %d \n",fetch_counter);
 strcpy(fetch_string,memory[PC]); 
 PC++;
 fetch_counter++;
 iGlobal=0;
 }
 else{
 Flag=1;
 }
}

void code(){
 int write_value = 0 ;
 int flag_init = 1;
 if(clk%2==1 && memory[0]!=NULL){
 fetch();
 }
 else{
 return;
 }
 while(/*clk<=7+((textSize-1)*2)*/iGlobal<=6){
 printf("clk:%d \n",clk);
 printf("iGlobal: %d \n",iGlobal);
 printf("PCCCCCCC %d \n",PC);
 if(clk%2==1 && memory[PC] != NULL && clk!=1){ //fetch
 fetch();
 }

 if(clk%2==0 && clk>=6 && count_memory==0){//memory ref must be here to use the correct execution output
 memoryWriteRead(exec_output);
 write_value = exec_output;
 }


 if( ((clk%2==0 && clk>=4 && flag_jump==0 && count_execute==0) || (flag_execute==1 && flag_jump==0 && count_execute==0))){ //execute//changed
 if(iGlobal<=4){
 if(flag_execute==1){
 
 execute(opcode_before,R1_before,R2_before,R3_before,Immediate_before,shamt_before,Address_before);
 printf("The output of instruction %d is %d \n",execute_counter-1,exec_output);
 flag_execute=0;
 //output_mem=exec_output;
 if(flag_jump==0){
 R2_mem=R2_before;
 Immediate_mem=Immediate_before;
 //DestinationReg_3=DestinationReg_2;
 }
 
 
 }
 else{
 printf("OPCODE BEFORE %s \n", opcode_before);
 execute(opcode_before,R1_before,R2_before,R3_before,Immediate_before,shamt_before,Address_before);
 flag_execute=1;
 }
 }
 }










 if( (clk%2==0 && fetch_string!=NULL && flag_jump==0 && count_decode==0) || (flag_decode==1 && fetch_string!=NULL && flag_jump==0 && count_decode==0)) { //decode//changed
 //printf("bye");
 if(flag_execute==0 && clk>5) //finished executing first instruction // changed
 { 
 strcpy(opcode_mem,opcode_before);//copy the opcode before changing it to use it in the memWriteRead
 printf("memory op code %s \n", opcode_mem);
 strcpy(opcode_before,opcode_global);
 printf("b4 op code %s \n", opcode_before);
 }
 printf("TEXTSIZE %d \n",textSize);
 if(iGlobal<=2){//changed
 if(flag_decode==1){
 printf("THE CLK %d \n", clk);
 printf("THE X %s \n", x);
 decode(x);
 flag_decode=0;
 if (flag_init || flag_jumpdecode==1){ //get the opcode for the first time ONLY
 strcpy(opcode_before,opcode_global);/////hmmm copied only for the first decode(2) why???
 printf("b4 op code %s \n", opcode_before);
 flag_init=0;
 flag_jumpdecode=0;
 }
 
 R1_before=R1_global;
 R2_before=R2_global;
 R3_before=R3_global;
 Immediate_before=Immediate_global;
 shamt_before=shamt_global;
 Address_before=Address_global;
 //DestinationReg_2=DestinationReg_1;
 
 }
 else{
 
 decode(fetch_string);
// strcpy(x,fetch_string);
// printf("x is %s \n",x);
 flag_decode=1;
 }
 }
 

 }



 if(clk%2==0 && clk>=6 && flag_jump==1){
 flag_jump=0;
 count_decode=1;
 count_execute=3;
 count_memory=5;
 printf("entererrererererrerererererrer");
 //count_writeback=5;
 }
 
 if(clk%2==1 && clk>=7 && count_writeback==0)
 { //check if its a mem ref or not to know what
 if(flag_jumpwriteback==1){
 count_writeback=5;
 flag_jumpwriteback=0;
 }
 if(strcmp(opcode_mem,"1010")==0 || strcmp(opcode_mem,"1011")==0){
 writeBack(output_mem);
 }
 else{
 writeBack(write_value);
 }
 //printf("AFTERRR writeback det reg 1 :::: %d \n", DestinationReg_1);
 //printf("AFTERRR writeback det reg 2 :::: %d \n", DestinationReg_2);
 //printf("AFTERRR writeback det reg 3 :::: %d \n", DestinationReg_3);
 }


 if (count_decode!=0){
 count_decode--;
 }
 if (count_execute!=0){
 count_execute--;
 }
 if (count_memory!=0){
 count_memory--;
 }
 if (count_writeback!=0){
 count_writeback--;
 }


 printf("\n \n");
 clk++;
 iGlobal++;

 }
 //printf("done");

}

int main() {
 fetch_string = (char*)malloc(100 * sizeof(char)); // Adjust size as needed
 x = (char*)malloc(100 * sizeof(char)); 
 execute_string = (char*)malloc(100 * sizeof(char)); // Adjust size as needed
 opcode_global = (char*)malloc(5 * sizeof(char));
 opcode_before = (char*)malloc(5 * sizeof(char));
 opcode_mem = (char*)malloc(5 * sizeof(char));
 
 init();
 code();
 
 free(fetch_string);
 free(opcode_global);
 free(opcode_before);
 free(execute_string);
 free(opcode_mem);
 free(x);
 return 0;
}
