/*=========================================================
                    SENDER.C
    Handles: Address lookup, Message->Binary, CRC generation,
    DDCMP Frame creation, Sender-side layer display, and
    writing frames to "channel.txt" (simulates the wire/
    transmission medium between sender and receiver).

    NEW: Before printing/sending each frame, the user is
    asked whether they want to introduce a bit error into
    that frame. If yes, the chosen bit is flipped in the
    frame's codeword (data + CRC) so that the receiver's
    CRC check will fail (non-zero remainder) for that frame.
=========================================================*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAXDATA 5000
#define FRAMESIZE 32
#define MAXFRAME 100

struct table {
    char url[50];
    char ip[20];
    char mac[25];
};

struct table route[] = {
    {"www.google.com","192.168.10.1","00:11:22:33:44:55"},
    {"www.amazon.in","192.168.10.2","66:77:88:99:AA:BB"},
    {"www.mepco.edu","192.168.10.3","10:20:30:40:50:60"},
    {"www.yahoo.com","192.168.10.4","AA:BB:CC:DD:EE:FF"},
    {"www.microsoft.com","192.168.10.5","11:22:33:44:55:66"}
};
#define NUM_ROUTES (sizeof(route) / sizeof(route[0]))

struct frame {
    char soh[10];
    int frame_no;
    int length;
    char src_url[50];
    char dest_url[50];
    char src_ip[20];
    char dest_ip[20];
    char src_mac[25];
    char dest_mac[25];
    char data[FRAMESIZE + 1];
    char augmented[MAXDATA];
    char crc[100];
    char codeword[MAXDATA];
    char quotient[MAXDATA];
    int modified;      /* NEW: 1 if user injected an error into this frame */
    int mod_position;  /* NEW: 1-indexed bit position that was flipped     */
};

struct table src;
struct table dest;
struct frame f[MAXFRAME];

char srcurl[50];
char desturl[50];
char message[500];
char binary[MAXDATA];
char dataword[MAXDATA];
char generator[100];
char generator_binary[100];

int total_frames;
int binary_length;

/* ---------------- Function Declarations ---------------- */
void loadTable();
void getInput();
void searchAddress();
void readMessage();
void decimalToBinary(int);
void messageToBinary();
void polynomialToBinary();
void crc_division(char *, char *, char *, char *);
void createFrames();
void modifyFrameIfRequested(int);   /* NEW */
void sender();
void saveFramesToChannel();

/*=========================================================
    Display routing table / Get user input / Validate URLs
=========================================================*/
void loadTable()
{
    int i;
    printf("\n============================================================\n");
    printf("                       ROUTING TABLE\n");
    printf("============================================================\n");
    printf("%-20s %-20s %-20s\n", "URL", "IP ADDRESS", "MAC ADDRESS");
    printf("------------------------------------------------------------\n");
    for(i = 0; i < NUM_ROUTES; i++) {
        printf("%-20s %-20s %-20s\n", route[i].url, route[i].ip, route[i].mac);
    }
    printf("============================================================\n");
}

void getInput()
{
    printf("\nEnter Source URL : ");
    scanf("%s", srcurl);

    printf("Enter Destination URL : ");
    scanf("%s", desturl);

    printf("Enter Generator Polynomial : ");
    scanf(" %[^\n]", generator);
}

void searchAddress()
{
    int i, f1 = 0, f2 = 0;

    while(1) {
        f1 = 0;
        for(i = 0; i < NUM_ROUTES; i++) {
            if(strcmp(route[i].url, srcurl) == 0) {
                src = route[i];
                f1 = 1;
                break;
            }
        }
        if(f1 == 1) break;
        printf("\nSource URL Not Found. Enter Valid Source URL : ");
        scanf("%s", srcurl);
    }

    while(1) {
        f2 = 0;
        for(i = 0; i < NUM_ROUTES; i++) {
            if(strcmp(route[i].url, desturl) == 0) {
                dest = route[i];
                f2 = 1;
                break;
            }
        }
        if(f2 == 1) break;
        printf("\nDestination URL Not Found. Enter Valid Destination URL : ");
        scanf("%s", desturl);
    }
}

/*=========================================================
    Read message from file, ASCII->Binary, Decimal->Binary
=========================================================*/
void readMessage()
{
    int ch;
    FILE *fp = fopen("message.txt", "r");

    if(fp == NULL) {
        printf("\nError: message.txt Not Found.");
        exit(0);
    }

    message[0] = '\0';
    while((ch = fgetc(fp)) != EOF) {
        int len = strlen(message);
        message[len] = ch;
        message[len + 1] = '\0';
    }
    fclose(fp);
    printf("\nMessage Read Successfully: %s\n", message);
}

void decimalToBinary(int num)
{
    int arr[8] = {0,0,0,0,0,0,0,0};
    int i = 0, j;
    int rem;
    char temp[2];

    while(num > 0) {
        rem = num % 2;
        arr[i] = rem;
        num = num / 2;
        i++;
    }

    for(j = 7; j >= 0; j--) {
        temp[0] = arr[j] + '0';
        temp[1] = '\0';
        strcat(binary, temp);
    }
}

