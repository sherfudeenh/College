/*=========================================================
        CHECKSUM - RECEIVER
=========================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*--------------- CONSTANTS ----------------*/

#define MAXBITS 10000

/*--------------- GLOBAL VARIABLES ----------------*/

char receivedData[MAXBITS];
char receivedChecksum[9];

/*--------------- FUNCTION PROTOTYPES ----------------*/

unsigned char wrapAround(unsigned int sum);
int binaryToDecimal(char binary[]);
void decimalTo8BitBinary(int num, char binary[]);
void checksumCalculation(char data[], char checksum[]);
void readChannel(char fileName[]);

/*=========================================================
                CHECKSUM FUNCTIONS
=========================================================*/

unsigned char wrapAround(unsigned int sum)
{
    while(sum > 255)
    {
        sum = (sum & 255) + (sum >> 8);
    }

    return sum;
}

int binaryToDecimal(char binary[])
{
    int i;
    int value = 0;

    for(i=0;i<8;i++)
    {
        value = value * 2 + (binary[i]-'0');
    }

    return value;
}

void decimalTo8BitBinary(int num,char binary[])
{
    int i;

    binary[8]='\0';

    for(i=7;i>=0;i--)
    {
        binary[i]=(num%2)+'0';
        num=num/2;
    }
}

void checksumCalculation(char data[],char checksum[])
{
    unsigned int sum=0;
    unsigned char value;
    char byte[9];
    int i;

    printf("\nCHECKSUM CALCULATION\n");

    for(i=0;data[i]!='\0';i+=8)
    {
        strncpy(byte,data+i,8);
        byte[8]='\0';

        value=binaryToDecimal(byte);

        printf("\nCurrent Sum : ");
        decimalTo8BitBinary(sum,byte);
        printf("%s",byte);

        strncpy(byte,data+i,8);
        byte[8]='\0';

        printf("\nNext Byte   : %s",byte);

        sum=sum+value;

        sum=wrapAround(sum);

        decimalTo8BitBinary(sum,byte);

        printf("\nAfter Add   : %s\n",byte);
    }

    sum=255-sum;

    decimalTo8BitBinary(sum,checksum);

    printf("\nOne's Complement");
    printf("\nChecksum : %s\n",checksum);
}

/*=========================================================
                READ FROM CHANNEL
=========================================================*/

void readChannel(char fileName[])
{
    FILE *fp;

    fp = fopen(fileName, "r");

    if(fp == NULL)
    {
        printf("\nCannot open %s\n", fileName);
        exit(1);
    }

    fscanf(fp, "%s", receivedData);
    fscanf(fp, "%s", receivedChecksum);

    fclose(fp);
}

/*=========================================================
                    MAIN FUNCTION
=========================================================*/

int main(int argc, char *argv[])
{
    char calculatedChecksum[9];
    char combined[MAXBITS];
    char verifyChecksum[9];

    if(argc > 1)
        readChannel(argv[1]);
    else
        readChannel("channel.txt");

    printf("\n========================================");
    printf("\n        RECEIVER SIDE CHECKSUM");
    printf("\n========================================\n");

    printf("\nReceived Data     : %s", receivedData);
    printf("\nReceived Checksum : %s\n", receivedChecksum);

    checksumCalculation(receivedData, calculatedChecksum);

    printf("\nCalculated Checksum : %s", calculatedChecksum);
    printf("\nReceived Checksum   : %s\n", receivedChecksum);

    strcpy(combined, receivedData);
    strcat(combined, receivedChecksum);

    checksumCalculation(combined, verifyChecksum);

    printf("\n========================================");
    printf("\n              VERIFICATION");
    printf("\n========================================\n");

    if(strcmp(verifyChecksum, "00000000") == 0)
    {
        printf("\nChecksum Verified : No Error Detected\n");
    }
    else
    {
        printf("\nChecksum Mismatch : Error Detected\n");
    }

    return 0;
}
