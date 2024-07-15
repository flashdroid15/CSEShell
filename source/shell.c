// Include the shell header file for necessary constants and function declarations
#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <time.h>
#include <dirent.h> //this includes the directory entry library for the 'ld' command 
#include <string.h>

#define HISTORY_SIZE 100 

char *history[HISTORY_SIZE];
int history_count = 0; 

void read_command (char **cmd)
{
  char *input = NULL;
  size_t len = 0; //size_t is an unsigned integer type
  getline(&input, &len, stdin);

  if(input == NULL){
    cmd[0] = NULL;
    return; 
  }

  if(strlen(input) > 0) {
    if (history_count < HISTORY_SIZE) {
      history[history_count++] = strdup(input);
    } else {
      free(history[0]);
      for (int i = 1; i < HISTORY_SIZE; i++) {
        history[i-1] = history[i];
      }
      history[HISTORY_SIZE-1] = strdup(input);
    }
  }

    // Parse the input into command tokens
    int i = 0;
    char *token = strtok(input, " \n"); //the input string is split into tokens
    while (token!=NULL) {
      cmd[i++] = strdup(token); //the token is duplicated and stored in command
      token = strtok(NULL, " \n"); //retrieve the next token
    }
    cmd[i] = NULL; //this indicates the end of cmd
    free(input); //the input buffer memory is erased
  }


// // Function to read a command from the user input
// void read_command(char **cmd)
// {
//   // Define a character array to store the command line input
//   char line[MAX_LINE];
//   // Initialize count to keep track of the number of characters read
//   int count = 0, i = 0;
//   // Array to hold pointers to the parsed command arguments
//   char *array[MAX_ARGS], *command_token;

//   // Infinite loop to read characters until a newline or maximum line length is reached
//   for (;;)
//   {
//     // Read a single character from standard input
//     int current_char = fgetc(stdin);
//     // Store the character in the line array and increment count
//     line[count++] = (char)current_char;
//     // If a newline character is encountered, break out of the loop
//     if (current_char == '\n')
//       break;
//     // If the command exceeds the maximum length, print an error and exit
//     if (count >= MAX_LINE)
//     {
//       printf("Command is too long, unable to process\n");
//       exit(1); //this indicates the program termninated with an error 
//     }
//   }
//   // Null-terminate the command line string
//   line[count] = '\0';

//   // If only the newline character was entered, return without processing
//   if (count == 1)
//     return;

//   // Use strtok to parse the first token (word) of the command
//   command_token = strtok(line, " \n");

//   // Continue parsing the line into words and store them in the array
//   while (command_token != NULL)
//   {
//     array[i++] = strdup(command_token);  // Duplicate the token and store it
//     command_token = strtok(NULL, " \n"); // Get the next token
//   }

//   // Copy the parsed command and its parameters to the cmd array
//   for (int j = 0; j < i; j++)
//   {
//     cmd[j] = array[j];
//   }
//   // Null-terminate the cmd array to mark the end of arguments
//   cmd[i] = NULL;
// }

// Function to display the shell prompt
void type_prompt()
{
  // Use a static variable to check if this is the first call to the function
  static int first_time = 1;
  if (first_time)
  {
    // Clear the screen on the first call
#ifdef _WIN32
    system("cls"); // Windows command to clear screen
#else
    system("clear"); // UNIX/Linux command to clear screen
#endif
    first_time = 0;
  }

  char cwd[1024]; //buffer variable stores the current working directory
  time_t t = time(NULL); //gets the current time
  struct tm tm = *localtime(&t); //convert current time to local time format

  if (getcwd(cwd, sizeof(cwd))!=NULL){
    char time_str[26]; //buffer varaibale stores the formatted time string 
    strftime(time_str, sizeof(time_str), "%a %b %d %H:%M:%S %Y", &tm); //time formatted

    printf(">> %s %s >> CSEShell\n↪ ", cwd, time_str);

    //printf(">> %s \n%s %02d %02d %02d:%02d:%02d %d >> CSEShell\n↪ ",
          /*  cwd,
            asctime(&tm), //current time in human readable format
            tm.tm_mday, //today's day
            tm.tm_mon + 1, //current month
            tm.tm_hour, //current hour
            tm.tm_min, //current minute
            tm.tm_sec, //current seond
            tm.tm_year + 1900); //current year */          
  } else {
    perror("getcwd() error"); //error printed for fialed getcwd 
  } 
  fflush(stdout); // Flush the output buffer
  //printf("$$ ");  // Print the shell prompt
}


// dspawn
int shell_dspawn(char **args)
{
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    } else if (pid == 0) {
        execvp("./bin/dspawn", args); // Adjust the path to the location of your dspawn executable
        perror("Execution failed");
        exit(1);
    } else {
        waitpid(pid, NULL, 0);
    }

    return 1;
}

