//Include libraries:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//  CITS2002 Project 1 2023
//  Student1:   23295274   Justin Gray

//  myscheduler (v1.0)
//  Compile with:  cc -std=c11 -Wall -Werror -o myscheduler myscheduler.c

//  THESE CONSTANTS DEFINE THE MAXIMUM SIZE OF sysconfig AND command DETAILS
//  THAT YOUR PROGRAM NEEDS TO SUPPORT.  YOU'LL REQUIRE THESE //  CONSTANTS
//  WHEN DEFINING THE MAXIMUM SIZES OF ANY REQUIRED DATA STRUCTURES.

//Define statements:
#define Debug
// #define Warnings
#define MAX_DEVICES                     4
#define MAX_DEVICE_NAME                 20
#define MAX_COMMANDS                    10
#define MAX_COMMAND_NAME                20
#define MAX_SYSCALLS_PER_PROCESS        40
#define MAX_RUNNING_PROCESSES           50

//  NOTE THAT DEVICE DATA-TRANSFER-RATES ARE MEASURED IN BYTES/SECOND,
//  THAT ALL TIMES ARE MEASURED IN MICROSECONDS (usecs),
//  AND THAT THE TOTAL-PROCESS-COMPLETION-TIME WILL NOT EXCEED 2000 SECONDS
//  (SO YOU CAN SAFELY USE 'STANDARD' 32-BIT ints TO STORE TIMES).

#define DEFAULT_TIME_QUANTUM            100

#define TIME_CONTEXT_SWITCH             5
#define TIME_CORE_STATE_TRANSITIONS     10
#define TIME_ACQUIRE_BUS                20

#define CHAR_COMMENT                    '#'

//Declare structs:
struct Device
{
    char name[MAX_DEVICE_NAME];
    char readSpeed[50];
    char writeSpeed[50];
};

struct Device devices[MAX_DEVICES];

//Declare struct:
struct Command
{
    char name[MAX_COMMAND_NAME]; //Command can be up to 50 characters long
    char exectionTime[MAX_SYSCALLS_PER_PROCESS][50]; //A list of up to 50 executionTimes (each up to 50 characters)
    char systemCall[MAX_SYSCALLS_PER_PROCESS][50]; //A array of up to 50 systemCalls (each up to 50 characters)
    char systemCallDetails[MAX_SYSCALLS_PER_PROCESS][50][2]; //an array of up to 50 'systemCallDetails' (each up to 50 characters)
};

struct Command commands[MAX_COMMANDS]; //Assumes up to 100 commands are present

struct PCB {
    int process_id;
    enum { READY, RUNNING, BLOCKED, TERMINATED } state;
    // Other process-specific information
};

struct systemCall
{

};

int readSysconfig(char sysconfigFileName[]);

int fetchTimeQuantum(char sysconfigfileFileName[]);

int readCommands(char commandFileName[]);

int main(int argc, char* argv[])
{
    int numDevices = readSysconfig(argv[1]);

    #ifdef Warnings
    if (numDevices == 0)
    {
        printf("WARNING: no devices!\n");
    }
    #endif

    int timeQuantum = fetchTimeQuantum(argv[1]);

    #ifdef Warnings
    if (timeQuantum == 0)
    {
        printf("WARNING: time quantum not found!\n");
        timeQuantum = DEFAULT_TIME_QUANTUM;
    }
    #endif

    int numCommands = readCommands(argv[2]);

    #ifdef Warnings
    if (numCommands == 0)
    {
        printf("WARNING: no commands found!\n");
    }
    #endif
}

