#include <stdio.h>
#include <string.h>

void foo(char* arg)
{
    char buf[20];
    strcpy(buf, arg);
    printf("Copied string is %s\n", buf);
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        puts("Missing argument");
        return -1;
    }
    foo(argv[1]);
}
