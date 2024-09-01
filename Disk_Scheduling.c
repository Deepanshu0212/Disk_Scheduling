#include "Disk_Scheduling.h"

PerformanceMetrics calculate_metrics(DiskState* state, int total_head_movement) {
    PerformanceMetrics metrics;
    metrics.total_head_movement = total_head_movement;
    
    int total_waiting_time = 0;
    int* waiting_times = malloc(state->num_requests * sizeof(int));
    int current_time = 0;
    int current_position = state->head_position;
    
    for (int i = 0; i < state->num_requests; i++) {
        int seek_time = abs(state->requests[i] - current_position);
        current_time += seek_time;
        waiting_times[i] = current_time;
        total_waiting_time += waiting_times[i];
        current_position = state->requests[i];
    }
    
    metrics.avg_waiting_time = (double)total_waiting_time / state->num_requests;
    metrics.throughput = (double)state->num_requests / current_time;
    
    double mean_response_time = metrics.avg_waiting_time;
    double sum_squared_diff = 0;
    
    for (int i = 0; i < state->num_requests; i++) {
        double diff = waiting_times[i] - mean_response_time;
        sum_squared_diff += diff * diff;
    }
    
    metrics.std_dev_response_time = sqrt(sum_squared_diff / state->num_requests);
    
    free(waiting_times);
    return metrics;
}
void plot_results(char* algorithm, char* output_file) {
    FILE *fp = fopen("disk_scheduling.plt", "w");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(fp, "set terminal png\n");
    fprintf(fp, "set output '%s'\n", output_file);
    fprintf(fp, "set title '%s Disk Scheduling'\n", algorithm);
    fprintf(fp, "set xlabel 'Request Sequence'\n");
    fprintf(fp, "set ylabel 'Cylinder'\n");
    fprintf(fp, "set yrange [0:199]\n");
    fprintf(fp, "set grid ytics\n");
    fprintf(fp, "set ytic 50\n");
    fprintf(fp, "total_movement = system('type total_movement.txt')\n");
    fprintf(fp, "set label sprintf('Total Movement: %%s', total_movement) at graph 0.02, graph 0.98 font ',10' tc rgb 'black'\n");
    fprintf(fp, "plot 'disk_scheduling.dat' with linespoints title 'Head Movement' lt rgb 'green' pt 7 ps 1\n");

    fclose(fp);

}



// Fcfs algo...
PerformanceMetrics fcfs(DiskState *state){

    FILE *fp = fopen("disk_scheduling.dat", "w");
    // if (fp == NULL) {
    //     printf("Error opening file!\n");
    //     return ;
    // }

    int total_head_mov =0;
    int curr_pos = state->head_position;
    fprintf(fp, "%d %d\n", 0, curr_pos);

    for(int i=0;i<state->num_requests;i++){
        
        total_head_mov += abs(state->requests[i] - curr_pos);
        curr_pos = state->requests[i];
        fprintf(fp, "%d %d\n", i+1, curr_pos);
    }
     

    fclose(fp);
    
    fp = fopen("total_movement.txt", "w");
    if (fp != NULL) {
        fprintf(fp, "%d", total_head_mov);
        fclose(fp);
    }

    return calculate_metrics(state , total_head_mov);
}


// sstf Algo implementation...
PerformanceMetrics sstf(DiskState *state){
    FILE *fp = fopen("disk_scheduling.dat", "w");
    

    int visited[MAX_REQUESTS] = {0};
    int current = state->head_position;
    int total_head_mov = 0;
    fprintf(fp, "%d %d\n", 0, current);

    for (int i = 0; i < state->num_requests; i++) {
        int min_distance = INT_MAX;
        int next_request = -1;

        for (int j = 0; j < state->num_requests; j++) {
            if (!visited[j]) {
                int distance = abs(state->requests[j] - current);
                if (distance < min_distance) {
                    min_distance = distance;
                    next_request = j;
                }
            }
        }

        visited[next_request] = 1;
        current = state->requests[next_request];
        total_head_mov += min_distance;
        fprintf(fp, "%d %d\n", i + 1, current);
    }

    fclose(fp);

    // Save total movement to a separate file
    fp = fopen("total_movement.txt", "w");
    if (fp != NULL) {
        fprintf(fp, "%d", total_head_mov);
        fclose(fp);
    }

    
    return calculate_metrics(state , total_head_mov);

}


// scan algo impl

int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

