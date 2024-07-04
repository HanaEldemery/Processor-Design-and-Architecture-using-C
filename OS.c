#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINES 100
#define MEMORY_SIZE 60
#define MAX_VARIABLES 3
#define MAX_STRING_LENGTH 100
typedef enum { zero, one } MutexValue;

int lower1=1;
int lower2=2;
int lower3=3;
int upper1=400;
int upper2=500;
int upper3=600;
int pc1=7;
int pc2=8;
int pc3=9;
int currentProcess=10;
int currentQueue=11;
int currentQuantum=12;
int clk;

struct Memory {
    char name[100];
    char value[100];
};

struct Memory memory[60];

typedef struct Node {
    int processID;
    struct Node* next;
} Node;

typedef struct {
    Node* front;
    Node* rear;
} Queue;

Queue queue1 = {NULL, NULL};
Queue queue2 = {NULL, NULL};
Queue queue3 = {NULL, NULL};
Queue queue4 = {NULL, NULL};
Queue blockedQueue={NULL,NULL};
Queue readyQueue={NULL,NULL};

void arrival(int programNum){
    int i=0;
    while(strcmp(memory[i].name,"")!=0){
        i++;
    }
    int lower=i;
    if (programNum==1){

        strcpy(memory[i].name, "Process ID: ");
        strcpy(memory[i].value, "1");

        strcpy(memory[i+1].name, "Process State: ");
        strcpy(memory[i+1].value, "Ready");

        strcpy(memory[i+2].name, "Current Priority: ");
        strcpy(memory[i+2].value, "1");

        strcpy(memory[i+4].name, "Lower: ");
        sprintf(memory[i+4].value, "%d", lower);
        lower1=lower;

        strcpy(memory[i+6].name, "?");
        strcpy(memory[i+6].value, "");

        strcpy(memory[i+7].name, "?");
        strcpy(memory[i+7].value, "");

        strcpy(memory[i+8].name, "?");
        strcpy(memory[i+8].value, "");

        FILE* filePointer;
        char line[100];
        int lineCount = 0;
        filePointer = fopen("Program_1.txt", "r");
        if (filePointer == NULL) {
            perror("Error opening file");
            return;
        }
        int j=9;
        while (fgets(line, sizeof(line), filePointer) != NULL && lineCount < 100) {
            line[strcspn(line, "\n")] = '\0'; // Remove trailing newline
            strcpy(memory[i+j].name, "Instruction: ");
            strcpy(memory[i+j].value, line);
            j++;
            lineCount++;
        }
        fclose(filePointer);

        strcpy(memory[i+5].name, "Upper: ");
        sprintf(memory[i+5].value, "%d", i+j-1);
        upper1=i+j-1;

        strcpy(memory[i+3].name, "Program Counter: ");
        sprintf(memory[i+3].value, "%d", i+9);
        pc1=i+9;

        enqueue(&readyQueue,1);
        enqueue(&queue1,1);

    }
    
    if (programNum==2){
        strcpy(memory[i].name, "Process ID: ");
        strcpy(memory[i].value, "2");

        strcpy(memory[i+1].name, "Process State: ");
        strcpy(memory[i+1].value, "Ready");

        strcpy(memory[i+2].name, "Current Priority: ");
        strcpy(memory[i+2].value, "1");

        strcpy(memory[i+4].name, "Lower: ");
        sprintf(memory[i+4].value, "%d", lower);
        lower2=lower;

        strcpy(memory[i+6].name, "?");
        strcpy(memory[i+6].value, "");

        strcpy(memory[i+7].name, "?");
        strcpy(memory[i+7].value, "");

        strcpy(memory[i+8].name, "?");
        strcpy(memory[i+8].value, "");

        FILE* filePointer;
        char line[100];
        int lineCount = 0;
        filePointer = fopen("Program_2.txt", "r");
        if (filePointer == NULL) {
            perror("Error opening file");
            return;
        }
        int j=9;
        while (fgets(line, sizeof(line), filePointer) != NULL && lineCount < 100) {
            line[strcspn(line, "\n")] = '\0';
            strcpy(memory[i+j].name, "Instruction: ");
            strcpy(memory[i+j].value, line);
            j++;
            lineCount++;
        }
        fclose(filePointer);

        strcpy(memory[i+5].name, "Upper: ");
        sprintf(memory[i+5].value, "%d", i+j-1);
        upper2=i+j-1;

        strcpy(memory[i+3].name, "Program Counter: ");
        sprintf(memory[i+3].value, "%d", i+9);
        pc2=i+9;

        enqueue(&readyQueue,2);
        enqueue(&queue1,2);
    }
    if (programNum==3){
        strcpy(memory[i].name, "Process ID: ");
        strcpy(memory[i].value, "3");

        strcpy(memory[i+1].name, "Process State: ");
        strcpy(memory[i+1].value, "Ready");

        strcpy(memory[i+2].name, "Current Priority: ");
        strcpy(memory[i+2].value, "1");

        strcpy(memory[i+4].name, "Lower: ");
        sprintf(memory[i+4].value, "%d", lower);
        lower3=lower;

        strcpy(memory[i+6].name, "?");
        strcpy(memory[i+6].value, "");

        strcpy(memory[i+7].name, "?");
        strcpy(memory[i+7].value, "");

        strcpy(memory[i+8].name, "?");
        strcpy(memory[i+8].value, "");

        FILE* filePointer;
        char line[100];
        int lineCount = 0;
        filePointer = fopen("Program_3.txt", "r");
        if (filePointer == NULL) {
            perror("Error opening file");
            return;
        }
        int j=9;
        while (fgets(line, sizeof(line), filePointer) != NULL && lineCount < 100) {
            line[strcspn(line, "\n")] = '\0';
            strcpy(memory[i+j].name, "Instruction: ");
            strcpy(memory[i+j].value, line);
            j++;
            lineCount++;
        }
        fclose(filePointer);

        strcpy(memory[i+5].name, "Upper: ");
        sprintf(memory[i+5].value, "%d", i+j-1);
        upper3=i+j-1;

        strcpy(memory[i+3].name, "Program Counter: ");
        sprintf(memory[i+3].value, "%d", i+9);
        pc3=i+9;

        enqueue(&readyQueue,3);
        enqueue(&queue1,3);
    }
}