// dcheck
int shell_dcheck(char **args) {
    pid_t pid;
    int status;
    if ((pid = fork()) < 0) {
        perror("Fork failed");
        return 1;
    } else if (pid == 0) {
        execvp("./bin/dcheck", args);
        perror("Execution failed");
        exit(1);
    } else {
        waitpid(pid, &status, WUNTRACED);
    }
    return 1;
}

int shell_exit(char **args) {
  //the memory stored in history buffer is erased
  for (int i = 0; i < history_count; i++) {
     free(history[i]);
  }
  return 0; //shell should exit 
}

int shell_history(char **args) {
  for (int i = 0; i < history_count; i++) {
    printf("%d %s", i+1, history[i]);
  }
  return 1; 
}

int shell_cd(char **args) {    //this function handles the 'cd' command by taking an array of character strings as the arugment
  if (args[1] == NULL) { // Checking if the next directory path argument is provided
    fprintf(stderr, "CSEShell: expected argument to \"cd\"\n"); //error message printed if no argument pcdrovided. stderr is standard error
  } else {
      if (chdir(args[1])!=0) { //checks whether the attempt to change the current working directory to args[1] fails
        perror("CSEShell"); //prints error message to stderr
      }
  }
  return 1; //this indicates the shell should continue running 
}

int shell_help(char **args) { //handles the 'help' command
  printf("CSEShell Interface \n"); //print a list of built-in commands
  printf("Usage: Command arguments \n");
  printf("The following commands are implemented within the shell:\n");
  for (int i =0; i < num_builtin_functions(); i++) { //print each built-in command
    printf("  %s\n", builtin_commands[i]);
   }
  return 1; //the shell should continue running 
  }

// int shell_exit(char **args){ //handles the 'exit' command
//   return 0; //indicates that the shell should exit
// }


int shell_usage(char **args) { //handles the 'usage' command
  if (args[1] == NULL) { //no command input
  printf("Command not given. Type usage <command>.\n");
} else {
  if( strcmp(args[1],"cd") == 0) {
    printf("Type: cd directory_name to change the current working directory of the shell\n");
  } else if (strcmp(args[1],"help") == 0) {
    printf("Type: help for supported commands\n");
  } else if (strcmp(args[1],"exit") == 0) {
    printf("Type: exit to terminate the shell gracefully\n");
  } else if (strcmp(args[1],"usage") == 0) {
    printf("Type: usage <command> to get usage information for a specific commands\n");
  } else if (strcmp(args[1],"env") == 0) {
    printf("Type: env to list all registered env variables\n");
  } else if (strcmp(args[1],"setenv") == 0) {
    printf("Type: setenv ENV=VALUE to set a new env variable\n");
  } else if (strcmp(args[1],"unsetenv") == 0) {
    printf("Type: unsetenv ENV to remove this new env from the list of env variables\n");
  } 
   else {
    printf("The command you gave: %s, is not part of CSEShell's builtin command\n", args[1]);
   }
}
return 1; 
}

int list_env(char **args) { //handles 'env' command
  extern char **environ; //creating an external char array environment variabl called environ
  for(char **env = environ; *env != 0; env++){ //loop through each environment variable
    printf("%s\n", *env);
  }
  return 1; 
}

int set_env_var(char **args) { //'setenv' command
  if(args[1] == NULL || strchr(args[1], '=') == NULL) { //strchr(args[1], "=") checks whether the string "=" is present in args[1] or not 
    fprintf(stderr, "shell: expected arguments to \"setenv\"\n"); //fprintf() prints the content on the file and not on the stderr output console
  } else {
    char *env_var = strtok(args[1], "=");  //splits the given argument at '='
    char *env_value = strtok(NULL, "="); //retreives the value part after '='
    if(setenv(env_var, env_value, 1)!=0){ //setenv changes the environment variable while allowing overwrite
      perror("shell"); //error message printed if setenv fails
    }
  }
  return 1; //shell continues running 
}

int unset_env_var(char **args) { //'unsetenv' command
  if(args[1]==NULL){
    fprintf(stderr, "shell: expected argument to unsetenv\n");
  } else {
    if(unsetenv(args[1])!= 0){ //unsetenv() unsets an environment variable set by the setenv() function
      perror("shell"); 
    }
  }
  return 1;
}

int num_builtin_functions() {
  return sizeof(builtin_commands)/sizeof(char *); //returns the number of built-in commands. Also, sizeof(char *) returns the size of a single char element '*'
}

// .rc

void parse_input(char *input, char **args) { 
    for (int i = 0; i < MAX_NUM_ARGUMENTS; i++) { 
        args[i] = strsep(&input, " "); 
        if (args[i] == NULL) break; 
    } 
} 
 
