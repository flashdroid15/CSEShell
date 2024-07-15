#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <pwd.h>
#include <string.h>

// Function to get the uptime
void print_uptime() {
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        int days = info.uptime / 86400;
        int hours = (info.uptime / 3600) % 24;
        int minutes = (info.uptime / 60) % 60;
        printf("Uptime: %d days, %d hours, %d minutes\n", days, hours, minutes);
    } else {
        perror("sysinfo");
    }
}

// Function to get the current username
void print_username() {
    struct passwd *pw = getpwuid(getuid());
    printf("User: %s\n", pw->pw_name);
}

/*
// Function to get the total memory size
void print_memory_info() {
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        printf("MemTotal: %lu KB\n", info.totalram / 1024);
    } else {
        perror("sysinfo");
    }
}
*/

/*
// Function to get CPU information
void print_cpu_info() {
    FILE *cpuinfo = fopen("/proc/cpuinfo", "rb");
    char *arg = 0;
    size_t size = 0;
    while (getdelim(&arg, &size, 0, cpuinfo) != -1) {
        char *match = strstr(arg, "model name");
        if (match) {
            char *colon = strchr(match, ':');
            if (colon) {
                printf("CPU: %s\n", colon + 2);
                break;
            }
        }
    }
    free(arg);
    fclose(cpuinfo);
}
*/

int main() {
    struct utsname buffer;

    if (uname(&buffer) != 0) {
        perror("uname");
        exit(EXIT_FAILURE);
    }

    printf("Simple System Information\n");
    printf("OS: %s\n", buffer.sysname);
    printf("Kernel: %s\n", buffer.release);
    printf("Hostname: %s\n", buffer.nodename);
	print_username();
    print_uptime();
    // print_cpu_info();
    // print_memory_info();

    return 0;
}
