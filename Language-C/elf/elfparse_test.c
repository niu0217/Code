/* ************************************************************************
> File Name:     elfparse_test.c
> Author:        niu0217
> Created Time:  Mon 01 Jul 2024 11:30:36 AM CST
> Description:   
 ************************************************************************/

// gcc -no-pie elfparse_test.c -o elfparse_test
// gcc elfparse.c -o elfparse
// ./elfparse elfparse_test

#include <stdio.h>

void foo() {
    printf("this is function foo\n");
}

void your_name(char *filename) {
    printf("your filename is %s\n", filename);
}

int main(int argc, char **argv) {
    foo();
    your_name(argv[0]);
}
