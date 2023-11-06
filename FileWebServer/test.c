#include <stdio.h>
#include <unistd.h>

int main() {
    printf("%d\n", access("WebRoot/1.png", F_OK));
}