typedef struct {
    char name[256];
    MutexValue value;
    Queue queue;
    int ownerID;
} Mutex;

Mutex mutexFile = { "File", one, { NULL, NULL }, -1 };
Mutex mutexInput = { "Input", one, {NULL, NULL}, -1}; //oamr
Mutex mutexOutput = { "Output", one, {NULL, NULL}, -1}; //omar

void print(const char* data){
    printf("%s \n",data);
}

void assign(char *variable, const char *value) {
    strcpy(variable, value);
}

void writeFile(const char* filename, const char* data) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    fprintf(file, "%s", data);

    fclose(file);
}

char* readFile(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char*)malloc(filesize + 1);
    if (buffer == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }

    size_t readSize = fread(buffer, 1, filesize, file);
    if (readSize != filesize) {
        perror("Error reading file");
        free(buffer);
        fclose(file);
        return NULL;
    }

    buffer[filesize] = '\0';

    fclose(file);
    return buffer;
}


void printFromTo(int x, int y) {
    if (x <= y) {
        for (int i = x; i <= y; i++) {
            printf("%d ", i);
        }
    } else {
        for (int i = x; i >= y; i--) {
            printf("%d ", i);
        }
    }
    printf("\n");
}

void initQueue(Queue* q) { //will not use
    q->front = NULL;
    q->rear = NULL;
}

void enqueue(Queue* q, int processID) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->processID = processID;
    newNode->next = NULL;

    if (q->rear == NULL) {
        q->front = newNode;
        q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

int dequeue(Queue* q) {
    if (q->front == NULL) {
        return -1; 
    } else {
        Node* temp = q->front;
        int processID = temp->processID;
        q->front = q->front->next;
        free(temp);
        if (q->front == NULL) {
            q->rear = NULL;
        }
        return processID;
    }
}

int isInQueue(Queue* q, int processID) {
    Node* current = q->front;
    while (current != NULL) {
        if (current->processID == processID) {
            return 1; 
        }
        current = current->next;
    }
    return 0;
}

int isEmpty(Queue* q) {
    return q->front == NULL;
}

void displayQueue(Queue* q) {
    if (isEmpty(q)) {
        printf("Queue is empty.\n");
        return;
    }

    Node* current = q->front;
    while (current != NULL) {
        printf("%d ", current->processID);
        current = current->next;
    }
    printf("\n");
}

void removeFromQueue(Queue* q, int processID) {
    if (q->front == NULL) {
        return;
    }

    Node* current = q->front;
    Node* previous = NULL;

    if (current != NULL && current->processID == processID) {
        q->front = current->next;
        if (q->front == NULL) {
            q->rear = NULL; 
        }
        free(current);
        return;
    }

    while (current != NULL && current->processID != processID) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        return;
    }

    previous->next = current->next;
    if (current->next == NULL) {
        q->rear = previous;
    }
    free(current);
}

