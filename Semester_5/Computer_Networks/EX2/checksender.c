/*=========================================================
        CHECKSUM - SENDER
=========================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*--------------- CONSTANTS ----------------*/

#define MAXMESSAGE 1000
#define MAXBITS 10000

/*--------------- GLOBAL VARIABLES ----------------*/

char message[MAXMESSAGE];
char bitStream[MAXBITS];
int totalBits = 0;

/*--------------- FUNCTION PROTOTYPES ----------------*/

void decimalToBinary(int number, int bits, char binary[]);
void characterToBinary(char ch, char binary[]);
void readMessage(char fileName[]);
void messageToBinary();

unsigned char wrapAround(unsigned int sum);
int binaryToDecimal(char binary[]);
void decimalTo8BitBinary(int num, char binary[]);
void checksumCalculation(char data[], char checksum[]);

void injectError(char data[]);
void writeChannel(char data[], char checksum[]);

/*=========================================================
                DECIMAL TO BINARY
=========================================================*/

void decimalToBinary(int number, int bits, char binary[])
{
    int i;

    binary[bits] = '\0';

    for(i = bits - 1; i >= 0; i--)
    {
        binary[i] = (number % 2) + '0';
        number = number / 2;
    }
}

/*=========================================================
                CHARACTER TO BINARY
=========================================================*/

void characterToBinary(char ch, char binary[])
{
    decimalToBinary((unsigned char)ch, 8, binary);
}

/*=========================================================
                READ MESSAGE
=========================================================*/

void readMessage(char fileName[])
{
    FILE *fp;
    int i = 0;
    char ch;

    fp = fopen(fileName, "r");

    if(fp == NULL)
    {
        printf("\nCannot Open File.\n");
        return;
    }

    while((ch = fgetc(fp)) != EOF)
    {
        message[i] = ch;
        i++;
    }

    message[i] = '\0';

    fclose(fp);
}

/*=========================================================
                MESSAGE TO BINARY
=========================================================*/

void messageToBinary()
{
    int i;
    char binary[9];

    bitStream[0] = '\0';

    printf("\n=========== APPLICATION LAYER ===========\n");

    printf("\nOriginal Message : %s\n", message);

    printf("\nCharacter to Binary\n\n");

    for(i = 0; message[i] != '\0'; i++)
    {
        characterToBinary(message[i], binary);

        printf("%c  -->  %s\n", message[i], binary);

        strcat(bitStream, binary);
    }

    totalBits = strlen(bitStream);

    printf("\nComplete Bit Stream\n");

    printf("%s\n", bitStream);

    printf("\nTotal Bits = %d\n", totalBits);
}

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
                ERROR INJECTION (OPTIONAL)
=========================================================*/

void injectError(char data[])
{
    int choice;
    int pos;

    printf("\nDo you want to introduce a bit error before sending? (1-Yes / 0-No): ");
    scanf("%d",&choice);

    if(choice == 1)
    {
        printf("Enter bit position to flip (0 to %d): ", (int)strlen(data)-1);
        scanf("%d",&pos);

        if(pos >= 0 && pos < (int)strlen(data))
        {
            data[pos] = (data[pos] == '0') ? '1' : '0';
            printf("\nBit at position %d flipped.\n", pos);
        }
        else
        {
            printf("\nInvalid position. No error introduced.\n");
        }
    }
}

/*=========================================================
                WRITE TO CHANNEL
=========================================================*/

void writeChannel(char data[], char checksum[])
{
    FILE *fp;

    fp = fopen("channel.txt", "w");

    if(fp == NULL)
    {
        printf("\nCannot open channel.txt\n");
        return;
    }

    fprintf(fp, "%s\n", data);
    fprintf(fp, "%s\n", checksum);

    fclose(fp);

    printf("\nData and Checksum written to channel.txt\n");
}

/*=========================================================
                    MAIN FUNCTION
=========================================================*/

int main()
{
    char checksum[9];

    readMessage("message.txt");

    messageToBinary();

    printf("\n========================================");
    printf("\n         SENDER SIDE CHECKSUM");
    printf("\n========================================\n");

    checksumCalculation(bitStream, checksum);

    injectError(bitStream);

    printf("\n\nFinal Data Sent      : %s", bitStream);
    printf("\nChecksum Sent        : %s\n", checksum);

    writeChannel(bitStream, checksum);

    return 0;
}
