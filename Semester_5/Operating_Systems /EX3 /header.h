#ifndef HEADER_H
#define HEADER_H

// Algorithm Declarations
void calculate_fcfs(int n, float at[], float bt[], float ct[], float tat[], float wt[]);
void calculate_sjf(int n, float at[], float bt[], float ct[], float tat[], float wt[]);
void calculate_srtf(int n, float at[], float bt[], float ct[], float tat[], float wt[]);
void calculate_rr(int n, float at[], float bt[], float ct[], float tat[], float wt[], float quantum);
void calculate_priority(int n, float at[], float bt[], int pr[], float ct[], float tat[], float wt[]);

// Utility to display results
void display_table(int n, float at[], float bt[], float ct[], float tat[], float wt[]);

#endif
