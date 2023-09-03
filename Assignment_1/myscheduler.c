#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Device
{
    char name[50];
    char readSpeed[50];
    char writeSpeed[50];
};

int main(int argc, char* argv[])
{
    char str[50];
    FILE* fp = fopen(argv[1], "rb");

    if (fp == NULL) 
    {
        perror("File cant be opened\n");
        return EXIT_FAILURE;
    }

    struct Device devices[100]; //Assumes up to 100 devices are visable

    static int deviceNumber = 0;

    while (fgets(str, 50, fp) != NULL) //read string from file
    {
        char deviceLabelContainer[6];

        // printf("%s strncmp: %d\n", str, strncmp(str, "device", 5)); //debug for checking which lines start with device
        if (strncmp(str, "device", 5) == 0) //if line in file begins with "device"
        {
            int numRead = sscanf(str, "%s %s %s %s", 
            deviceLabelContainer, devices[deviceNumber].name, devices[deviceNumber].readSpeed, 
            devices[deviceNumber].writeSpeed);
            
            deviceNumber ++; //increment device number after populating struct 
        }
    }

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

    fclose(fp);
    return 0;
}

int populate_device_struct(char* sysconfig_file_name[])
{
    return 0;
}