int readSysconfig(char sysconfig_file_name[])
{
    char str[50];
    FILE* fp = fopen(sysconfig_file_name, "rb");

    if (fp == NULL) 
    {
        perror("File cant be opened\n");
        return EXIT_FAILURE;
    }

    static int deviceNumber = 0;
    char deviceLabelContainer[6] = "";

    while (fgets(str, 50, fp) != NULL) //read string from file
    {
        // printf("%s strncmp: %d\n", str, strncmp(str, "device", 5)); //debug for checking which lines start with device
        if (strncmp(str, "device", 5) == 0) //if line in file begins with "device"
        {
            int numRead = sscanf(str, "%s %s %s %s", 
            deviceLabelContainer, devices[deviceNumber].name, devices[deviceNumber].readSpeed, 
            devices[deviceNumber].writeSpeed);
            
            deviceNumber ++; //increment device number after populating struct 
        }
    }

    #ifdef Debug
    printf("number of devices detected: %d\n", deviceNumber);

    printf("name of device 1: %s\n", devices[0].name);
    printf("name of device 2: %s\n", devices[1].name);
    printf("name of device 3: %s\n", devices[2].name);
    printf("name of device 4: %s\n", devices[3].name);

    printf("read speed of device 1: %s\n", devices[0].readSpeed);
    printf("read speed of device 2: %s\n", devices[1].readSpeed);
    printf("read speed of device 3: %s\n", devices[2].readSpeed);
    printf("read speed of device 4: %s\n", devices[3].readSpeed);

    printf("write speed of device 1: %s\n", devices[0].writeSpeed);
    printf("write speed of device 2: %s\n", devices[1].writeSpeed);
    printf("write speed of device 3: %s\n", devices[2].writeSpeed);
    printf("write speed of device 4: %s\n", devices[3].writeSpeed);
    #endif

    fclose(fp);
    return deviceNumber;
}

int fetchTimeQuantum(char sysconfigFileName[])
{
    char str[50];
    FILE* fp = fopen(sysconfigFileName, "rb");

    if (fp == NULL) 
    {
        perror("File cant be opened\n");
        return EXIT_FAILURE;
    }

    char timeQuantumLabel[11] = "";
    char timeQuantumValue[50] = "";

    while (fgets(str, 50, fp) != NULL) //read string from file
    {
        // printf("%s strncmp: %d\n", str, strncmp(str, "device", 5)); //debug for checking which lines start with device
        if (strncmp(str, "timequantum", 11) == 0) //if line in file begins with "time "
        {
            int numRead = sscanf(str, "%s %s", timeQuantumLabel, timeQuantumValue);

            #ifdef Debug
            printf("time quantum: %s\n", timeQuantumValue);
            #endif
        }
    }

    int intTimeQuantumValue;

    if (!strcmp(timeQuantumValue, ""))
    {
        //timeQuantum was not found
        intTimeQuantumValue = 0;
    }
    else
    {
        //timeQuantum was found
        intTimeQuantumValue = atoi(timeQuantumValue);
    }

    return intTimeQuantumValue;
}

int readCommands(char commandFileName[])
{
    char str[50];
    FILE* fp = fopen(commandFileName, "rb");

    if (fp == NULL) 
    {
        perror("File cant be opened\n");
        return EXIT_FAILURE;
    }

    static int commandNumber = 0;
    int foundCommand = 0;
    static int systemCallNumber = 0;
    
    while (fgets(str, 50, fp) != NULL) //read string from file
    {
        if (foundCommand == 1)
        {
            foundCommand = 0;
            systemCallNumber = 0;

            int numRead = sscanf(str, "%s", 
            commands[commandNumber].name);
            
            commandNumber ++;
        }

        else //foundCommand == 0
        {
            systemCallNumber ++;

            if (strncmp(str, "  ", 1) == 0) //if it begins with a tab
            {
                int numRead = sscanf(str, " %s  %s  %s %s", commands[commandNumber].exectionTime[systemCallNumber], 
                commands[commandNumber].systemCall[systemCallNumber], 
                commands[commandNumber].systemCallDetails[systemCallNumber][0],
                commands[commandNumber].systemCallDetails[systemCallNumber][1]); 

                //printf("commandNumber: %d, systemCallNumber: %d, Value: %s\n",commandNumber, systemCallNumber, commands[commandNumber].exectionTime[systemCallNumber]);
            }
        }
        
        if (strncmp(str, "#", 1) == 0) //if line in file begins with "#"
        {
            foundCommand = 1;
        }
    }

    #ifdef Debug
    printf("DEBUG: Number of commands found: %d\n", commandNumber);

    printf("Name of command 1: %s\n", commands[0].name);
    printf("Name of command 2: %s\n", commands[1].name);
    printf("Name of command 3: %s\n", commands[2].name);
    printf("Name of command 4: %s\n", commands[3].name);
    printf("Name of command 5: %s\n", commands[4].name);

    printf("Execution time of command 4, system call 1: %s\n", commands[4].exectionTime[1]);

    printf("System call 4 of command 1: %s\n", commands[4].systemCall[1]);

    printf("System call detts of command 4, system call 1 part 1: %s\n", commands[4].systemCallDetails[1][0]);

    printf("System call detts of command 4, system call 1 part 2: %s\n", commands[4].systemCallDetails[1][1]);
    #endif


    return commandNumber;
}