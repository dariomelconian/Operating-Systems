/*
 * CS 3305
 * Assignment #1
 * Dario Melconian
 * 251044493
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]) {

    pid_t child_1, child_2, parent;

    printf("Parent (PID %d): process started\n", getpid());

    // fork a child process
    printf("Parent (PID %d): forking child_1\n", getpid());
    child_1 = fork();

    // if the pid < 0, deal with the error
    if (child_1 < 0) {

        perror("fork()");
        printf("main function: errno number is %d\n",errno);
        exit(child_1);
    }

    // go into parent process
    if (child_1 > 0) {

        // print that the fork was successful and its PID
        printf("Parent (PID %d): fork successful for child_1 (PID %d)\n", getpid(), child_1);

        // save current pid value as 'parent'
        parent = getpid();

        // wait for process to terminate before starting child_2 fork
        printf("Parent (PID %d): waiting for child_1 (PID %d) to complete\n", getpid(), child_1);
        // initiate wait() function
        wait(NULL);

        // create child_2
        printf("Parent (PID %d) forking child_2\n", getpid());
        child_2 = fork();

        // if the pid < 0, deal with the error
        if (child_2 < 0) {

            perror("fork()");
            printf("main function: errno number is %d\n",errno);
            exit(child_2);
        }

        // go into parent process
        if (child_2 > 0) {

            // print that the fork was successful and its PID
            printf("Parent (PID %d): fork successful for child_2 (PID %d)\n", getpid(), child_2);

            // wait for process to terminate before progressing
            printf("Parent (PID %d): waiting for child_2 (PID %d) to complete\n", getpid(), child_2);
            // initiate wait() function
            wait(NULL);

        }

        // if (child_2 == 0)
        // we are now in the child_2
        else {

            // call the external program
            printf("Child_2 (PID %d): calling an external program [external_program.out]\n", getpid());

            // CALL EXTERNAL PROGRAM:

            // create char for the PID
            char child_2_PID[10];
            sprintf(child_2_PID, "%d", getpid());

            // create char for the string including the 'for child_1'
            char child_2_String[] = " for child_2";

            // concatenate the 2 char's to send to the external
            strcat(child_2_PID, child_2_String);

            // execl call to external
            execl("external_program.out", child_2_PID, argv[1], NULL);
            //execl("external_program.out", argv[1], child1PID, NULL);

            //printf("Should not see this");

        } // close the else (this is the child_2)

        // express that the child_2 has completed
        printf("Parent (PID %d) completed\n", getpid());


    } // close the if > 0 (this is the parent)

    // where PID == 0
    // go into child_1 process meaning we are inside child_1
    else {

        pid_t child_11;

        // fork child_1 into child_1.1
        printf("Child_1 (PID %d): forking child_1.1\n", getpid());
        child_11 = fork();

        // if the pid < 0, deal with the error
        if (child_11 < 0) {

            perror("fork()");
            printf("main function: errno number is %d\n",errno);

            exit(child_11);
        }

        if (child_11 > 0) {

            // print that the fork was successful and its PID
            printf("Child_1 (PID %d): fork successful for child_1.1 (PID %d)\n", getpid(), child_11);

            // wait for process to terminate before starting child_2 fork
            printf("Child_1 (PID %d): waiting for child_1.1 (PID %d) to complete\n", getpid(), child_11);
            // initiate wait() function
            wait(NULL);

        }

        // if (child_11 == 0)
        // we are now in the child_1.1
        else {

            // call external program
            printf("Child_1.1 (PID %d): calling an external program [external_program.out]\n", getpid());

            // CALL EXTERNAL PROGRAM:

            // create char for the PID
            char child_1_PID[10];
            sprintf(child_1_PID, "%d", getpid());

            // create char for the string including the 'for child_1'
            char child_1_String[] = " for child_1.1";

            // concatenate the 2 char's to send to the external
            strcat(child_1_PID, child_1_String);

            // execl call to external
            execl( "external_program.out", child_1_PID, argv[1], NULL);

            //printf("Should not see this");

        }  // close the else (this is the child_1.1)

        // express that the child_1 has completed
        printf("Child_1 (PID %d) completed\n", getpid());

    } // close the else (this is the child_1)

    return 0;
}  // end of code

