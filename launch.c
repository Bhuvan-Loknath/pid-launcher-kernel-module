#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

void launchProcess(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <executable_path> [args...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *executablePath = argv[1];
    char *args[argc];

    // Copy arguments, including executable path
    for (int i = 0; i < argc - 1; ++i) {
        args[i] = argv[i + 1];
    }

    args[argc - 1] = NULL;  // Null-terminate the argument array

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process
        execv(executablePath, args);
        perror("execv"); // If execv fails
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        printf("Launched process with PID: %d\n", pid);

        // Write the PID to /proc/pid for tracking
        int fd = open("/proc/pid", O_WRONLY);

        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        dprintf(fd, "%d", pid);
        close(fd);

        waitpid(pid, NULL, 0); // Wait for the child process to finish
    }
}

int main(int argc, char *argv[]) {
    // Launch the process with command-line arguments
    launchProcess(argc, argv);

    return 0;
}
