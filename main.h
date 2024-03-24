#include <stdio.h>
#include <ctype.h>
#include "list.h"
#include <stdbool.h>
#include <unistd.h>

//======================= Data Structures =======================

// Represents a single process in the system
typedef struct {
    int processID;            // Unique identifier for the process
    int processPriority;      // Priority level of the process
    char* processState;       // Current state of the process: (r)eady, r(u)nning, (b)locked, (d)eadlock
    char* processMessage;     // Message to be printed when the process is scheduled
} ProcessControlBlock;

// Represents a semaphore in the system
typedef struct {
    int semaphoreValue;       // Current value of the semaphore (between 0 and 4)
    int semaphoreID;          // Unique identifier for the semaphore (0 to 4)
} Semaphore;

// Represents a message sent between processes
typedef struct {
    int senderProcessID;      // ID of the process sending the message
    int recipientProcessID;   // ID of the process receiving the message
    char* messageContent;     // Content of the message
} MessagePackage;

//======================= System Lists =======================

List* allProcesses;           // List of all processes in the system
List* readyProcesses;         // List of processes ready to run
List* highPriorityProcesses;  // List of high priority (0) processes
List* normalPriorityProcesses;// List of normal priority (1) processes
List* lowPriorityProcesses;   // List of low priority (2) processes
List* messageQueue;           // List of messages to be sent between processes
List* semaphoreList;          // List of semaphores in the system

//======================= Command Functions =======================

// Creates a new process with the specified priority
int createProcess(int priority);

// Creates a new process by copying the currently running process
int forkProcess();

// Terminates the process with the specified ID
int killProcess(int processID);

// Terminates the currently running process
void killCurrentProcess();

// Expires the time quantum of the currently running process
void expireQuantum();

// Sends a message from the currently running process to the process with the specified ID
int sendProcessMessage(int processID, char* message);

// Receives a message for the currently running process
void receiveProcessMessage();

// Sends a reply message from the currently running process to the process with the specified ID
int replyToMessage(int processID, char* message);

// Creates a new semaphore with the specified ID and initial value
int createSemaphore(int semaphoreID, int initialValue);

// Executes the P (wait) operation on the semaphore with the specified ID
int executeSemaphoreP(int semaphoreID);

// Executes the V (signal) operation on the semaphore with the specified ID
int executeSemaphoreV(int semaphoreID);

// Displays detailed information about the process with the specified ID
void displayProcessInfo(int processID);

// Displays information about all processes and system queues
void displaySystemInfo();

//======================= Utility Functions =======================

// Reads a command input from the user
char getCommandInput();

// Initializes the system and sets up the necessary data structures
void initializeSystem();