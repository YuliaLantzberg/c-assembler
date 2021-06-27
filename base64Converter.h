typedef struct binaryCommand {
    unsigned int part1:6;
    unsigned int part2:6;
} binaryCommand;

void getBase64TwoChars(binaryCommand* input ,char output[2]);
char convert6BitsToChar(unsigned int inBits);
