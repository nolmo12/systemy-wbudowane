#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void reverseArr(int arr[], int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        int temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}

int* binaryToGray(int binary[]) {
    int *gray = (int*)malloc(8 * sizeof(int));
    if (gray == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    gray[0] = binary[0];
    for (int i = 1; i < 8; i++) {
        gray[i] = binary[i] ^ binary[i - 1];
    }
    return gray;
}

int* decToBinConverter(int dec, int bits) {
    int *binArr = (int*)malloc(bits * sizeof(int));
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
    while (i < bits) {
        binArr[i] = 0;
        i++;
    }
    reverseArr(binArr, bits);
    return binArr;
}

int binToDecConverter(int *bin, int bits)
{
    int dec = 0;
    for(int i = 7; i >= 0; i--)
    {
        if(bin[i] == 1)
            dec += 1 << (bits - 1 - i);
    }
    return dec;
}


void printArr(int arr[], int length) {
    for (int i = 0; i < length; i++) {
        printf("%d", arr[i]);
    }
}

int main() {
    int *bin;
    int *gray;
    int *seed = (int*) malloc(6 * sizeof(int));
    seed[0] = 0;
    seed[1] = 0;
    seed[2] = 0;
    seed[3] = 0;
    seed[4] = 0;
    seed[5] = 1;
    int number = binToDecConverter(seed, 6);
    printf("%d", number);

    for (int i = 0; i < 61; i++)
    {
        bin = decToBinConverter(i, 8);
        gray = binaryToGray(bin);

        printArr(bin, 8);
        printf(" -- ");
        printArr(gray, 8);
        seed[5] = seed[0] ^ seed[1] ^ seed[4] ^ seed[5];
        printf("\n%d", number);
        number = binToDecConverter(seed, 6);
        number = number >> 1;
        seed = decToBinConverter(number, 6);
        free(bin);
        free(gray);

        sleep(1);
        system("cls");
    }

    free(seed);
    return 0;
}
