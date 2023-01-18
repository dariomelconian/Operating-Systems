/*
 * Dario Melconian
 * Assignment #2
 * October 12, 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {

    // port for pipe
    int port[2];

    // char array created to store the string from the pipe
    char stringPipe[100];
    char stringXY[100];
    char stringPipe2[100];

    // value to store the fork
    pid_t pid;

    // if port < 0, then catch the error
    if (pipe(port) < 0){

        perror("Pipe error");
        exit(0);
    }

    // pipe worked, so initiate X, Y, and Z variables:
    char *X;
    char *Y;
    char *Z;

    // fork after the pipe
    pid = fork();

    // if pid is < 0, then catch the error
    if (pid < 0) {

        perror("fork()");
        printf("main function: errno number is %d\n",errno);

        exit(0);
    }

    // go into parent
    if (pid > 0) {

        printf("Parent (PID %d): created child (PID %d)\n", getpid(), pid);

        // read the stringPipe char array which has the command line parameter 'X' in it.
        read(port[0], stringPipe, sizeof(stringPipe));

        // print the read
        printf("Parent (PID: %d): read from pipe \"%s\"\n", getpid(), stringPipe);

        // read second argument from command line, known as 'Y'
        Y = argv[2];

        // access 'Y' is the next step:
        printf("Parent (PID %d): received Y = \"%s\"\n", getpid(), Y);

        // use sprintf function to concat stringPipe and Y and store as stringPipe
        sprintf(stringXY, "%s %s", stringPipe, Y);

        // print the concatenation of X and Y and the result
        printf("Parent (PID %d): \"%s\" + Y  = \"%s\"\n", getpid(), stringPipe, stringXY);

        // then write it into the pipe
        printf("Parent (PID %d): writing into pipe \"%s\"\n", getpid(), stringXY);

        // actually writing it into pipe
        write(port[1], stringXY, sizeof(stringXY));

        // thus it will print child first after this point as it waits until it is done.
        wait(NULL);

        // read pipe from child
        read(port[0], stringPipe2, sizeof(stringPipe2));

        // print read
        printf("Parent (PID %d): read from pipe \"%s\"\n", getpid(), stringPipe2);

        // print all tasks completed
        printf("Parent (PID %d): all tasks completed\n", getpid());

    } // end of parent

    // go into child (pid == 0)
    else {

        // read first argument from command line, known as 'X'
        X = argv[1];

        // access X
        printf("Child (PID %d): received X = \"%s\"\n", getpid(), X);

        // write X into pipe
        printf("Child (PID %d): writing \"%s\" into pipe\n", getpid(), X);

        // write it now onto pipe
        write(port[1],X, strlen(X)+1);

        // sleep 5 seconds, so it sends control back to the parent
        sleep(5);

        // read the stringPipe char array which has the command line parameter 'X + Y' in it.
        read(port[0], stringXY, sizeof(stringXY));

        // print the read
        printf("Child (PID: %d): read from pipe \"%s\"\n", getpid(), stringXY);

        // read third argument from command line, known as 'Z'
        Z = argv[3];

        // access Z
        printf("Child (PID %d): received Z = \"%s\"\n", getpid(), Z);

        // use sprintf function to concat stringPipe (XY) and Z and store as stringPipe
        sprintf(stringPipe2, "%s %s", stringXY, Z);

        // print that current concat + Z is the final answer
        printf("Child (PID %d): \"%s\" + Z = \"%s\"\n", getpid(), stringXY, stringPipe2);

        // write into pipe
        printf("Child (PID %d): writing into pipe \"%s\"\n", getpid(), stringPipe2);

        // actually write
        write(port[1], stringPipe2, sizeof(stringPipe2));

        // all tasks completed
        printf("Child (PID %d): all tasks completed\n", getpid());

        // finished: now goes into the parent

    }  // end of child

    return 0;
}  // end of code
