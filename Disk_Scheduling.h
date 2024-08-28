
#ifndef DISK_SCHEDULING_H
#define DISK_SCHEDULING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct {
    int *requests;
    int num_requests;
    int head_position;
} DiskState;

typedef struct{
    int total_head_movement;
    double avg_waiting_time;
    double throughput;
    double std_dev_respnse_time;
} PerformanceMetrics;

// Generating performance metrics
PerformanceMetrics fcfs(DiskState *state);
PerformanceMetrics sstf(DiskState *state);
PerformanceMetrics scan(DiskState *state);
PerformanceMetrics cscan(DiskState *state);
PerformanceMetrics look(DiskState *state);
PerformanceMetrics clook(DiskState *state);

// Functions for description of each scheduling algo
void fcfs_description();
void sstf_description();
void scan_description();
void cscan_description();
void look_description();
void clook_description();

// Function for calculating metrics

PerformanceMetrics calculate_metrics(DiskState* , int total_head_mov);
// Function for ploting graph
void plot_results(const char *algorithm_name, int *requests, int num_requests, int head_position, int total_head_movement);

// Function for random generation of inputs
void generate_random_requests(DiskState *state, int num_requests);

#endif