void messageToBinary()
{
    int i;
    binary[0] = '\0';

    for(i = 0; message[i] != '\0'; i++) {
        decimalToBinary((int)message[i]);
    }

    strcpy(dataword, binary);
    binary_length = strlen(dataword);
}

/*=========================================================
    Polynomial to binary (manual power checking)
=========================================================*/
void polynomialToBinary()
{
    int power[20] = {0};
    int i = 0, max = 0, num = 0;

    while(generator[i] != '\0') {
        if(generator[i] == 'x' || generator[i] == 'X') {
            if(generator[i+1] == '^') {
                i += 2; num = 0;
                while(generator[i] >= '0' && generator[i] <= '9') {
                    num = num * 10 + (generator[i] - '0');
                    i++;
                }
                power[num] = 1;
                if(num > max) max = num;
            } else {
                power[1] = 1;
                if(max < 1) max = 1;
                i++;
            }
        }
        else if(generator[i] == '1') { power[0] = 1; i++; }
        else { i++; }
    }

    generator_binary[0] = '\0';
    for(i = max; i >= 0; i--) {
        if(power[i] == 1) strcat(generator_binary,"1");
        else strcat(generator_binary,"0");
    }
}

/*=========================================================
    CRC division (binary XOR long division), Frame creation
=========================================================*/
void crc_division(char *dividend, char *divisor, char *remainder, char *quotient)
{
    int d_len = strlen(dividend);
    int g_len = strlen(divisor);
    char temp[MAXDATA];
    int i, j;

    strcpy(temp, dividend);
    quotient[0] = '\0';

    for(i = 0; i <= d_len - g_len; i++)
    {
        if(temp[i] == '1')
        {
            strcat(quotient, "1");
            for(j = 0; j < g_len; j++) {
                temp[i + j] = (temp[i + j] == divisor[j]) ? '0' : '1';
            }
        }
        else {
            strcat(quotient, "0");
        }
    }

    for(i = d_len - g_len + 1, j = 0; i < d_len; i++, j++) {
        remainder[j] = temp[i];
    }
    remainder[j] = '\0';
}

void createFrames()
{
    int i, j, k = 0;
    int len = strlen(dataword);

    total_frames = len / FRAMESIZE;
    if(len % FRAMESIZE != 0) total_frames++;

    for(i = 0; i < total_frames; i++)
    {
        strcpy(f[i].soh, "01111110");
        f[i].frame_no = i + 1;
        strcpy(f[i].src_url, src.url);
        strcpy(f[i].src_ip, src.ip);
        strcpy(f[i].src_mac, src.mac);
        strcpy(f[i].dest_url, dest.url);
        strcpy(f[i].dest_ip, dest.ip);
        strcpy(f[i].dest_mac, dest.mac);

        int remaining = len - k;
        f[i].length = (remaining >= FRAMESIZE) ? FRAMESIZE : remaining;

        j = 0;
        while(j < FRAMESIZE) {
            if(k < len) {
                f[i].data[j] = dataword[k];
                k++;
            } else {
                f[i].data[j] = '0';
            }
            j++;
        }
        f[i].data[FRAMESIZE] = '\0';

        int glen = strlen(generator_binary);
        strcpy(f[i].augmented, f[i].data);
        for(j = 0; j < glen - 1; j++) {
            strcat(f[i].augmented, "0");
        }

        crc_division(f[i].augmented, generator_binary, f[i].crc, f[i].quotient);

        strcpy(f[i].codeword, f[i].data);
        strcat(f[i].codeword, f[i].crc);

        f[i].modified = 0;      /* NEW: default, no error injected */
        f[i].mod_position = 0;  /* NEW */
    }
}

/*=========================================================
    NEW: Ask the user whether to inject a bit error into
    this specific frame before it is transmitted/printed.
    If yes, flip the chosen bit in DATA or CRC (whichever
    the position falls into) and keep CODEWORD in sync,
    so the receiver's CRC check will fail for this frame.
=========================================================*/
void modifyFrameIfRequested(int i)
{
    char choice;
    int pos;
    int datalen = strlen(f[i].data);
    int crclen  = strlen(f[i].crc);
    int codelen = datalen + crclen;

    printf("\nFrame %d is ready to be sent.\n", f[i].frame_no);
    printf("Do you want to introduce a bit error in Frame %d before sending? (y/n): ", f[i].frame_no);
    scanf(" %c", &choice);

    if(choice == 'y' || choice == 'Y') {
        printf("Enter the bit position to flip (1 to %d, where 1-%d is DATA and %d-%d is CRC): ",
                codelen, datalen, datalen + 1, codelen);
        scanf("%d", &pos);

        while(pos < 1 || pos > codelen) {
            printf("Invalid position. Enter a bit position between 1 and %d: ", codelen);
            scanf("%d", &pos);
        }

        if(pos <= datalen) {
            /* Flip a bit inside the DATA portion */
            f[i].data[pos - 1] = (f[i].data[pos - 1] == '0') ? '1' : '0';
        } else {
            /* Flip a bit inside the CRC portion */
            int crc_pos = pos - datalen - 1;
            f[i].crc[crc_pos] = (f[i].crc[crc_pos] == '0') ? '1' : '0';
        }

        /* Rebuild codeword so DATA + CRC + CODEWORD all stay consistent */
        strcpy(f[i].codeword, f[i].data);
        strcat(f[i].codeword, f[i].crc);

        f[i].modified = 1;
        f[i].mod_position = pos;

        printf("Bit %d flipped. Frame %d will now be transmitted with an intentional error.\n",
                pos, f[i].frame_no);
    } else {
        printf("Frame %d will be transmitted unmodified.\n", f[i].frame_no);
        f[i].modified = 0;
    }
}

