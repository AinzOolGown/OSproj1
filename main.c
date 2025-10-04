#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

// structure set up to take in the data off the text file
typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
    //these are for the output
    int waiting_time;
    int turnaround_time;
    int completion_time;
} Process;

int compareSJF(const void *a, const void *b) {
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;
    return p1->burst_time - p2->burst_time;
}


int main() {
    //checking for the file, assumes its in the same directory as program
    FILE *file = fopen("processes.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    //making sure data is properly formated in the text file
    Process *processes = NULL;
    int count = 0;
    int capacity = 5; // initial size
    processes = (Process*) malloc(capacity * sizeof(Process));
    if (processes == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        return 1;
    }

    //scanning the file for the data
    while (fscanf(file, "%d %d %d %d",
                  &processes[count].pid,
                  &processes[count].arrival_time,
                  &processes[count].burst_time,
                  &processes[count].priority) == 4)
    {
        count++;

        //adding capacity incase data is larger than expected
        if (count >= capacity) {
            capacity *= 2;
            processes = (Process*) realloc(processes, capacity * sizeof(Process));
            if (processes == NULL) {
                perror("Memory reallocation failed");
                fclose(file);
                return 1;
            }
        }
    }

    fclose(file);

    //printing information scanned to check success
    printf("Successfully read %d processes:\n", count);
    printf("PID\tArrival\tBurst\tPriority\n");
    printf("---------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%d\t%d\t%d\t%d\n",
               processes[i].pid,
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].priority);
    }

    free(processes);
    return 0;
}
