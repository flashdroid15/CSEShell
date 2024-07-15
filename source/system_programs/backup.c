#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define PATH_MAX 4096 

int shell_backup(char **args) {
    const char *backup_dir = getenv("BACKUP_DIR");
    if (backup_dir == NULL) {
        fprintf(stderr, "Error: BACKUP_DIR environment variable is not set.\n");
        return 1; 
    }

    //get the current time for the backup file name
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char archive_name[PATH_MAX];
    snprintf(archive_name, sizeof(archive_name), "./archive/backup-%04d%02d%02d%02d%02d%02d.zip",
             tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    if (access("./archive", F_OK) == -1) {
        if (mkdir("./archive", 0755) != 0) {
            perror("mkdir");
            return 1; 
        }
    }

    char command[2 * PATH_MAX + 50];
    snprintf(command, sizeof(command), "zip -r %s %s", archive_name, backup_dir);
    
    int ret = system(command);
    if (ret != 0) {
        fprintf(stderr, "Error: Failed to create backup archive.\n");
        return 1; 
    }

    printf("Backup created successfully at %s\n", archive_name);
    return 0;
}

int main(void) {
    char *cmd[] = {"backup", NULL}; 

    shell_backup(cmd);

    return 0; 
}