PerformanceMetrics scan(DiskState *state) {
    int total_head_movement = 0;
    int current_position = state->head_position;
    int *sorted_requests = malloc(state->num_requests * sizeof(int));

    memcpy(sorted_requests, state->requests, state->num_requests * sizeof(int));

    // Sort the requests array
    qsort(sorted_requests, state->num_requests, sizeof(int), compare);

    // Open a file to write the positions
    FILE *file = fopen("disk_scheduling.dat", "w");
    if (file == NULL) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    // Write the initial head position to the file
    fprintf(file, "0 %d\n", current_position);

    // Find the first request greater than or equal to the current head position
    int head_index = 0;
    while (head_index < state->num_requests && sorted_requests[head_index] < current_position) {
        head_index++;
    }

    int request_number = 1;

    // Process requests in the upward direction (towards higher cylinder numbers)
    for (int i = head_index; i < state->num_requests; i++) {
        total_head_movement += abs(sorted_requests[i] - current_position);
        current_position = sorted_requests[i];
        // Write the new head position to the file with request number
        fprintf(file, "%d %d\n", request_number, current_position);
        request_number++;
    }

    // Move the head to the maximum cylinder if it hasn't been done yet
    if (current_position < MAX_CYLINDER) {
        total_head_movement += (MAX_CYLINDER) - current_position;
        current_position = MAX_CYLINDER;
        fprintf(file, "%d %d\n", request_number, current_position);
        request_number++;
    }

    // Change direction and process requests in the downward direction
    for (int i = head_index - 1; i >= 0; i--) {
        total_head_movement += abs(sorted_requests[i] - current_position);
        current_position = sorted_requests[i];
        // Write the new head position to the file with request number
        fprintf(file, "%d %d\n", request_number, current_position);
        request_number++;
    }

    free(sorted_requests);
    fclose(file);

    // Write the total head movement to a separate file
    file = fopen("total_movement.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d", total_head_movement);
        fclose(file);
    }
    return calculate_metrics(state, total_head_movement);
}

// Implementation of c-scan:

PerformanceMetrics cscan(DiskState *state) {
    int total_head_movement = 0;
    int current_position = state->head_position;
    int *sorted_requests = malloc(state->num_requests * sizeof(int));
    int visited[MAX_CYLINDER] = {0};  // Array to track visited positions

    memcpy(sorted_requests, state->requests, state->num_requests * sizeof(int));

    // Sort the requests array
    qsort(sorted_requests, state->num_requests, sizeof(int), compare);

    // Open a file to write the positions
    FILE *file = fopen("disk_scheduling.dat", "w");
    if (file == NULL) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    // Write the initial head position to the file
    fprintf(file, "0 %d\n", current_position);
    visited[current_position] = 1;

    // Find the first request greater than or equal to the current head position
    int head_index = 0;
    while (head_index < state->num_requests && sorted_requests[head_index] < current_position) {
        head_index++;
    }

    int request_number = 1;

    // Process requests in the upward direction (towards higher cylinder numbers)
    for (int i = head_index; i < state->num_requests; i++) {
        if (!visited[sorted_requests[i]]) {
            total_head_movement += abs(sorted_requests[i] - current_position);
            current_position = sorted_requests[i];
            // Write the new head position to the file with request number
            fprintf(file, "%d %d\n", request_number, current_position);
            visited[current_position] = 1;
            request_number++;
        }
    }

    // Move the head to the maximum cylinder, if needed
    if (current_position < MAX_CYLINDER - 1) {
        total_head_movement += (MAX_CYLINDER - 1) - current_position;
        current_position = MAX_CYLINDER - 1;
        fprintf(file, "%d %d\n", request_number, current_position);
        request_number++;
    }

    // Move the head to the start of the disk (cylinder 0)
    total_head_movement += current_position; // current_position is now MAX_CYLINDER - 1
    current_position = 0;
    fprintf(file, "%d %d\n", request_number, current_position);
    request_number++;

    // Continue processing requests from the beginning up to the original head position
    for (int i = 0; i < head_index; i++) {
        if (!visited[sorted_requests[i]]) {
            total_head_movement += abs(sorted_requests[i] - current_position);
            current_position = sorted_requests[i];
            // Write the new head position to the file with request number
            fprintf(file, "%d %d\n", request_number, current_position);
            visited[current_position] = 1;
            request_number++;
        }
    }

    free(sorted_requests);
    fclose(file);

    // Write the total head movement to a separate file
    file = fopen("total_movement.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d", total_head_movement);
        fclose(file);
    }
    return calculate_metrics(state, total_head_movement);
}


PerformanceMetrics look(DiskState *state) {
    int total_head_movement = 0;
    int current_position = state->head_position;
    int *sorted_requests = malloc(state->num_requests * sizeof(int));

    memcpy(sorted_requests, state->requests, state->num_requests * sizeof(int));

    // Sort the requests array
    qsort(sorted_requests, state->num_requests, sizeof(int), compare);

    // Open a file to write the positions
    FILE *file = fopen("disk_scheduling.dat", "w");
    if (file == NULL) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    // Write the initial head position to the file
    fprintf(file, "0 %d\n", current_position);

    // Find the first request greater than or equal to the current head position
    int head_index = 0;
    while (head_index < state->num_requests && sorted_requests[head_index] < current_position) {
        head_index++;
    }

    int request_number = 1;

    // Process requests in the upward direction (towards higher cylinder numbers)
    for (int i = head_index; i < state->num_requests; i++) {
        total_head_movement += abs(sorted_requests[i] - current_position);
        current_position = sorted_requests[i];
        // Write the new head position to the file with request number
        fprintf(file, "%d %d\n", request_number, current_position);
        request_number++;
    }

    // Change direction and process requests in the downward direction
    for (int i = head_index - 1; i >= 0; i--) {
        total_head_movement += abs(sorted_requests[i] - current_position);
        current_position = sorted_requests[i];
        // Write the new head position to the file with request number
        fprintf(file, "%d %d\n", request_number, current_position);
        request_number++;
    }

    free(sorted_requests);
    fclose(file);

    // Write the total head movement to a separate file
    file = fopen("total_movement.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d", total_head_movement);
        fclose(file);
    }
    return calculate_metrics(state, total_head_movement);
}