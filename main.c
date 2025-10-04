#include <stdio.h>
#include <stdlib.h>

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
    //for handling downtime
    int is_completed;
} Process;

//sjf sorting function
void sjf(Process *processes, int count) {
    int completed = 0, current_time = 0;
    double total_waiting = 0, total_turnaround = 0;

    // Arrays to store Gantt chart data
    int gantt_pids[100];
    int gantt_start[100];
    int gantt_end[100];
    int gantt_count = 0;

    // Initialize completion flags
    for (int i = 0; i < count; i++) {
        processes[i].is_completed = 0;
    }

    while (completed < count) {
        int idx = -1;
        int min_burst = 99999;

        // Find shortest job that has arrived
        for (int i = 0; i < count; i++) {
            if (processes[i].arrival_time <= current_time && !processes[i].is_completed) {
                if (processes[i].burst_time < min_burst) {
                    min_burst = processes[i].burst_time;
                    idx = i;
                }
            }
        }

        // If no process is ready, advance time
        if (idx == -1) {
            current_time++;
            continue;
        }

        // Record Gantt segment
        gantt_pids[gantt_count] = processes[idx].pid;
        gantt_start[gantt_count] = current_time;
        current_time += processes[idx].burst_time;
        gantt_end[gantt_count] = current_time;
        gantt_count++;

        // Update process data
        processes[idx].completion_time = current_time;
        processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival_time;
        processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
        processes[idx].is_completed = 1;

        total_waiting += processes[idx].waiting_time;
        total_turnaround += processes[idx].turnaround_time;
        completed++;
    }

    // ✅ Print properly formatted Gantt Chart
    printf("\nGantt Chart:\n");
    for (int i = 0; i < gantt_count; i++) {
        printf("| P%d ", gantt_pids[i]);
    }
    printf("|\n");

    for (int i = 0; i < gantt_count; i++) {
        printf("%-5d", gantt_start[i]);
    }
    printf("%-5d\n", gantt_end[gantt_count - 1]);

    printf("\n\nProcess Details:\n");
    printf("PID\tArrival\tBurst\tPriority\tWaiting\tTurnaround\n");
    for (int i = 0; i < count; i++) {
        printf("%d\t%d\t%d\t%d\t\t%d\t%d\n",
               processes[i].pid,
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].priority,
               processes[i].waiting_time,
               processes[i].turnaround_time);
    }

    printf("\nAverage Waiting Time: %.2f\n", total_waiting / count);
    printf("Average Turnaround Time: %.2f\n", total_turnaround / count);
}

//priority sorting function
void priority_scheduling(Process *processes, int count) {
    int completed = 0, current_time = 0;
    double total_waiting = 0, total_turnaround = 0;

    // Arrays to store Gantt chart info
    int gantt_pids[100];
    int gantt_start[100];
    int gantt_end[100];
    int gantt_count = 0;

    for (int i = 0; i < count; i++)
        processes[i].is_completed = 0;

    while (completed < count) {
        int idx = -1;
        int highest_priority = 99999;

        // Find highest priority process that has arrived
        for (int i = 0; i < count; i++) {
            if (processes[i].arrival_time <= current_time && !processes[i].is_completed) {
                if (processes[i].priority < highest_priority) {
                    highest_priority = processes[i].priority;
                    idx = i;
                }
            }
        }

        // If no process is ready, advance time
        if (idx == -1) {
            current_time++;
            continue;
        }

        // Record Gantt segment
        gantt_pids[gantt_count] = processes[idx].pid;
        gantt_start[gantt_count] = current_time;
        current_time += processes[idx].burst_time;
        gantt_end[gantt_count] = current_time;
        gantt_count++;

        // Update process data
        processes[idx].completion_time = current_time;
        processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival_time;
        processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
        processes[idx].is_completed = 1;

        total_waiting += processes[idx].waiting_time;
        total_turnaround += processes[idx].turnaround_time;
        completed++;
    }

    // Print Gantt Chart
    printf("\nGantt Chart:\n");
    for (int i = 0; i < gantt_count; i++)
        printf("| P%d ", gantt_pids[i]);
    printf("|\n");

    for (int i = 0; i < gantt_count; i++)
        printf("%-5d", gantt_start[i]);
    printf("%-5d\n", gantt_end[gantt_count - 1]);

    // Print process details
    printf("\nProcess Details:\n");
    printf("PID\tArrival\tBurst\tPriority\tWaiting\tTurnaround\n");
    for (int i = 0; i < count; i++) {
        printf("%d\t%d\t%d\t%d\t\t%d\t%d\n",
               processes[i].pid,
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].priority,
               processes[i].waiting_time,
               processes[i].turnaround_time);
    }

    printf("\nAverage Waiting Time: %.2f\n", total_waiting / count);
    printf("Average Turnaround Time: %.2f\n", total_turnaround / count);
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
                  &processes[count].priority) == 4){
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

    int choice;
    printf("Choose Scheduling Algorithm:\n");
    printf("1. Shortest Job First (SJF)\n");
    printf("2. Priority Scheduling\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    if (choice == 1)
        sjf(processes, count);
    else if (choice == 2)
        priority_scheduling(processes, count);
    else
        printf("Invalid choice.\n");


    free(processes);
    return 0;

    /*printing information scanned to check success of text file reading
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
    */
}
