#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

char output_file_path[256];

int daemon_work()
{
    // put your full PROJECT_DIR path here  
    // strcpy(output_file_path, "/home/roshansamueltms14/Programming Assignment 1/PA1 Roshan v3/dspawn.log"); 
    getcwd(output_file_path, sizeof(output_file_path));
    strcat(output_file_path, "/dspawn.log");

    int num = 0;
    FILE *fptr;
    char *cwd;
    char buffer[1024];

    // write PID of daemon in the beginning
    fptr = fopen(output_file_path, "a");
    if (fptr == NULL)
    {
        return EXIT_FAILURE;
    }

    fprintf(fptr, "Daemon process running with PID: %d, PPID: %d, opening logfile with FD %d\n", getpid(), getppid(), fileno(fptr));

    // then write cwd
    cwd = getcwd(buffer, sizeof(buffer));
    if (cwd == NULL)
    {
        perror("getcwd() error");
        return 1;
    }

    fprintf(fptr, "Current working directory: %s\n", cwd);
    fclose(fptr);

    while (1)
    {

        fptr = fopen(output_file_path, "a");

        if (fptr == NULL)
        {
            return EXIT_FAILURE;
        }

        fprintf(fptr, "PID %d Daemon writing line %d to the file.  \n", getpid(), num);
        num++;

        fclose(fptr);

        sleep(10);

        if (num == 10) // we just let this process terminate after 10 counts
            break;
    }

    return EXIT_SUCCESS;
}

void daemonize()
{
    pid_t pid, sid;

    // Fork the parent process
    pid = fork();

    // If we got a bad PID, terminate
    if (pid < 0)
        exit(EXIT_FAILURE);

    // If we got a good PID, then exit the parent process
    if (pid > 0)
        exit(EXIT_SUCCESS);

    // Change the file mode mask
    umask(0);

    // Create a new SID for the child process
    sid = setsid();
    if (sid < 0)
        exit(EXIT_FAILURE);

    // Change the current working directory
    if ((chdir("/")) < 0)
        exit(EXIT_FAILURE);

    // Close out the standard file descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Open the log file
    int fd = open("/tmp/dspawn.log", O_RDWR | O_CREAT | O_APPEND, 0600);
    if (fd < 0)
    {
        // If the log file cannot be opened, log an error and exit
        exit(EXIT_FAILURE);
    }

    // Redirect standard input/output/error to the log file
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);

    // Daemon-specific initialization goes here

    // The Big Loop
    while (1)
    {
        daemon_work();

    }

    close(fd);
}


int main(int argc, char *argv[])
{
    daemonize();

    return 0;
}
