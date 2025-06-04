#include <stdio.h>
#include <string.h>

#define MAX_SIZE 309

typedef struct {
    int nums[MAX_SIZE];
    int size;
    int sign;
} BigInt;

void initializeBigInt(BigInt *num) {
    memset(num->nums, 0, sizeof(num->nums));
    num->size = 0;
    num->sign = 0;
}

void scanBigInt(BigInt *num) {
    char input[MAX_SIZE + 1];
    printf("Enter a BigInt number: ");
    scanf("%s", input);
    initializeBigInt(num);
    int len = strlen(input);
    if(len <= MAX_SIZE){
        if (input[0] == '-') {
            num->sign = 1;
        }

        if (num->sign == 0) {
            for (int i = 0; i < len; i++) {
                num->nums[i] = input[len - 1 - i] - '0';
            }
            num->size = len;
        } 
        else {
            for (int i = 0; i < len - 1; i++) {
                num->nums[i] = input[len - 1 - i] - '0';
            }
            num->size = len - 1;
        }
    }
    else    
        printf("Enter a valid BigInt number!");
}

void printBigInt(BigInt *num) {
    if (num->sign == 1) {
        printf("-");
    }
    for (int i = num->size - 1; i >= 0; i--) {
        printf("%d", num->nums[i]);
    }
    printf("\n");
}

void addBigInt(BigInt*, BigInt*, BigInt*);
void subtBigInt(BigInt*, BigInt*, BigInt*);
void multiplyBigInt(BigInt*, BigInt*, BigInt*);

void addBigInt(BigInt *result, BigInt *num1, BigInt *num2) {
    int carry = 0;

    if ((num1->sign == num2->sign)) {
        for (int i = 0; i < MAX_SIZE; i++) {
            int sum = num1->nums[i] + num2->nums[i] + carry;
            result->nums[i] = sum%10;
            carry = sum/10;
        }

        for (int i = MAX_SIZE - 1; i >= 0; i--) {
            if (result->nums[i] != 0) {
                result->size = i + 1;
                break;
            }
        }

        result->sign = num1->sign;
    } 
    else if (num1->sign == 0) {
        num2->sign = 0;
        subtBigInt(result, num1, num2);
        num2->sign = 1;
    } 
    else {
        num1->sign = 0;
        subtBigInt(result, num2, num1);
        num1->sign = 1;
    }
}

void multiplyBigInt(BigInt *result, BigInt *num1, BigInt *num2) {
    for (int i = 0; i < MAX_SIZE; i++) {
        int carry = 0;
        for (int j = 0; j < MAX_SIZE - i; j++) {
            int product = num1->nums[i] * num2->nums[j] + result->nums[i + j] + carry;
            result->nums[i + j] = product % 10;
            carry = product / 10;
        }
    }

    for (int i = MAX_SIZE - 1; i >= 0; i--) {
        if (result->nums[i] != 0) {
            result->size = i + 1;
            break;
        }
    }

    result->sign = (num1->sign == num2->sign) ? 0 : 1;
}

void subtBigInt(BigInt *result, BigInt *num1, BigInt *num2) {
    int max_size = (num1->size > num2->size) ? num1->size : num2->size;

    if (num1->sign == num2->sign) {
        int borrow = 0;
        result->size = max_size + 1;
        num1->nums[max_size] = 1;

        for (int i = 0; i <= max_size; i++) {
            int diff = num1->nums[i] - num2->nums[i] - borrow;

            if (diff < 0) {
                borrow = 1;
                diff += 10;
            } else {
                borrow = 0;
            }

            result->nums[i] = diff % 10;
        }

        if (result->nums[max_size] == 1) {
            result->sign = num1->sign;
            result->nums[max_size] = 0;
            result->size = max_size;
        } 
        else {
            result->nums[max_size] = 0;
            result->nums[0] = 10 - result->nums[0];
            int carry = result->nums[0] / 10;
            result->nums[0] %= 10;

            for (int i = 1; i < max_size; i++) {
                result->nums[i] = 9 - result->nums[i] + carry;
                carry = result->nums[i] / 10;
                result->nums[i] %= 10;
            }
            result->size = max_size;
            result->sign = (num1->sign == 0) ? 1 : 0;
        }

        num1->nums[max_size] = 0;
    } 
    else if (num2->sign == 1) {
        num2->sign = 0;
        addBigInt(result, num1, num2);
        result->sign = 0;
        num2->sign = 1;
    } 
    else{
        num1->sign = 0;
        addBigInt(result, num1, num2);
        result->sign = 1;
        num1->sign = 1;
    }
    
    for (int i = MAX_SIZE - 1; i >= 0; i--) {
        if (result->nums[i] != 0) {
            result->size = i + 1;
            break;
        }
    }
}

int main() {
    BigInt num1, num2, result;
    initializeBigInt(&num1);
    initializeBigInt(&num2);
    initializeBigInt(&result);

    scanBigInt(&num1);
    scanBigInt(&num2);

    printf("\n");
    addBigInt(&result, &num1, &num2);
    printf("Addition: ");
    printBigInt(&result);

    initializeBigInt(&result);
    printf("\n");
    subtBigInt(&result, &num1, &num2);
    printf("Subtraction: ");
    printBigInt(&result);

    initializeBigInt(&result);
    printf("\n");
    multiplyBigInt(&result, &num1, &num2);
    printf("Multiplication: ");
    printBigInt(&result);
    printf("\n");

    return 0;
}
