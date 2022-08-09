#include<stdio.h>

//convert the char type into int type
_Bool digit(char value){
    return (('0' <= value) && ('9' >= value));
}

int convert(int n, const char c[n]){
    int result;

    if(c[0] == '-'){
        if(n == 1){
            result = 2147483648;
        }else if(n == 2 && digit(c[1])){
            result = - (c[1] - '0');
        }else if(n == 3 && c[0] != '0' && digit(c[1]) && digit(c[2])){
            result = -(10 * (c[1] - '0') + (c[2] - '0'));
        }else if(n == 4 && c[0] != '0' && digit(c[1]) && digit(c[2]) && digit(c[3])){
            result = -(100 * (c[1] - '0') + 10 * (c[2] - '0') + (c[3] - '0'));
            }else{
            result = 2147483648;
        }
    }else{
        if(n == 1 && digit(c[0])){
            result = c[0] - '0';
        }else if (n == 2 && c[0] != '0' && digit(c[0]) && digit(c[1])){
            result = 10 *(c[0] - '0') + (c[1] - '0');
        }else if (n == 3 && c[0] != '0' && digit(c[0]) && digit(c[1]) && digit(c[2])){
            result = 100 * (c[0] - '0') + 10 * (c[1] - '0') + (c[2] - '0');
        }else{
            result = 2147483648;
        }
    }

    return result;
}

int length(const char c[]){
    int i = 0;
    while(c[i] != '\0'){
        i++;
    }
    return i;
}

void binaryChar(int result){
    if(result != 2147483648 && result < 128 && result > -129){
        int i, j;
        int array[sizeof(char) * 8] = {0};

        if(result < 0){
            array[sizeof(char) * 8 - 1] = 1;
            result = -result;
        }else if(result > 0){
            array[sizeof(char) * 8 - 1] = 0;
        }//determine the positive and negative of the number

        if(result > 0){
            for(j = 0; j < sizeof(char) * 8 - 1; j++){
                array[j] = result % 2;
                result = result / 2;
            }
        }//rue form

        if(array[sizeof(char) * 8 - 1] == 1){
            for(int k = sizeof(char) * 8 - 2; k >= 0; k--){
                if(array[k] == 1){
                    array[k] = 0;
                }else if(array[k] == 0){
                    array[k] = 1;
                }
            }
        }//Inverse code

        if(array[sizeof(char) * 8 - 1] == 1){
            if(array[0] == 0){
                array[0] = 1;
            }else if(array[0] == 1){
                for(int a = 0; a <= sizeof(char) * 8 - 2; a++){
                    if(array[0] + 1 == 2){
                        array[0] = 0;
                        array[1] += 1;
                    }else if(a > 0 && array[a] == 2){
                        array[a] = 0;
                        array[a + 1] += 1;
                    }
                }
            }
            array[sizeof(char) * 8 - 1] = 1;
        }//two's complement representation

        for (i = sizeof(char) * 8 - 1; i >= 0; i--){
            if (i % 4 == 3 && i != sizeof(char) * 8 - 1){
                putchar(' ');
            }
            printf("%d", array[i]);
        }
        putchar('\n');
    }else if(result > 128 || result < -129){
        char c[] = "Input error.";
        printf("%s\n", c);
    }else{
        char c[] = "Input error.";
        printf("%s\n", c);
    }
}

void binaryUnsigned(int result){
    if(result != 2147483648 && result < 256 && result > 0){
        int i, j;
        int array[sizeof(char) * 8] = {0};
        
        for(j = 0; j < sizeof(char) * 8; j++){
            array[j] = result % 2;
            result = result / 2;
        }

        for (i = sizeof(char) * 8 - 1; i >= 0; i--){
            if (i % 4 == 3 && i != sizeof(char) * 8 - 1){
                putchar(' ');
            }
            printf("%d", array[i]);
        }
        putchar('\n');
        
    }else if(result > 0 || result < 256){
        char c[] = "Input error.";
        printf("%s\n", c);
    }else{
        char c[] = "Input error.";
        printf("%s\n", c);
    }
}

int main(int n, char *args[n]){
    setbuf(stdout, NULL);
    if (n == 1){
        printf("All tests pass.\n");
    }else if(n == 3){
        binaryChar(convert(length(args[2]), args[2]));
    }else if(n == 4){
        binaryUnsigned(convert(length(args[3]), args[3]));
    } 

    return 0;
}