/*=========================================================
    Sender-side layer display (Application/Transport/
    Network/Data Link layers with DDCMP frame contents)
=========================================================*/
void sender()
{
    int i;
    printf("\n=======================================\n");
    printf("APPLICATION LAYER\n");
    printf("=======================================\n");
    printf("Application Layer Processing Completed.\n");

    printf("\n=======================================\n");
    printf("TRANSPORT LAYER\n");
    printf("=======================================\n");
    printf("Message          : %s\n", message);
    printf("Source Port      : 5000\n");
    printf("Destination Port : 8080\n");
    printf("Transport Layer Processing Completed.\n");

    printf("\n=======================================\n");
    printf("NETWORK LAYER\n");
    printf("=======================================\n");
    printf("Packets Created : %d\n", total_frames);
    printf("Network Layer Processing Completed.\n");

    printf("\n=======================================\n");
    printf("DATA LINK LAYER\n");
    printf("=======================================\n");

    for(i = 0; i < total_frames; i++) {

        /* NEW: ask before printing/sending this frame */
        modifyFrameIfRequested(i);

        printf("\n=========================================================\n");
        printf("                    DDCMP FRAME %d\n", f[i].frame_no);
        printf("=========================================================\n");

        printf("SOH : %s\n", f[i].soh);

        printf("\nHEADER");
        printf("\n---------------------------------------------------------");
        printf("\nSource URL      : %s", f[i].src_url);
        printf("\nDestination URL : %s", f[i].dest_url);
        printf("\nSource IP       : %s", f[i].src_ip);
        printf("\nDestination IP  : %s", f[i].dest_ip);
        printf("\nSource MAC      : %s", f[i].src_mac);
        printf("\nDestination MAC : %s", f[i].dest_mac);
        printf("\n---------------------------------------------------------");

        printf("\n\nSTX");
        printf("\n\nLENGTH          : %d bits", f[i].length);
        printf("\n\nDATA            :\n%s", f[i].data);
        printf("\n\nETX");
        printf("\n\nCRC (TRAILER)   : %s", f[i].crc);
        if(f[i].modified) {
            printf("\n\n[NOTE] Bit %d was intentionally flipped in this frame before transmission.", f[i].mod_position);
        }
        printf("\n=========================================================\n");
    }
    printf("\nData Link Layer Completed.\n");

    printf("\n=======================================\n");
    printf("SUMMARY\n");
    printf("=======================================\n");
    printf("Packets Created : %d\n", total_frames);
    printf("Frames Created  : %d\n", total_frames);
}

/*=========================================================
    Write all frames + generator to channel.txt
    This file acts as the shared "transmission medium"
    that the Receiver program will read from.
=========================================================*/
void saveFramesToChannel()
{
    FILE *fp = fopen("channel.txt", "w");
    int i;

    if(fp == NULL) {
        printf("\nError: Could not create channel.txt\n");
        return;
    }

    fprintf(fp, "GEN %s\n", generator_binary);
    fprintf(fp, "TOTAL %d\n", total_frames);

    for(i = 0; i < total_frames; i++) {
        fprintf(fp, "FRAME %d\n", f[i].frame_no);
        fprintf(fp, "SOH %s\n", f[i].soh);
        fprintf(fp, "SRC_URL %s\n", f[i].src_url);
        fprintf(fp, "DEST_URL %s\n", f[i].dest_url);
        fprintf(fp, "SRC_IP %s\n", f[i].src_ip);
        fprintf(fp, "DEST_IP %s\n", f[i].dest_ip);
        fprintf(fp, "SRC_MAC %s\n", f[i].src_mac);
        fprintf(fp, "DEST_MAC %s\n", f[i].dest_mac);
        fprintf(fp, "LENGTH %d\n", f[i].length);
        fprintf(fp, "DATA %s\n", f[i].data);
        fprintf(fp, "CODEWORD %s\n", f[i].codeword);
    }

    fclose(fp);
    printf("\n[Sender] Frames written to channel.txt (simulated transmission medium).\n");
    printf("[Sender] Now compile and run receiver.c to receive the message.\n");
}

/*=========================================================
                        MAIN (SENDER)
=========================================================*/
int main()
{
    loadTable();
    getInput();
    searchAddress();
    readMessage();
    messageToBinary();
    polynomialToBinary();
    createFrames();
    sender();
    saveFramesToChannel();

    return 0;
}
