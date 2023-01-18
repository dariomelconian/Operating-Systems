/*
 * Dario Melconian
 * Assignment #4
 * November 9th, 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define CAPACITY 1000

typedef struct Process_Str {
    int label;
    int burst_time;
    int current_burst_time;
    int arrival_time;
} Process;

Process * create_process(int label, int burst_time, int arrival_time) {
    size_t size = sizeof(int)*4;
    Process *process = (Process *) malloc(sizeof(Process));
    process -> label = label;
    process -> burst_time = burst_time;
    process -> current_burst_time = 0;
    process -> arrival_time = arrival_time;
    return process;
}

void destroy_process(Process * process) {
    free(process);
}

typedef struct Queue_Str {
    size_t size;
    Process** elements;
} Queue;

typedef struct Stack_Str {
    size_t size;
    Process** elements;
} Stack;

Queue * create_queue() {
    Queue * queue = (Queue *) malloc(sizeof (Queue));
    queue -> size = 0;
    queue -> elements = (Process **) malloc(CAPACITY * sizeof(Process *));
    return queue;
}

void destroy_queue(Queue * queue) {
    free(queue -> elements);
    free(queue);
}

Stack * create_stack() {
    Stack * stack = (Stack *) malloc(sizeof (Stack));
    stack -> size = 0;
    stack -> elements = (Process **) malloc(CAPACITY * sizeof(Process *));
    return stack;
}

void destroy_stack(Stack * stack) {
    free(stack -> elements);
    free(stack);
}

void enqueue(Queue * queue, Process * process) {
    (queue -> elements)[queue -> size] = process;
    queue -> size++;
}

Process * dequeue(Queue * queue) {
    if (queue -> size > 0) {
        Process * process = queue -> elements[0];
        for(int i = 1; i < queue -> size; i++) {
            queue -> elements[i-1] = queue -> elements[i];
        }
        queue -> size--;
        return process;
    }
    else {
        printf("Error: Cannot dequeue an empty queue");
    }
    return 0;
}

void push(Stack * stack, Process * process) {
    stack -> elements[stack -> size] = process;
    stack -> size++;
}

Process * pop(Stack * stack) {
    if (stack -> size > 0) {
        stack -> size--;
        return stack->elements[stack -> size];
    }
    else {
        printf("Error: Cannot pop an empty stack");
    }
    return 0;
}

Process * peek(Stack * stack) {
    if (stack -> size > 0) {
        return stack->elements[stack -> size-1];
    }
    else {
        printf("Error: Stack is empty");
    }
    return 0;
}



int main() {

    size_t buffer_size = 255;
    char line[255];
    char* l = line;

    FILE * fpointer = fopen("rr_input.txt", "r");

    int testCaseNumber = 1;

    while(getline(&l, &buffer_size, fpointer) != -1){

        // determine the number of processes to determine the size of the array
        // read  line from the file and store it in 'line' variable
        // getline(&line, 255, fpointer);
        printf("Test case #%d: %s", testCaseNumber, line);
        char temp[255];

        // copy it together
        strcpy(temp, line);

        char *determineAmount = strtok(temp, " ");
        int numOfProcesses = 0;

        while(determineAmount != NULL){
            if(strchr(determineAmount, 'p') || strchr(determineAmount, 'P')){
                numOfProcesses++;
            }
            determineAmount = strtok(NULL, " ");
        }

        // get values into the arrays
        int determine = 1;
        int processCount = 0;
        int arrivalCount = 0;
        int burstCount = 0;

        // variable that stores the 'quantum time', last element in the line
        int quantumTime;

        // storing the process number, the arrival time, and its burst time
        char *processes[numOfProcesses]; // Stores all the process
        int arrivalTime[numOfProcesses]; // Stores the arrival times
        int burstTime[numOfProcesses]; // Stores the burst times

        char *parse = strtok(line, " "); // Split line by spaces and check each token and place it array

        // run through while parse tokens still exist
        while(parse != NULL) {

            // if token is a p- process
            if(strchr(parse, 'p')){
                // add to the process array
                processes[processCount] = parse;
                processCount++;
                determine++;

            } else if(determine % 2 == 0) {
                // if the token is 'arrival time'
                arrivalTime[arrivalCount] = atoi(parse);
                arrivalCount++;
                determine++;

            } else if(determine % 3 == 0){
                // if token at 'burst time'
                burstTime[burstCount] = atoi(parse);
                burstCount++;
                determine = 1;

            } else {
                // token at 'quantum time'
                quantumTime = atoi(parse);
            }
            parse = strtok(NULL, " ");
            // end of test case parsing
        }
        printf("Number of Processes: %d, Quantum: %d\nProcess Scheduling Started:\n", numOfProcesses, quantumTime);

        Process * processArray[numOfProcesses];

        for (int i = 0; i < numOfProcesses; i++) {
            processArray[i] = create_process(i+1, burstTime[i], arrivalTime[i]);
        }

        Stack * arrivalStack = create_stack();
        for (int i = 0; i < numOfProcesses; i++) {
            push(arrivalStack, processArray[numOfProcesses - i - 1]);
        }

        Queue * rr_queue = create_queue();

        int current_time = 0;
        int current_quantum = -1;
        Process * process = NULL;

        int totalWaitingTime = 0;
        int totalTurnaroundTime = 0;

        // set to false
        int continueTriggered = 0;

        while (arrivalStack -> size > 0 || rr_queue ->size > 0 || 
            process -> current_burst_time < process ->burst_time) {

            if (!continueTriggered) {

                printf("CPU Time %d: ", current_time);

            }
            else {
                // reset it to 0
                continueTriggered = 0;
            }

            // has a new process arrived?
            while (arrivalStack -> size > 0 &&  peek(arrivalStack) -> arrival_time == current_time) {

                // add them all to the queue
                Process * arriveProcess =  pop(arrivalStack);
                enqueue(rr_queue, arriveProcess);
                printf("[p%d Arrived] ", arriveProcess -> label);
            }

            // Is a process being served?
            if (current_quantum != -1) {
                // is the process ongoing?
                if (process -> current_burst_time < process -> burst_time) {
                    current_quantum --;
                    process -> current_burst_time ++;
                    printf("p%d [%d/%d]", process -> label, process -> current_burst_time, process -> burst_time);
                }
                // has the process finished?
                if (process -> current_burst_time == process -> burst_time) {
                    int waiting = current_time - process -> arrival_time - process ->burst_time + 1;
                    totalWaitingTime += waiting;
                    int turnaround = current_time - process -> arrival_time + 1;
                    totalTurnaroundTime += turnaround;
                    printf("\nProcess p%d completed with Turn Around Time: %d, Waiting Time: %d", process -> label, turnaround, waiting);
                    current_quantum = -1;
                }
                // the process has not finished
                else {
                     // has the time quantum expired?
                    if (current_quantum == 0) {
                        enqueue(rr_queue, process);
                        current_quantum = -1;
                    }
                }
            }
            else {
                if (rr_queue -> size == 0) {
                    printf("None");
                }
                else {
                    process = dequeue(rr_queue);
                    current_quantum = quantumTime;
                    continueTriggered = 1;
                    continue;
                }
            }

            current_time++;
            printf("\n");
        }

        // final stats
        printf("Process scheduling completed with Avg Turn Around Time: %.2f, Avg Waiting Time: %.2f\n",
               totalTurnaroundTime * 1.0 / numOfProcesses,
               totalWaitingTime * 1.0 / numOfProcesses);
        printf("\n");

        // increment the test case number
        testCaseNumber ++;

        // clear memory
        destroy_queue(rr_queue);
        destroy_stack(arrivalStack);

        // destroy processes
        for (int i = 0; i < numOfProcesses; i++) {
            destroy_process(processArray[i]);
        }
    }

    // close file pointers
    fclose(fpointer);

    // return 0
    return 0;
}  // end of main
