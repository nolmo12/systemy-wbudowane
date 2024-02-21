#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void reverseArr(int arr[], int length)
{
    int start = 0;
    int end = length - 1;
    while(start < end)
    {
        int temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}

int *binaryToGray(int binary[]) {
    int *binArr = (int*) malloc(8 * sizeof(int));
    for (int i = 1; i < 8; i++)
    {
        gray[i] = binary[i] ^ binary[i - 1];
    }
}

int* decToBinConverter(int dec) {
    int *binArr = (int*) malloc(8 * sizeof(int));
    if (binArr == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    while (dec > 0) {
        binArr[i] = dec % 2;
        dec = dec / 2;
        i++;
    }
    reverseArr(binArr, 8);
    return binArr;
}

void printArr(int arr[], int length)
{
    for(int i = 0; i < length; i++)
    {
        printf("%d", arr[i]);
    }
}

int main()
{
    int *bin;
    int gray[8];

    for(int i = 0; i < 61; i++)
    {
        bin = decToBinConverter(i);
        printArr(bin, 8);
        printf(" -- ");
        printArr(binaryToGray(bin), 8);
        sleep(1);
        system("cls");
    }
    return 0;
}
