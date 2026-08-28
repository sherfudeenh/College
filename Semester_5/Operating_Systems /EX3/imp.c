#include <stdio.h>
#include "header.h"

// 1. FCFS Logic
void calculate_fcfs(int n, float at[], float bt[], float ct[], float tat[], float wt[]) {
    int completed[10] = {0};
    float current_time = 0;

    for (int count = 0; count < n; count++) {
        int next_idx = -1;
        float min_at = 99999.0;

        for (int i = 0; i < n; i++) {
            if (!completed[i] && at[i] < min_at) {
                min_at = at[i];
                next_idx = i;
            }
        }

        if (next_idx == -1) break;
        if (current_time < at[next_idx]) current_time = at[next_idx];

        current_time += bt[next_idx];
        ct[next_idx] = current_time;
        tat[next_idx] = ct[next_idx] - at[next_idx];
        wt[next_idx] = tat[next_idx] - bt[next_idx];
        completed[next_idx] = 1;
    }
}

// 2. SJF Non-Preemptive Logic
void calculate_sjf(int n, float at[], float bt[], float ct[], float tat[], float wt[]) {
    int completed[10] = {0};
    float current_time = 0;
    int count = 0;

    while (count < n) {
        int index = -1;
        float min_bt = 99999.0;
        float min_at = 99999.0;

        for (int i = 0; i < n; i++) {
            if (at[i] <= current_time && completed[i] == 0) {
                if (bt[i] < min_bt) {
                    min_bt = bt[i];
                    index = i;
                }
            }
            if (completed[i] == 0 && at[i] < min_at) min_at = at[i];
        }

        if (index == -1) current_time = min_at;
        else {
            current_time += bt[index];
            ct[index] = current_time;
            tat[index] = ct[index] - at[index];
            wt[index] = tat[index] - bt[index];
            completed[index] = 1;
            count++;
        }
    }
}

// 3. SRTF Preemptive Logic
void calculate_srtf(int n, float at[], float bt[], float ct[], float tat[], float wt[]) {
    float rt[10];
    int completed = 0;
    float current_time = 0;
    for (int i = 0; i < n; i++) rt[i] = bt[i];

    while (completed < n) {
        int min_idx = -1;
        float min_rt = 99999.0;
        float min_at = 99999.0;

        for (int i = 0; i < n; i++) {
            if (at[i] <= current_time && rt[i] > 0) {
                if (rt[i] < min_rt) {
                    min_rt = rt[i];
                    min_idx = i;
                }
            }
            if (rt[i] > 0 && at[i] < min_at) min_at = at[i];
        }

        if (min_idx == -1) {
            current_time = min_at;
            continue;
        }

        rt[min_idx] -= 1.0;
        current_time += 1.0;

        if (rt[min_idx] <= 0.0001) {
            ct[min_idx] = current_time;
            tat[min_idx] = ct[min_idx] - at[min_idx];
            wt[min_idx] = tat[min_idx] - bt[min_idx];
            completed++;
        }
    }
}

// 4. Round Robin Logic
void calculate_rr(int n, float at[], float bt[], float ct[], float tat[], float wt[], float quantum) {
    float rt[10];
    int queue[2000], front = 0, rear = 0, in_queue[10] = {0}, completed = 0;
    float current_time = 0;

    for (int i = 0; i < n; i++) rt[i] = bt[i];

    float min_at = 99999.0;
    for (int i = 0; i < n; i++) if (at[i] < min_at) min_at = at[i];
    current_time = min_at;

    for (int i = 0; i < n; i++) {
        if (at[i] <= current_time) {
            queue[rear++] = i;
            in_queue[i] = 1;
        }
    }

    while (completed < n) {
        if (front == rear) {
            float next_at = 99999.0;
            for (int i = 0; i < n; i++) if (rt[i] > 0 && at[i] < next_at) next_at = at[i];
            current_time = next_at;
            for (int i = 0; i < n; i++) {
                if (at[i] <= current_time && rt[i] > 0 && !in_queue[i]) {
                    queue[rear++] = i;
                    in_queue[i] = 1;
                }
            }
            continue;
        }

        int idx = queue[front++];
        float allocation = (rt[idx] > quantum) ? quantum : rt[idx];
        float start_time = current_time;
        rt[idx] -= allocation;
        current_time += allocation;

        for (int i = 0; i < n; i++) {
            if (at[i] > start_time && at[i] <= current_time && rt[i] > 0 && !in_queue[i]) {
                queue[rear++] = i;
                in_queue[i] = 1;
            }
        }

        if (rt[idx] > 0) queue[rear++] = idx;
        else {
            ct[idx] = current_time;
            tat[idx] = ct[idx] - at[idx];
            wt[idx] = tat[idx] - bt[idx];
            completed++;
            in_queue[idx] = 0;
        }
    }
}

// 5. Priority Scheduling (Non-Preemptive)
void calculate_priority(int n, float at[], float bt[], int pr[], float ct[], float tat[], float wt[]) {
    int completed[10] = {0};
    float current_time = 0;
    int count = 0;

    while (count < n) {
        int index = -1;
        int min_priority = 99999;
        float min_at = 99999.0;

        for (int i = 0; i < n; i++) {
            if (at[i] <= current_time && completed[i] == 0) {
                // Smallest priority number = Highest Priority
                if (pr[i] < min_priority) {
                    min_priority = pr[i];
                    index = i;
                } else if (pr[i] == min_priority) { // Tie-breaker: FCFS
                    if (at[i] < at[index]) index = i;
                }
            }
            if (completed[i] == 0 && at[i] < min_at) min_at = at[i];
        }

        if (index == -1) current_time = min_at;
        else {
            current_time += bt[index];
            ct[index] = current_time;
            tat[index] = ct[index] - at[index];
            wt[index] = tat[index] - bt[index];
            completed[index] = 1;
            count++;
        }
    }
}

void display_table(int n, float at[], float bt[], float ct[], float tat[], float wt[]) {
    float avgWT = 0, avgTAT = 0;
    printf("\n---------------------------------------------------------");
    printf("\nProc\tAT\tBT\tCT\tTAT\tWT");
    printf("\n---------------------------------------------------------");
    for(int i = 0; i < n; i++) {
        printf("\nP%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f", i+1, at[i], bt[i], ct[i], tat[i], wt[i]);
        avgTAT += tat[i]; avgWT += wt[i];
    }
    printf("\n---------------------------------------------------------");
    printf("\nAverage Turnaround Time = %.2f", avgTAT/n);
    printf("\nAverage Waiting Time = %.2f\n", avgWT/n);
}