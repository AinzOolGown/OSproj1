#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
} Process;

int main() {
    FILE *file = fopen("processes.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    Process *processes = NULL;
    int count = 0;
    int capacity = 5; // initial size
    processes = (Process*) malloc(capacity * sizeof(Process));
    if (processes == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        return 1;
    }

    while (fscanf(file, "%d %d %d %d",
                  &processes[count].pid,
                  &processes[count].arrival_time,
                  &processes[count].burst_time,
                  &processes[count].priority) == 4)
    {
        count++;

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
