/*
 * Dario Melconian
 * Assignment #3 - CS 3305A
 * October 25, 2022
 * 251044493
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

// port for pipe
int port[2];

// char array created to store the string from the pipe
char stringXY[100];
int S;

// executed by thread 1
void *subtract(void *thread_id) {

    // inside thread 1
    int *thread_id_1 = (int*)thread_id;

    // read X and Y from pipe: read the stringXY char array which has the command line parameter 'X + Y' in it.
    read(port[0], stringXY, sizeof(stringXY));

    // break the stringXY read into 2 integers
    const char s[2] = " ";
    char *token;
    char *token2;

    // get the 1st token
    token = strtok(stringXY, s);
    // get the 2nd token
    token2 = strtok(NULL, s);

    printf("Thread (TID %d) reads X = %s and Y = %s from the pipe\n", *thread_id_1, token, token2);

    // perform the subtraction
    int a;
    int b;
    a = atoi(token);
    b = atoi(token2);
    S = a - b;

    // actually write it
    write(port[1], &S, sizeof(S));

    // print it will be written to pipe
    printf("Thread (TID %d) writes X-Y = %d to the pipe\n", *thread_id_1, S);

    return NULL;
}  // end of thread 1


// executed by thread 2
void *prime_check(void *thread_id) {

    // inside thread 1
    int *thread_id_2 = (int*)thread_id;

    // read from thread_1
    read(port[0], &S, sizeof(S));

    printf("Thread (TID %d) reads X-Y = %d from the pipe\n", *thread_id_2, S);

    // perform prime number calculation below
    int iterator;
    int divisor = 0;

    for (iterator = 1; iterator <= S; iterator ++) {
        // check if it 0 remainder, then go to the next divisible
        if (S % iterator == 0) {
            divisor ++;
        }
    }

    // if c==2 by the end, there was nothing that went into the number other than 1, so it is prime
    if (divisor == 2) {
        printf("Thread (TID %d) identified that %d is a prime number\n", *thread_id_2, S);
    }
    else {
        printf("Thread (TID %d) identified that %d is NOT a prime number\n", *thread_id_2, S);
    }

    // write to pipe now
    write(port[1], &S, sizeof(S));

    // print that it will be written to pipe
    printf("Thread (TID %d) writes %d to the pipe\n", *thread_id_2, S);

    return NULL;
}  // end of thread 2


// executed by thread 3
void *reverse_num(void *thread_id) {

    // inside thread 1
    int *thread_id_3 = (int*)thread_id;

    // read from thread_1
    read(port[0], &S, sizeof(S));

    printf("Thread (TID %d) reads X-Y = %d from the pipe\n", *thread_id_3, S);

    // perform the reversal of the number S
    int reverseNumber = 0;
    int remainder;

    while (S != 0) {
        remainder = S % 10;
        reverseNumber = reverseNumber * 10 + remainder;
        S /= 10;
    }

    // print the reverse of the number
    printf("Thread (TID %d) reversed number is %d\n", *thread_id_3, reverseNumber);

    return NULL;
}  // end of thread 3*/


/*
 * MAIN BEGINS
 */

// accept 2 integers (X, Y), where they adhere to following conditions: X, Y > 0, X>Y
int main(int argc, char *argv[]) {

    // if port < 0, then catch the error
    if (pipe(port) < 0){

        perror("Pipe error");
        exit(0);
    }

    // pipe worked, so initiate X, Y, and Z variables:
    char *X;
    char *Y;

    // inside parent
    X = argv[1];
    Y = argv[2];

    // check to make sure X > Y and they are both integers > 0
    int checker1 = atoi(X);
    int checker2 = atoi(Y);

    // I made them casted using atoi() to integers just for the check to ensure they meet the conditions
    if ((checker1 < checker2) || (checker1 < 1) || (checker2 < 1)) {

        exit(1);
    }

    printf("Parent (PID %d) receives X = %s and Y = %s from the user\n", getpid(), X, Y);

    // write X and Y into pipe
    printf("Parent (PID %d) writes X = %s and Y = %s to the pipe\n", getpid(), X, Y);

    // initialize the thread integers TID
    int thread_id_1 = 1;
    int thread_id_2 = 2;
    int thread_id_3 = 3;

    // parent initializes 3 threads of type pthread_t
    pthread_t thread_1;
    pthread_t thread_2;
    pthread_t thread_3;

    // use sprintf function to concat stringPipe and Y and store as stringPipe
    sprintf(stringXY, "%s %s", X, Y);

    // write X and Y into pipe
    write(port[1],stringXY, strlen(stringXY));

    // create the thread
    pthread_create(&thread_1, NULL, subtract, &thread_id_1);
    // go into thread_1
    pthread_join(thread_1, NULL);

    pthread_create(&thread_2, NULL, prime_check, &thread_id_2);
    // go into thread_2
    pthread_join(thread_2, NULL);

    pthread_create(&thread_3, NULL, reverse_num, &thread_id_3);
    // go into thread_3
    pthread_join(thread_3, NULL);

    return 0;
}  // end of main