// Function to execute a command 
// void execute_command(char **args) { 
//     if (strcmp(args[0], "sys") == 0) { 
//         char *sys_path = "./system_programs/sys"; // Adjust path as necessary 
//         execvp(sys_path, args); // Execute sys with arguments 
//         perror("execvp failed"); // Print error if execvp fails 
//         _exit(EXIT_FAILURE); // Exit child process with failure status 
//     } else if (strcmp(args[0], "dspawn") == 0) { 
//         char *dspawn_path = "./system_programs/dspawn"; // Adjust path as necessary 
//         execvp(dspawn_path, args); // Execute dspawn with arguments 
//         perror("execvp failed"); // Print error if execvp fails 
//         _exit(EXIT_FAILURE); // Exit child process with failure status 
//     } else if (strcmp(args[0], "dcheck") == 0) { 
//         if (args[1] == NULL) { 
//             fprintf(stderr, "Usage: dcheck <daemon-pid>\n"); 
//         } else { 
//             char *dcheck_path = "./system_programs/dcheck"; // Adjust path as necessary 
//             execvp(dcheck_path, args); // Execute dcheck with arguments 
//             perror("execvp failed"); // Print error if execvp fails 
//             _exit(EXIT_FAILURE); // Exit child process with failure status 
//         } 
//     } else if (strcmp(args[0], "backup") == 0) { 
//         if (args[1] == NULL) { 
//             fprintf(stderr, "Usage: backup <directory>\n"); 
//         } else { 
//             char *backup_path = "./system_programs/backup"; // Adjust path as necessary 
//             execvp(backup_path, args); // Execute backup with arguments 
//             perror("execvp failed"); // Print error if execvp fails 
//             _exit(EXIT_FAILURE); // Exit child process with failure status 
//         } 
//     } else { 
//         // Execute using execvp for system commands 
//         pid_t pid = fork(); 
//         if (pid == 0) { 
//             execvp(args[0], args); 
//             perror("execvp failed"); 
//             _exit(EXIT_FAILURE); 
//         } else { 
//             wait(NULL); 
//         } 
//     } 
// } 

void execute_command(char **cmd, char *cwd) {
  int child_status;
  pid_t pid;
  char full_path[PATH_MAX] = {0};

  int commandIndex;
  int isBuiltin = 0; 
  for (commandIndex = 0; commandIndex < num_builtin_functions(); commandIndex++) {
    if(strcmp(cmd[0], builtin_commands[commandIndex])==0){ //checking if the command matches a built-int command
        isBuiltin = 1; //sets flag to 1 to indiacte a built-in command
        break;
      }
    }
 if (isBuiltin) {
      int result = (*builtin_command_func[commandIndex])(cmd); // using function pointer call the corresponding built-in command handler
      if (strcmp(cmd[0], "exit") == 0 && result == 0){ //if the command is 'exit' and the handler resturns 0, then simply exit the shell
        return; 
      }
    }
    
    pid = fork();
    if(pid<0) {
      perror("Fork failed");
      exit(1);
    }
    if (pid==0) {
      printf("%s\n", full_path);
      snprintf(full_path, sizeof(full_path), "%s/bin/%s", cwd, cmd[0]);
      execv(full_path, cmd); //execvp() command runs the program located at 'full_path' with the arguments present in cmd. It replaces the current process with a new program. If successful it never returns, otherwise an error is returned
      execvp(cmd[0],cmd);
      printf("Command %s not found\n", cmd[0]); // If execv returns, command execution has failed
      exit(1);}
     else {
      waitpid(pid, &child_status, WUNTRACED); //it makes the parent process wait until the child process finishes execution. It stores the status of the child process with process ID pid inside the child_status variable 
        if (WIFEXITED(child_status)){
          int child_exit_status = WEXITSTATUS(child_status);
          if (child_exit_status!= 1) {
             /*   printf("Child exited with status: %d\n", child_exit_status);    */
          } } 
        else if (WIFSIGNALED(child_status)) {
          printf("Child process terminated with a signal: %d\n", WTERMSIG(child_status));
        } else if (WIFSTOPPED(child_status)) {
          printf("Child process stopped by a signal: %d\n", WSTOPSIG(child_status));
        } else {
          printf("Child process did not terminate normally.\n");
        }
    }
}
 
