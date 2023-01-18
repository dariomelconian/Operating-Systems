/*
 * Dario Melconian
 * Assignment #5
 * December 2, 2022
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *parse[10];

// store account balances
int accountBalance[10];

// mutex lock variable declaration
pthread_mutex_t LOCK;

// void method thread to complete transactions - read char string from file
void *thread_read(void *arg) {

    // variables for reading words in file
    char *array[50];
    char *line = (char *)arg;
    char *string;

    // keeps count of string
    int count = 0;

    // identifies strings as chars separated by " " space
    string = strtok(line, " ");

    // iterate through char arrays
    while (string != NULL) {

        array[count] = string;

        // separate via the " " space
        string = strtok(NULL, " ");

        // increment count of string
        count += 1;
    }

    // transaction count
    int totalTransactions = (count - 1) / 3;

    // iterate through the number of transactions
    for (int transactionCount = 0; transactionCount < totalTransactions;
         transactionCount++) {

        char *transactionType = array[(transactionCount * 3) + 1];
        char *account = array[(transactionCount * 3) + 2];

        int transaction = atoi(array[(transactionCount * 3) + 3]);
        int accountID = (int)(account[7] - '0');

        // lock before depositing/withdrawing (any type of transaction)
        pthread_mutex_lock(&LOCK);

        /* now the transaction is officially LOCKED */

        // 1) deal with 'withdraw' transaction types
        if (strcmp(transactionType, "withdraw") == 0) {

            if (accountBalance[accountID - 1] >= transaction) {

                accountBalance[accountID - 1] -= transaction;
            }
        }

            // 2) deal with 'deposit' transaction types
        else if (strcmp(transactionType, "deposit") == 0) {

            accountBalance[accountID - 1] += transaction;
        }

            // if not 'deposit' or 'withdraw'
        else {
            // if this enters, error with infile
            printf("Error transaction type.");
            break;
        }

        // deposit/withdraw finished, thus, UNLOCK
        pthread_mutex_unlock(&LOCK);

        /* now the transaction is officially UNLOCKED */
    }
    // return 0
    return 0;
} // end of method

// initial variable declaration for main
int numberOfClients = 0;
int numberOfAccounts = 0;

// enter main
int main(void) {

    // pointer for file
    FILE *pointer;

    // pointer referring to the line in file
    char *line = NULL;

    size_t length = 0;

    // open file using pointer as reference
    pointer = fopen("assignment_5_input.txt", "r");

    // in the event that file is not found
    if (pointer == NULL) {

        printf("The file is not found.\n");
        exit(EXIT_FAILURE);
    }

    // iterate through the file storing transaction info
    // while (!feof(pointer)) {
    // while (fgets(line, sizeof(line)))
    while ((getline(&line, &length, pointer)) != -1) {

        // initialize string variables
        char strArray[100];
        int stringCount = 0;
        char *readString;

        // add to the char array
        strcpy(strArray, line);
        readString = strtok(line, " ");

        // temp array to store balance
        char *tempCharArray[50];

        // iterate through file's contents until NULL
        while (readString != NULL) {

            tempCharArray[stringCount] = readString;
            readString = strtok(NULL, " ");

            // iterate length of the length of string (its count)
            stringCount = stringCount + 1;
        }

        // if its length is 3, it is an account
        if (stringCount == 3) {

            accountBalance[numberOfAccounts] = atoi(tempCharArray[2]);
            // increment # of accounts
            numberOfAccounts++;
        }

        // if its length is 4 or more, it is a client
        if (stringCount >= 4) {

            parse[numberOfClients] = malloc(sizeof(strArray));
            strcpy(parse[numberOfClients], strArray);

            // increment # of clients
            numberOfClients++;
        }
    }

    // declaring other variables required for threading
    pthread_t threads[numberOfClients];

    // create a thread on a per-client basis
    for (int a = 0; a < numberOfClients; a++) {

        pthread_create(&threads[a], NULL, &thread_read, parse[a]);
    }

    // join after creating thread
    for (int b = 0; b < numberOfClients; b++) {

        pthread_join(threads[b], NULL);
    }

    // print # of accounts as well as # of clients
    printf("No. of Accounts: %d\nNo. of Clients: %d\n", numberOfAccounts, numberOfClients);

    // print each account, with their balance
    for (int c = 0; c < numberOfAccounts; c++) {

        printf("account%i, balance: %i\n", c + 1, accountBalance[c]);
    }

    // when finished, destroy the mutex pointed by LOCK
    pthread_mutex_destroy(&LOCK);

    // return 0
    return 0;

} // end of main

