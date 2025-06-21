#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 309

typedef struct {
    int nums[MAX_SIZE + 1];
    int sign; // '0' means positive & '1' mean negative
    int size;
} BigInt;

void initialise_Number(BigInt** num){
    memset((*num)->nums, 0, sizeof((*num)->nums));
    (*num)->sign = 0;
    (*num)->size = 0;
}

BigInt* Addition(BigInt*, BigInt*);
BigInt* Substraction(BigInt*, BigInt*);
BigInt* Multiplication(BigInt*, BigInt*);

BigInt* scanNumber(){
    char tmp[MAX_SIZE + 1];
    BigInt* num = (BigInt*)malloc(sizeof(BigInt));
    initialise_Number(&num);
    printf("\nEnter a 1024 or less bit number: ");
    scanf("%s", tmp);

    int len = strlen(tmp);
    if(len > MAX_SIZE){
        printf("Invalid Number! The number is out of 1024 bit (309 Digits).\n");
        free(num);
        return 0;
    }

    if(tmp[0] == '-') num->sign = 1;

    num->size = (num->sign) ? len - 1 : len;
    int limit = (num->sign) ? 1 : 0;
    int j = 0;
    for (int i = len - 1; i >= limit; i--){
        if(!(48 <= tmp[i] && tmp[i] <= 57) ){
            printf("Invalid Number!\n");
            free(num);
            return 0;
        }
        num->nums[j++] = tmp[i] - '0';
    }
    
    return num;
}

void printNum(BigInt* num){
    if(num->size == 0){
        printf("0");
        return;
    }
    if (num->sign) printf("-");
    for(int i = num->size - 1; i >=0; i--)
        printf("%d", num->nums[i]);

    free(num);
}

BigInt* Addition(BigInt* num1, BigInt* num2){
    int carry = 0, tmp, i;
    BigInt* res = NULL;
    if(num1->sign == num2->sign){
        res = (BigInt*)malloc(sizeof(BigInt));
        initialise_Number(&res);
        res->sign = num1->sign;
        int maxSz = (num1->size > num2->size) ? num1->size : num2->size;
        for (i = 0; i < maxSz; i++){
            int d1 = (i < num1->size) ? num1->nums[i] : 0;
            int d2 = (i < num2->size) ? num2->nums[i] : 0;
            tmp = d1 + d2 + carry;
            res->nums[i] = tmp % 10;
            carry = tmp / 10;
        }
        if (carry) res->nums[i++] = carry;
        res->size = i;
    }
    else if(num1->sign == 1){
        num2->sign = 1;
        res = Substraction(num1, num2);
        num2->sign = 0;
    }
    else{
        num2->sign = 0;
        res = Substraction(num1, num2);
        num2->sign = 1;
    }
    return res;
}

BigInt* Substraction(BigInt* num1, BigInt* num2){
    int borrow = 0, i;
    BigInt* res = NULL;
    if(num1->sign == num2->sign){
        res = (BigInt*)malloc(sizeof(BigInt));
        initialise_Number(&res);
        
        int maxSz = (num1->size > num2->size) ? num1->size : num2->size;
        num1->nums[maxSz] = 1;
        for (i = 0; i <= maxSz; i++){
            int diff = num1->nums[i] - num2->nums[i] + borrow;
            if (diff < 0){
                diff += 10;
                borrow = -1;
            }
            else{
                borrow = 0;
            }
            res->nums[i] = diff % 10;
        }

        if(res->nums[maxSz] == 1){ // num1 is greater.
            res->sign = num1->sign;
            res->nums[maxSz] = 0;
        }
        else{
            res->sign = (num1->sign == 0) ? 1: 0;
            res->nums[maxSz] = 0;

            res->nums[0] = 10 - res->nums[0];
            int carry = res->nums[0] / 10;
            res->nums[0] %= 10;
            for(i = 1; i < maxSz; i++){
                res->nums[i] = 9 - res->nums[i] + carry;
                carry = res->nums[i] / 10;
                res->nums[i] %= 10;
            }
        }
    }
    else if (num1->sign == 1){
        num2->sign = 1;
        res = Addition(num1, num2);
        num2->sign = 0;
    }
    else{
        num2->sign = 0;
        res = Addition(num1, num2);
        num2->sign = 1;
    }

    for (int i = MAX_SIZE - 1; i >= 0; i--) {
        if (res->nums[i] != 0) {
            res->size = i + 1;
            break;
        }
    }
    return res;
}

BigInt* Multiplication(BigInt* num1, BigInt* num2){
    BigInt* res = (BigInt*)malloc(sizeof(BigInt));
    initialise_Number(&res);
    if(num1->sign != num2->sign) res->sign = 1;
    int i = 0, j = 0, carry = 0;
    for(; i < num1->size; i++){
        carry = 0;
        for (j = 0; j < num2->size; j++){
            if(i + j >= MAX_SIZE){
                printf("!!Overflow!!");
                free(res);
                return 0;
            }
            int product = num1->nums[i] * num2->nums[j] + res->nums[i + j] + carry;
            res->nums[i + j] = product % 10;
            carry = product / 10;
        }
        if(carry) res->nums[i + j] = carry;
    }
    for(int k = MAX_SIZE - 1; k >= 0; k--){
        if(res->nums[k] != 0){
            res->size = k + 1;
            break;
        }
    }
    return res;
}

int main(){
    printf("\n========== Calculation of Big integer ==========\n");

    BigInt* num1 = scanNumber();
    if (!num1) return 1;
    BigInt* num2 = scanNumber();
    if (!num2) {
        free(num1);
        return 1;
    }

    int choice;
    do {
        printf("------------------------------------\n1. Addition\n2. Substraction\n3. Multiplication\n0. Exit\n\nEnter choice: ");
        scanf("%d", &choice);
        
        switch(choice){
            case 1: 
                printf("Addition: ");
                printNum(Addition(num1, num2));
                printf("\n");
                break;
            case 2: 
                printf("Subtraction: ");
                printNum(Substraction(num1, num2));
                printf("\n");
                break;
            case 3: 
                printf("Multiplication: ");
                printNum(Multiplication(num1, num2));
                printf("\n");
                break;
            case 0:
                free(num1);
                free(num2);
                printf("Exiting...\n");
                break;
            default: 
                printf("Invalid choice!\n");
        }
    } while (choice);

    return 0;
}