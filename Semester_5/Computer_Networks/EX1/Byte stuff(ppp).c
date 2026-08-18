#include <stdio.h>
#include <string.h>

#define FLAG "01111110"
#define ESC  "01111101"

int main()
{
    char message[100];
    char binary[9];

    char bits[10000];
    char byte[9];

    int i, j, num;
    int value;

    FILE *fp;


    /* ================= PPP BYTE STUFFING ================= */

    fp = fopen("stuffed.txt", "w");

    if(fp == NULL)
    {
        printf("File cannot be opened.\n");
        return 0;
    }

    printf("Enter Message: ");
    scanf("%s", message);

    printf("\nOriginal Message : %s\n", message);
    printf("\nPPP Stuffed Data :\n");

    for(i = 0; message[i] != '\0'; i++)
    {
        num = message[i];

        /* Convert character to binary */

        for(j = 7; j >= 0; j--)
        {
            if(num % 2 == 0)
                binary[j] = '0';
            else
                binary[j] = '1';

            num = num / 2;
        }

        binary[8] = '\0';

        /* Check for FLAG or ESC */

        if(strcmp(binary, FLAG) == 0 ||
           strcmp(binary, ESC) == 0)
        {
            fputs(ESC, fp);
            printf("%s ", ESC);
        }

        /* Store binary */

        fputs(binary, fp);
        printf("%s ", binary);
    }

    fclose(fp);

    printf("\n\nStuffed data stored in stuffed.txt\n");


    /* ================= PPP BYTE DESTUFFING ================= */

    fp = fopen("stuffed.txt", "r");

    if(fp == NULL)
    {
        printf("File not found.\n");
        return 0;
    }

    i = 0;

    while((bits[i] = fgetc(fp)) != EOF)
    {
        i++;
    }

    bits[i] = '\0';

    fclose(fp);

    printf("\nReceived Stuffed Data :\n%s\n", bits);

    printf("\nOriginal Message : ");

    i = 0;

    while(bits[i] != '\0')
    {
        /* Copy 8 bits */

        for(j = 0; j < 8; j++)
        {
            byte[j] = bits[i + j];
        }

        byte[8] = '\0';

        /* If ESC is found, skip it */

        if(strcmp(byte, ESC) == 0)
        {
            i = i + 8;

            for(j = 0; j < 8; j++)
            {
                byte[j] = bits[i + j];
            }

            byte[8] = '\0';
        }

        /* Convert binary to decimal */

        value = 0;

        for(j = 0; j < 8; j++)
        {
            value = value * 2;

            if(byte[j] == '1')
            {
                value = value + 1;
            }
        }

        printf("%c", value);

        i = i + 8;
    }

    printf("\n");

    return 0;
}

Enter Message: HELLO
Original Message : HELLO

PPP Stuffed Data :
01001000 01000101 01001100 01001100 01001111

Stuffed data stored in stuffed.txt

Received Stuffed Data :
0100100001000101010011000100110001001111

Original Message : HELLO