#include <stdio.h>

int main()
{
    char data[100];
    char stuffed[200];
    char destuffed[200];

    int i, j, count;

    /* ================= BIT STUFFING ================= */

    j = 0;
    count = 0;

    printf("Enter Data Bits: ");
    scanf("%s", data);

    for(i = 0; data[i] != '\0'; i++)
    {
        stuffed[j] = data[i];
        j++;

        if(data[i] == '1')
        {
            count++;

            if(count == 5)
            {
                stuffed[j] = '0';
                j++;
                count = 0;
            }
        }
        else
        {
            count = 0;
        }
    }

    stuffed[j] = '\0';

    printf("\nOriginal Data : %s\n", data);
    printf("HDLC Stuffed Data : %s\n", stuffed);


    /* ================= BIT DESTUFFING ================= */

    j = 0;
    count = 0;

    for(i = 0; stuffed[i] != '\0'; i++)
    {
        destuffed[j] = stuffed[i];
        j++;

        if(stuffed[i] == '1')
        {
            count++;

            if(count == 5)
            {
                i++;          /* Skip stuffed 0 */
                count = 0;
            }
        }
        else
        {
            count = 0;
        }
    }

    destuffed[j] = '\0';

    printf("\nDestuffed Data : %s\n", destuffed);

    return 0;
}

Enter Data Bits: 1111101111110

Original Data : 1111101111110
HDLC Stuffed Data : 111110011111010
Destuffed Data : 1111101111110