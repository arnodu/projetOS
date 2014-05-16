#include <stdio.h>
#include <signal.h>

/* number of times the handle will run: */

void handle(int sig) {
    printf("Hello\n");
    alarm(1);
}

int main() {
    signal(SIGALRM, handle);
    alarm(1);
    while(1);
    printf("done\n");
    return 0;
}
