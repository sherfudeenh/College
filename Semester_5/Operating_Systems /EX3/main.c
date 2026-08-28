#include <stdio.h>
#include "header.h"

int main() {
    int n, choice, run_again = 1;
    float quantum;
    float at[10], bt[10], ct[10], tat[10], wt[10];
    int pr[10];

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    for(int i = 0; i < n; i++) {
        printf("Enter Arrival Time, Burst Time, and Priority (low int = high priority) for P%d: ", i + 1);
        scanf("%f %f %d", &at[i], &bt[i], &pr[i]);
    }

    while(run_again == 1) {
        printf("\nChoose CPU Scheduling Algorithm:\n");
        printf("1. FCFS\n2. SJF (Non-Preemptive)\n3. SRTF (Preemptive)\n4. Round Robin\n5. Priority (Non-Preemptive)\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: calculate_fcfs(n, at, bt, ct, tat, wt); break;
            case 2: calculate_sjf(n, at, bt, ct, tat, wt); break;
            case 3: calculate_srtf(n, at, bt, ct, tat, wt); break;
            case 4:
                printf("Enter Time Quantum: ");
                scanf("%f", &quantum);
                calculate_rr(n, at, bt, ct, tat, wt, quantum);
                break;
            case 5: calculate_priority(n, at, bt, pr, ct, tat, wt); break;
            default: printf("Invalid Choice!\n"); continue;
        }

        display_table(n, at, bt, ct, tat, wt);

        printf("\nExecute another algorithm? (1 for Yes, 0 to Exit): ");
        scanf("%d", &run_again);
    }
    return 0;
}