void semWait(Mutex *m) {
    if (m->value == one) {
        m->ownerID = currentProcess;
        m->value = zero;
    } else {
        enqueue(&(m->queue), currentProcess);
        removeFromQueue(&readyQueue,currentProcess);
        enqueue(&blockedQueue,currentProcess);
    }
}

void semSignal(Mutex *m) {
    if (m->ownerID == currentProcess) {
        if (m->queue.front == NULL) {
            m->value = one;
        } else {
            int processID;
            if (countQueue(&(m->queue))==1){
                processID = dequeue(&(m->queue));
            }
            else {
                int first=dequeue(&(m->queue));
                int second=dequeue(&(m->queue));
                if (existsInQueue(&queue1,second)==1 && existsInQueue(&queue1,first)==0){
                    processID=second;
                    enqueue(&(m->queue),first);
                }
                else if (existsInQueue(&queue2,second)==1 && existsInQueue(&queue2,first)==0){
                    processID=second;
                    enqueue(&(m->queue),first);
                }
                else if (existsInQueue(&queue3,second)==1 && existsInQueue(&queue3,first)==0){
                    processID=second;
                    enqueue(&(m->queue),first);
                }
                else{
                    processID=first;
                    enqueue(&(m->queue),second);
                }
            }
            removeFromQueue(&blockedQueue,processID);
            enqueue(&readyQueue,processID);
            m->ownerID = processID;
        }
    }
}

