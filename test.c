#include <stdio.h>
#include <stdlib.h>

int bar(char* s)
{
    return printf("From bar: %s\n", s);
}

void foo()
{
    printf("And from foo: %d\n", bar("test"));
}

int main()
{
    foo();
}