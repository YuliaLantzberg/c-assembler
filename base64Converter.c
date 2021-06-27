#include <stdio.h>
#include "base64Converter.h"

/*
Convert received data (6 bits field) to base 64 char.
*/
char convert6BitsToChar(unsigned int inBits) {
    switch (inBits) {
        case 0: return 'A';
        case 1: return 'B';
        case 2: return 'C';
        case 3: return 'D';
        case 4: return 'E';
        case 5: return 'F';
        case 6: return 'G';
        case 7: return 'H';
        case 8: return 'I';
        case 9: return 'J';
        case 10: return 'K';
        case 11: return 'L';
        case 12: return 'M';
        case 13: return 'N';
        case 14: return 'O';
        case 15: return 'P';
        case 16: return 'Q';
        case 17: return 'R';
        case 18: return 'S';
        case 19: return 'T';
        case 20: return 'U';
        case 21: return 'V';
        case 22: return 'W';
        case 23: return 'X';
        case 24: return 'Y';
        case 25: return 'Z';
        case 26: return 'a';
        case 27: return 'b';
        case 28: return 'c';
        case 29: return 'd';
        case 30: return 'e';
        case 31: return 'f';
        case 32: return 'g';
        case 33: return 'h';
        case 34: return 'i';
        case 35: return 'j';
        case 36: return 'k';
        case 37: return 'l';
        case 38: return 'm';
        case 39: return 'n';
        case 40: return 'o';
        case 41: return 'p';
        case 42: return 'q';
        case 43: return 'r';
        case 44: return 's';
        case 45: return 't';
        case 46: return 'u';
        case 47: return 'v';
        case 48: return 'w';
        case 49: return 'x';
        case 50: return 'y';
        case 51: return 'z';
        case 52: return '0';
        case 53: return '1';
        case 54: return '2';
        case 55: return '3';
        case 56: return '4';
        case 57: return '5';
        case 58: return '6';
        case 59: return '7';
        case 60: return '8';
        case 61: return '9';
        case 62: return '+';
        case 63: return '/';
        default: printf("Error: Problem in converting command bits to base 64. \n"); return 0;
    }
}
 /*
 Convert a binary command input to base 64 encoding.
 */
void getBase64TwoChars(binaryCommand* input ,char output[2]) {
    output[1] = convert6BitsToChar(input->part1);
    output[0] = convert6BitsToChar(input->part2);
}