int countQueue(Queue* q) {
    int count = 0;
    Node* current = q->front;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

int isSubset(Queue* smallerOrEqualQueue, Queue* biggerQueue) {
    Node* current = smallerOrEqualQueue->front;
    while (current != NULL) {
        if (!isInQueue(biggerQueue, current->processID)) {
            return 0;
        }
        current = current->next;
    }
    return 1;
}

int getQueueSize(Queue* q) {
    int size = 0;
    Node* current = q->front;
    while (current != NULL) {
        size++;
        current = current->next;
    }
    return size;
}

void whichRuns() {
    int processID;
    Queue temporary = {NULL, NULL};
    int originalQueueSize;

    originalQueueSize = getQueueSize(&queue1);
    for (int i = 0; i < originalQueueSize; i++) {
        if (isEmpty(&queue1)){
            break;
        }
        processID = dequeue(&queue1);
        if (existsInQueue(&blockedQueue, processID) == 0) {
            currentProcess = processID;
            currentQueue = 1;
            currentQuantum = 1;
            //enqueue(&queue1, processID);
            while (isEmpty(&temporary) == 0) {
                enqueue(&queue1, dequeue(&temporary));
            }
            return;
        } 
        else { //blocked
            enqueue(&temporary, processID);
        }
    }
    while (isEmpty(&temporary) == 0) {
        enqueue(&queue1, dequeue(&temporary));
    }

    originalQueueSize = getQueueSize(&queue2);
    for (int i = 0; i < originalQueueSize; i++) {
        if (isEmpty(&queue2)){
            break;
        } 
        processID = dequeue(&queue2);
        if (existsInQueue(&blockedQueue, processID) == 0) {
            currentProcess = processID;
            currentQueue = 2;
            currentQuantum = 2;
            //enqueue(&queue2,processID);
            while (isEmpty(&temporary) == 0) {
                enqueue(&queue2, dequeue(&temporary));
            }
            return;
        } 
        else { //blocked
            enqueue(&temporary, processID);
        }
    }
    while (isEmpty(&temporary) == 0) {
        enqueue(&queue2, dequeue(&temporary));
    }

    originalQueueSize = getQueueSize(&queue3);
    for (int i = 0; i < originalQueueSize; i++) {
        if (isEmpty(&queue3)){
            break;
        } 
        processID = dequeue(&queue3);
        if (existsInQueue(&blockedQueue, processID) == 0) {
            currentProcess = processID;
            currentQueue = 3;
            currentQuantum = 4;
            //enqueue(&queue3, processID);
            while (isEmpty(&temporary) == 0) {
                enqueue(&queue3, dequeue(&temporary));
            }
            return;
        } 
        else { //blocked
            enqueue(&temporary, processID);
        }
    }
    while (isEmpty(&temporary) == 0) {
        enqueue(&queue3, dequeue(&temporary));
    }

    originalQueueSize = getQueueSize(&queue4);
    for (int i = 0; i < originalQueueSize; i++) {
        if (isEmpty(&queue4)){
            break;
        } 
        processID = dequeue(&queue4);
        if (existsInQueue(&blockedQueue, processID) == 0) {
            currentProcess = processID;
            currentQueue = 4;
            currentQuantum = 8;
            while (isEmpty(&temporary) == 0) {
                enqueue(&queue4, dequeue(&temporary));
            }
            return;
        } 
        else { //blocked
            enqueue(&temporary, processID);
        }
    }
    while (isEmpty(&temporary) == 0) {
        enqueue(&queue4, dequeue(&temporary));
    }
}

void printQueue(Queue* q) {
    Node* current = q->front;
    while (current != NULL) {
        printf("%d \n", current->processID);
        current = current->next;
    }
}

Queue* get_queue_from_mutex(Mutex* mutex) {
    return &mutex->queue;
}

int existsInQueue(Queue* q, int processID) {
    Node* current = q->front;
    while (current != NULL) {
        if (current->processID == processID) {
            return 1; 
        }
        current = current->next;
    }
    return 0; 
}

void Code(){    
    int nextProcess=0;//false
    int program1;
    printf("Enter which is the program to arrive at CLK 0: ");
    scanf("%d", &program1);
    arrival(program1);
    whichRuns();
    int program2;
    printf("Enter which is the second program to arrive: ");
    scanf("%d", &program2);
    int arrival2;
    printf("Enter the arrival time for the second program: ");
    scanf("%d", &arrival2);
    int program3;
    printf("Enter which is the third program to arrive: ");
    scanf("%d", &program3);
    int arrival3;
    printf("Enter the arrival time for the third program: ");
    scanf("%d", &arrival3);

    //upper3=18;
    while (countQueue(&readyQueue)!=0){
       
        if (clk==arrival2){
            arrival(program2);
        }
        if (clk==arrival3){
            arrival(program3);
        }

        if (nextProcess==1){
            whichRuns();
        }

        nextProcess=0;
        currentQuantum=currentQuantum-1;

        int blocked=0;//false

        char* theInstruction="AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
        int currpc;
        int currlow;
        int currup;
        //printf("PC BEFORE %d \n",currpc);
        if (currentProcess==1){
            theInstruction=memory[pc1].value;
            //strcpy(theInstruction,memory[pc1].value); 
            currpc=pc1;
            currlow=lower1;
            currup=upper1;
        }
        if (currentProcess==2){
            theInstruction=memory[pc2].value;
            currpc=pc2;
            currlow=lower2;
            currup=upper2;
        }
        if (currentProcess==3){
            theInstruction=memory[pc3].value;
            currpc=pc3;
            currlow=lower3;
            currup=upper3;
        }
        //printf("PC AFTER %d \n",currpc);
        strcpy(memory[currlow+1].value,"Ready");//change status in memory to ready
        printf("CLK: %d \n",clk);
        printf("Current Process: %d From Queue: %d",currentProcess,currentQueue);
        printf(" Current Instruction: %s", theInstruction);
        printf(" PC: %d \n",currpc);
        printf("ready queue\n");
        printQueue(&readyQueue);
        //printf("\n");
        printf("blocked queue\n");
        printQueue(&blockedQueue);
        //printf("\n");
        printf("queue 1\n");
        printQueue(&queue1);
        //printf("\n");
        printf("queue 2\n");
        printQueue(&queue2);
        //printf("\n");
        printf("queue 3\n");
        printQueue(&queue3);
        //printf("\n");
        printf("queue 4\n");
        printQueue(&queue4);
        //printf("\n");
        printf("input queue\n");
        printQueue(&mutexInput.queue);
        //printf("\n");
        printf("file queue\n");
        printQueue(&mutexFile.queue);
        //printf("\n");
        printf("output queue\n");
        printQueue(&mutexOutput.queue);
        //printf("\n");
        print("MEMORY: ");
        for (int i = 0; i < 60 && memory[i].name[0] != '\0'; i++) {
            printf("%d- %s %s\n", i, memory[i].name, memory[i].value);
        }

        if(strcmp(theInstruction,"semWait userInput")==0){
            semWait(&mutexInput); 
            if(existsInQueue(get_queue_from_mutex(&mutexInput), currentProcess)){
                blocked = 1;
            }
        }
        if(strcmp(theInstruction,"semSignal userInput")==0){
            semSignal(&mutexInput); 
        }
        if(strcmp(theInstruction,"semWait file")==0){
            semWait(&mutexFile); 
            if(existsInQueue(get_queue_from_mutex(&mutexFile), currentProcess)){
                blocked = 1;
            }
        }
        if(strcmp(theInstruction,"semSignal file")==0){
            semSignal(&mutexFile); 
        }
        if(strcmp(theInstruction,"semWait userOutput")==0){
            semWait(&mutexOutput); 
            if(existsInQueue(get_queue_from_mutex(&mutexOutput), currentProcess)){
                blocked = 1;
            }
        }
        if(strcmp(theInstruction,"semSignal userOutput")==0){
            semSignal(&mutexOutput); 
        }

        if ( strncmp(theInstruction,"assign",strlen("assign"))==0 && (theInstruction[strlen("assign")] == ' ' || theInstruction[strlen("assign")] == '\0') ){
            char ins2[100];
            strcpy(ins2,theInstruction);
            char* firsthalf;
            char *secondhalf;
            firsthalf = strtok(ins2, " ");
            secondhalf = strtok(NULL, "");//a input OR a readfile b
            char ins3[100];
            strcpy(ins3,secondhalf);
            char* thirdhalf;
            char* fourthhalf;
            thirdhalf = strtok(ins3, " ");//a
            fourthhalf = strtok(NULL, "");//input or readfile b
            if (strcmp(fourthhalf,"input")==0){//input
                printf("For program %d, please enter a value for %s: ", currentProcess,thirdhalf);
                char variable[100];
                char value[100];
                scanf("%s", value);
                assign(variable, value);
                printf("%d",currlow+6);
                printf("%d",currlow+9);
                for (int i=currlow+6;i<currlow+9;i++){
                    if (strcmp(memory[i].name,"?")==0){//la2eit makan fady
                        strcpy(memory[i].name,thirdhalf);
                        strcpy(memory[i].value,variable);
                        break;
                    }
                }
            }
            else{//readfile a
                char ins4[100];
                strcpy(ins4,fourthhalf);
                char* fifthhalf;
                char* sixthhalf;
                fifthhalf = strtok(ins4, " ");//readfile
                sixthhalf = strtok(NULL, "");//b
                char* whatToReadFrom="AAAAAAAAAAAAAAAAAAAAAAAAAAA";//what was in b
                for (int i=currlow+6;i<currlow+9;i++){
                    if (strcmp(memory[i].name,sixthhalf)==0){
                        whatToReadFrom=memory[i].value;
                    }
                }

                char* value=readFile(whatToReadFrom);
                char variable[100];
                assign(variable, value);
                for (int i=currlow+6;i<currlow+9;i++){
                    if (strcmp(memory[i].name,"?")==0){//la2eit makan fady
                        strcpy(memory[i].name,thirdhalf);
                        strcpy(memory[i].value,variable);
                        break;
                    }
                }
            }
        }

        if (strncmp(theInstruction,"printFromTo",strlen("printFromTo"))==0 && (theInstruction[strlen("printFromTo")] == ' ' || theInstruction[strlen("printFromTo")] == '\0') ){
            char ins2[100];
            strcpy(ins2,theInstruction);
            char* firsthalf;
            char *secondhalf;
            firsthalf = strtok(ins2, " ");//printFromTo
            secondhalf = strtok(NULL, "");//a b
            char ins3[100];
            strcpy(ins3,secondhalf);
            char* thirdhalf;
            char* fourthhalf;
            thirdhalf = strtok(ins3, " ");//a
            fourthhalf = strtok(NULL, "");//b
            char* whatWasA="AAAAAAAAAAAAAAAAAAAAAAAAAAA";//what was in a
            char* whatWasB="AAAAAAAAAAAAAAAAAAAAAAAAAAA";//what was in b
            for (int i=currlow+6;i<currlow+9;i++){
                if (strcmp(memory[i].name,thirdhalf)==0){
                    whatWasA=memory[i].value;
                }
                if (strcmp(memory[i].name,fourthhalf)==0){
                    whatWasB=memory[i].value;
                }
            }
            int a=atoi(whatWasA);
            int b=atoi(whatWasB);
            printFromTo(a,b);
        }

        if (strncmp(theInstruction,"writeFile",strlen("writeFile"))==0 && (theInstruction[strlen("writeFile")] == ' ' || theInstruction[strlen("writeFile")] == '\0') ){
            char ins2[100];
            strcpy(ins2,theInstruction);
            char* firsthalf;
            char *secondhalf;
            firsthalf = strtok(ins2, " ");//writeFile
            secondhalf = strtok(NULL, "");//a b
            char ins3[100];
            strcpy(ins3,secondhalf);
            char* thirdhalf;
            char* fourthhalf;
            thirdhalf = strtok(ins3, " ");//a
            fourthhalf = strtok(NULL, "");//b
            char* whatWasA="AAAAAAAAAAAAAAAAAAAAAAAAAAA";//what was in a
            char* whatWasB="AAAAAAAAAAAAAAAAAAAAAAAAAAA";//what was in b
            for (int i=currlow+6;i<currlow+9;i++){
                if (strcmp(memory[i].name,thirdhalf)==0){
                    whatWasA=memory[i].value;
                }
                if (strcmp(memory[i].name,fourthhalf)==0){
                    whatWasB=memory[i].value;
                }
            }
            writeFile(whatWasA,whatWasB);
        }
        
        if ( strncmp(theInstruction,"print",strlen("print"))==0 && (theInstruction[strlen("print")] == ' ' || theInstruction[strlen("print")] == '\0') ){
            char ins2[100];
            strcpy(ins2,theInstruction);
            char* firsthalf;
            char *secondhalf;
            firsthalf = strtok(ins2, " ");
            secondhalf = strtok(NULL, "");
            for (int i=currlow+6;i<currlow+9;i++){
                if (strcmp(memory[i].name,secondhalf)==0){
                    char* printval=memory[i].value; 
                    print(printval);
                }
            }
        }

        if (blocked==1){//DONT INCREMENT PC

            strcpy(memory[currlow+1].value,"Blocked");//change status in memory to blocked

            if (currentQuantum==0){

                //printf("BLOCKED AND QUANTUM OVER THROW IN NEXT QUEUE AND CHANGE PROCESS \n");

                char* prevPri=memory[currlow+2].value;
                int prevPriInt=atoi(prevPri);
                sprintf(memory[currlow+2].value, "%d", prevPriInt+1);//update priority

                if (currentQueue==4){//throw in next queue
                    enqueue(&queue4,currentProcess);
                }
                if (currentQueue==1){
                    enqueue(&queue2,currentProcess);
                }
                if (currentQueue==2){
                    enqueue(&queue3,currentProcess);
                }
                if (currentQueue==3){
                    enqueue(&queue4,currentProcess);
                }

                char* prevPc=memory[currlow+3].value;  //OMAR
                int prevPcInt=atoi(prevPc);
                sprintf(memory[currlow+3].value, "%d", prevPcInt+1);//increment current process PC
                if (currentProcess==1){//increment current process PC
                    pc1++;

                }
                if (currentProcess==2){
                    pc2++;

                }
                if (currentProcess==3){
                    pc3++;

                }  
                //clk--;//OMAR
                nextProcess = 1;
            }
            else{
                //printf("BLOCKED AND QUANTUM NOT OVER THROW IN SAME QUEUE AND CHANGE PROCESS \n");
                //throw in same queue
                if (currentQueue==4){
                    enqueue(&queue4,currentProcess);
                }
                if (currentQueue==1){
                    enqueue(&queue1,currentProcess);
                }
                if (currentQueue==2){
                    enqueue(&queue2,currentProcess);
                }
                if (currentQueue==3){
                    enqueue(&queue3,currentProcess);
                }

                char* prevPc=memory[currlow+3].value;  //OMAR
                int prevPcInt=atoi(prevPc);
                sprintf(memory[currlow+3].value, "%d", prevPcInt+1);//increment current process PC
                if (currentProcess==1){//increment current process PC
                    pc1++;

                }
                if (currentProcess==2){
                    pc2++;

                }
                if (currentProcess==3){
                    pc3++;

                }  
                //clk--;//OMAR
                nextProcess=1;
            }
        }
        else{

            char* prevPc=memory[currlow+3].value;
            int prevPcInt=atoi(prevPc);
            sprintf(memory[currlow+3].value, "%d", prevPcInt+1);//increment current process PC
            if (currentProcess==1){//increment current process PC
                pc1++;

            }
            if (currentProcess==2){
                pc2++;

            }
            if (currentProcess==3){
                pc3++;

            }
            if (pc1==(upper1+1) && existsInQueue(&readyQueue,1)){//done  //OMAR added check queue
                sprintf(memory[currlow+3].value, "%d", prevPcInt-1); //OMAR NOW
                nextProcess=1;
                removeFromQueue(&readyQueue,1);
                strcpy(memory[currlow+1].value,"Done");//change status in memory to blocked
                strcpy(memory[currlow+2].value,"");

            }
            else if (pc2==(upper2+1) && existsInQueue(&readyQueue,2)){  //OMAR added check queue
                sprintf(memory[currlow+3].value, "%d", prevPcInt-1); //OMAR NOW
                nextProcess=1;
                removeFromQueue(&readyQueue,2);
                strcpy(memory[currlow+1].value,"Done");
                strcpy(memory[currlow+2].value,"");

            }
            else if (pc3==(upper3+1) && existsInQueue(&readyQueue,3)){  //OMAR added check queue
                sprintf(memory[currlow+3].value, "%d", prevPcInt-1); //OMAR NOW
                nextProcess=1;
                removeFromQueue(&readyQueue,3);
                strcpy(memory[currlow+1].value,"Done");
                strcpy(memory[currlow+2].value,"");
            }
            else{//IF NOT DONE
                if (currentQuantum==0){
                    //printf("NOT BLOCKED QUANTUM OVER THROW IN NEXT QUEUE AND CHANGE PROCESS \n");
                    strcpy(memory[currlow+1].value,"Preempted");//change status in memory to prempted

                    char* prevPri=memory[currlow+2].value;
                    int prevPriInt=atoi(prevPri);
                    sprintf(memory[currlow+2].value, "%d", prevPriInt+1);//update priority

                    if (currentQueue==4){//throw in next queue
                        enqueue(&queue4,currentProcess);
                    }
                    if (currentQueue==1){
                        enqueue(&queue2,currentProcess);
                    }
                    if (currentQueue==2){
                        enqueue(&queue3,currentProcess);
                    }
                    if (currentQueue==3){
                        enqueue(&queue4,currentProcess);
                    }
                    //printf("ENTERED AT CLK:[ %d], CURRENT PROCESS:[ %d] \n",clk, currentProcess);
                    nextProcess=1;
                }
                else{//CONTINUE
                    //printf("NOT BLOCKED QUANTUM NOT OVER KEEP IN QUEUE CONTINUE IN SAME PROCESS \n");
                    nextProcess=0;
                }
            }
        }
        // printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
        // printf("CLK: %d \n",clk);
        // printf("Current Process: %d From Queue: %d",currentProcess,currentQueue);
        // printf(" Current Instruction: %s", theInstruction);
        // printf(" PC: %d \n",currpc);
        // printf("ready queue\n");
        // printQueue(&readyQueue);
        // //printf("\n");
        // printf("blocked queue\n");
        // printQueue(&blockedQueue);
        // //printf("\n");
        // printf("queue 1\n");
        // printQueue(&queue1);
        // //printf("\n");
        // printf("queue 2\n");
        // printQueue(&queue2);
        // //printf("\n");
        // printf("queue 3\n");
        // printQueue(&queue3);
        // //printf("\n");
        // printf("queue 4\n");
        // printQueue(&queue4);
        // //printf("\n");
        // printf("input queue\n");
        // printQueue(&mutexInput.queue);
        // //printf("\n");
        // printf("file queue\n");
        // printQueue(&mutexFile.queue);
        // //printf("\n");
        // printf("output queue\n");
        // printQueue(&mutexOutput.queue);
        // //printf("\n");
        // print("MEMORY: ");
        // for (int i = 0; i < 60 && memory[i].name[0] != '\0'; i++) {
        //     printf("%d- %s %s\n", i, memory[i].name, memory[i].value);
        // }
         clk++;
    }
}

int main() {
    Code();
    return 0;
}