// Function to process the .cseshellrc file 
void process_rc_file(char *cwd) { 
    FILE *file = fopen(".cseshellrc", "r"); 
    if (file == NULL) { 
        perror("Failed to open .cseshellrc"); 
        return; 
    } 
 
    char line[MAX_COMMAND_LENGTH]; 
    while (fgets(line, sizeof(line), file)) { 
        // Remove newline character 
        line[strcspn(line, "\n")] = '\0'; 
 
        // Check if the line is setting the PATH 
        if (strncmp(line, "PATH=", 5) == 0) { 
            setenv("PATH", line + 5, 1); 
        } else if (line[0] != '\0') { 
            // Parse and execute the command 
            char *args[MAX_NUM_ARGUMENTS]; 
            parse_input(line, args); 
            execute_command(args, cwd); 
        } 
    } 
 
    fclose(file); 
}







// sys
int shell_sys(char **args) {
    pid_t pid;
    int status;

    if ((pid = fork()) < 0) {
        perror("Fork failed");
        return 1;
    } else if (pid == 0) {
        execvp("./bin/sys", args);
        printf("Execution failed\n");
        exit(1);
    } else {
        waitpid(pid, &status, WUNTRACED);
    }

    return 1;
}

// The main function where the shell's execution begins
int main(void)
{
  // Define an array to hold the command and its arguments
  char *cmd[MAX_ARGS]; 
  int child_status;
  pid_t pid;


  char full_path[PATH_MAX] = {0};
  char cwd[1024];   //creating a char array to store the path of the current working directory. It has a size of 1024 characters
  if (getcwd(cwd, sizeof(cwd)) == NULL) {   //getcwd attempts to get the current working directory and store it in the cwd array
    perror("Failed to get the current working directory"); //getcwd fails
    exit(1);
  }
  process_rc_file(cwd);
  
  while(1){
    type_prompt();     // Display the prompt
    read_command(cmd); // Read a command from the user

    if (cmd[0] == NULL) //Skip empty command
     continue;

    int commandIndex;
    int isBuiltin = 0; 
    for (commandIndex = 0; commandIndex < num_builtin_functions(); commandIndex++) {
      if(strcmp(cmd[0], builtin_commands[commandIndex])==0){ //checking if the command matches a built-int command
          isBuiltin = 1; //sets flag to 1 to indiacte a built-in command
          break;
        }
    }

    if (isBuiltin) {
      int result = (*builtin_command_func[commandIndex])(cmd); // using function pointer call the corresponding built-in command handler
      if (strcmp(cmd[0], "exit") == 0 && result == 0){ //if the command is 'exit' and the handler resturns 0, then simply exit the shell
        return 0; 
      }
      continue; // skip the loop for built-in commands
    }


    
    pid = fork();
    if(pid<0) {
      perror("Fork failed");
      exit(1);
    }
    if (pid==0) {
      /*execvp(cmd[0], cmd); //replaces one process with another where cmd[0] is the command to be executed from the array of commands called cmd. e.g. cmd[0] = 'ls'
      perror("Command not found"); //command execution failed when execvp returns
      exit(1);
    }*/
      // char full_path[PATH_MAX];
      // char cwd[1024];   //creating a char array to store the path of the current working directory. It has a size of 1024 characters
      // if (getcwd(cwd, sizeof(cwd)) != NULL) {   //getcwd attempts to get the current working directory and store it in the cwd array
      //   snprintf(full_path, sizeof(full_path), "%s/bin/%s", cwd, cmd[0]); //creates full path to the command by concatenating cwd, the /bin/ directory, and command name {cmd[0]}. It gets stored in the full_path array
      // } else {
      //   perror("Failed to get the current working directory"); //getcwd fails
      //   exit(1);
      // }
      printf("%s\n", full_path);
      snprintf(full_path, sizeof(full_path), "%s/bin/%s", cwd, cmd[0]);
      execv(full_path, cmd); //execvp() command runs the program located at 'full_path' with the arguments present in cmd. It replaces the current process with a new program. If successful it never returns, otherwise an error is returned
      execvp(cmd[0],cmd);
      printf("Command %s not found\n", cmd[0]); // If execv returns, command execution has failed
      exit(1);}
     else {
      waitpid(pid, &child_status, WUNTRACED); //it makes the parent process wait until the child process finishes execution. It stores the status of the child process with process ID pid inside the child_status variable 
        if (WIFEXITED(child_status)){
          int child_exit_status = WEXITSTATUS(child_status);
          if (child_exit_status!= 1) {
             /*   printf("Child exited with status: %d\n", child_exit_status);    */
          } } 
        else if (WIFSIGNALED(child_status)) {
          printf("Child process terminated with a signal: %d\n", WTERMSIG(child_status));
        } else if (WIFSTOPPED(child_status)) {
          printf("Child process stopped by a signal: %d\n", WSTOPSIG(child_status));
        } else {
          printf("Child process did not terminate normally.\n");
        }
    }
 // Free the allocated memory for the command arguments before exiting
  for (int i = 0; cmd[i] != NULL; i++)
    {
      free(cmd[i]);
    }
  }
     return 0;
} 
