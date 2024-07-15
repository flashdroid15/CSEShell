#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *fp;
    char path[1035];
    int count = 0;

    /* Open the command for reading. */
    // fp = popen("ps -e | grep dspawn | grep -v grep", "r");
	fp = popen("ps -efj | grep dspawn  | grep -Ev 'tty|pts'", "r");
    if (fp == NULL) {
        printf("Failed to run command\n");
        exit(1);
    }

    /* Read the output a line at a time - output it. */
    while (fgets(path, sizeof(path), fp) != NULL) {
        count++;
    }

    /* close */
    pclose(fp);

    printf("Number of live daemons: %d\n", count);
    return 0;
}
