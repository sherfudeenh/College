#include<stdio.h>      // Standard Input/Output functions
#include<string.h>     // String handling functions
#include<stdlib.h>     // Standard library functions

#define MAXDATA 5000       // Maximum binary data size
#define FRAMESIZE 32       // Data bits in one frame
#define MAXFRAME 100       // Maximum number of frames

// Structure to store one frame
struct frame {
    char soh[10];                  // Start of Header
    int frame_no;                  // Frame number
    int length;                    // Length of binary data
    char src_url[50];              // Source URL
    char dest_url[50];             // Destination URL
    char src_ip[20];               // Source IP
    char dest_ip[20];              // Destination IP
    char src_mac[25];              // Source MAC
    char dest_mac[25];             // Destination MAC
    char data[FRAMESIZE + 1];      // Binary data
    char codeword[MAXDATA];        // Data + CRC
};

// Global variables
struct frame f[MAXFRAME];          // Array to store all frames
char generator_binary[100];        // Generator polynomial
int total_frames;                  // Total frames received

// Function declarations
void loadFramesFromChannel();
void crc_division(char *, char *, char *, char *);
void receiver();


// Function to read frames from channel.txt
void loadFramesFromChannel()
{
    // Open channel.txt in read mode
    FILE *fp = fopen("channel.txt", "r");

    char tag[20];
    int i;

    // If file not found
    if(fp == NULL) {
        printf("\nError: channel.txt Not Found. Run sender.c first.\n");
        exit(0);               // Stop execution
    }

    // Read generator polynomial
    fscanf(fp, "%s %s", tag, generator_binary);

    // Read total number of frames
    fscanf(fp, "%s %d", tag, &total_frames);

    // Read each frame
    for(i = 0; i < total_frames; i++) {

        fscanf(fp, "%s %d", tag, &f[i].frame_no);
        fscanf(fp, "%s %s", tag, f[i].soh);
        fscanf(fp, "%s %s", tag, f[i].src_url);
        fscanf(fp, "%s %s", tag, f[i].dest_url);
        fscanf(fp, "%s %s", tag, f[i].src_ip);
        fscanf(fp, "%s %s", tag, f[i].dest_ip);
        fscanf(fp, "%s %s", tag, f[i].src_mac);
        fscanf(fp, "%s %s", tag, f[i].dest_mac);
        fscanf(fp, "%s %d", tag, &f[i].length);
        fscanf(fp, "%s %s", tag, f[i].data);
        fscanf(fp, "%s %s", tag, f[i].codeword);
    }

    // Close file
    fclose(fp);

    printf("\n[Receiver] Frames successfully read from channel.txt\n");
}



// CRC Division Function
void crc_division(char *dividend,
                  char *divisor,
                  char *remainder,
                  char *quotient)
{
    int d_len = strlen(dividend);      // Dividend length
    int g_len = strlen(divisor);       // Generator length

    char temp[MAXDATA];

    int i, j;

    // Copy dividend into temp
    strcpy(temp, dividend);

    // Empty quotient initially
    quotient[0] = '\0';

    // Perform modulo-2 division
    for(i = 0; i <= d_len - g_len; i++)
    {
        // If MSB is 1 perform XOR
        if(temp[i] == '1')
        {
            strcat(quotient, "1");

            // XOR operation
            for(j = 0; j < g_len; j++)
            {
                temp[i+j] =
                (temp[i+j] == divisor[j]) ? '0' : '1';
            }
        }

        // If MSB is 0
        else
        {
            strcat(quotient, "0");
        }
    }

    // Copy remaining bits as CRC remainder
    for(i = d_len-g_len+1, j=0;
        i<d_len;
        i++,j++)
    {
        remainder[j]=temp[i];
    }

    remainder[j]='\0';
}



// Receiver Function
void receiver()
{
    int i,j;

    char remainder[100];
    char quotient[MAXDATA];

    int error_flag;

    char received_data[MAXDATA]="";

    char ascii_char[9];

    int decimal_val;

    printf("\n=======================================\n");
    printf("       RECEIVER PROCESSING\n");
    printf("=======================================\n");

    // Process each frame
    for(i=0;i<total_frames;i++)
    {
        printf("\n--- Receiving Frame %d ---\n",f[i].frame_no);

        printf("SOH               : %s\n",f[i].soh);

        printf("Source URL        : %s\n",f[i].src_url);

        printf("Destination URL   : %s\n",f[i].dest_url);

        printf("Received Codeword : %s\n",f[i].codeword);

        // Check CRC
        crc_division(
            f[i].codeword,
            generator_binary,
            remainder,
            quotient);

        printf("Decoder Quotient  : %s\n",quotient);

        printf("Syndrome (Rem)    : %s\n",remainder);

        error_flag=0;

        // Check whether remainder contains any 1
        for(j=0;j<strlen(remainder);j++)
        {
            if(remainder[j]=='1')
            {
                error_flag=1;
                break;
            }
        }

        // If error detected
        if(error_flag==1)
        {
            printf("Status: ERROR DETECTED. Frame %d discarded.\n",
                    f[i].frame_no);
        }

        // If no error
        else
        {
            printf("Status: NO ERROR (Zero syndrome). Frame %d accepted.\n",
                    f[i].frame_no);

            // Append received data
            strncat(received_data,
                    f[i].data,
                    f[i].length);
        }
    }

    // Display complete binary
    printf("\nExtracted Binary Data:\n%s\n",
            received_data);

    printf("\nDecoded Message: ");

    // Convert binary to ASCII
    for(i=0;i<strlen(received_data);i+=8)
    {
        strncpy(ascii_char,
                &received_data[i],
                8);

        ascii_char[8]='\0';

        decimal_val=0;

        // Binary → Decimal
        for(j=0;j<8;j++)
        {
            decimal_val=
            decimal_val*2+
            (ascii_char[j]-'0');
        }

        // Decimal → Character
        printf("%c",decimal_val);
    }

    printf("\n\n");
}



// Main Function
int main()
{
    // Read frames from file
    loadFramesFromChannel();

    // Perform receiver operations
    receiver();

    return 0;
}