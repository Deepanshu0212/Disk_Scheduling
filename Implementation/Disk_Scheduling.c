#include "../Header/Disk_Scheduling.h"
// randomly generate all the requests
void generate_random_requests(DiskState *state, int num_requests) {
    srand(time(NULL));
    state->num_requests = num_requests;
    state->requests = (int *)malloc(num_requests * sizeof(int));
    for (int i = 0; i < num_requests; i++) {
        state->requests[i] = rand() % (MAX_CYLINDER + 1);
    }

}

// calculate the metrics of each algorithm
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
    FILE *fp;
    char total_movement[256];

    // Read total movement from the file
    FILE *movement_file = fopen("Information/total_movement.txt", "r");
    if (movement_file == NULL) {
        printf("Error opening total_movement.txt!\n");
        return;
    }
    if (fgets(total_movement, sizeof(total_movement), movement_file) == NULL) {
        printf("Error reading total_movement.txt!\n");
        fclose(movement_file);
        return;
    }
    fclose(movement_file);
    
    // Remove newline character if present
    total_movement[strcspn(total_movement, "\n")] = '\0';

    // Open the Gnuplot script file
    fp = fopen("Information/disk_scheduling.plt", "w");
    if (fp == NULL) {
        printf("Error opening Gnuplot script file!\n");
        return;
    }

    // Write Gnuplot commands
    fprintf(fp, "set terminal png\n");
    fprintf(fp, "set output '%s'\n", output_file);
    fprintf(fp, "set title '%s Disk Scheduling'\n", algorithm);
    fprintf(fp, "set xlabel 'Request Sequence'\n");
    fprintf(fp, "set ylabel 'Cylinder'\n");
    fprintf(fp, "set yrange [0:199]\n");
    fprintf(fp, "set grid ytics\n");
    fprintf(fp, "set ytic 50\n");
    fprintf(fp, "set label sprintf('Total Movement: %s', '%s') at graph 0.02, graph 0.98 font ',10' tc rgb 'black'\n", total_movement, total_movement);
    fprintf(fp, "plot 'Information/disk_scheduling.dat' with linespoints title 'Total Movement' lt rgb 'green' pt 7 ps 1\n");

    fclose(fp);

    // Execute the Gnuplot script
    int result = system("gnuplot Information/disk_scheduling.plt");
    if (result == -1) {
        perror("Error executing gnuplot\n");
    } else if (result != 0) {
        fprintf(stderr, "gnuplot returned error: %d\n", result);
    } else {
        printf("Plot generated!\n");
    }
}




// Fcfs algo...
PerformanceMetrics fcfs(DiskState *state){

    FILE *fp = fopen("Information/disk_scheduling.dat", "w");
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
    
    fp = fopen("Information/total_movement.txt", "w");
    if (fp != NULL) {
        fprintf(fp, "%d", total_head_mov);
        fclose(fp);
    }

    return calculate_metrics(state , total_head_mov);
}


// sstf Algo implementation...
PerformanceMetrics sstf(DiskState *state){
    FILE *fp = fopen("Information/disk_scheduling.dat", "w");
    

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
    fp = fopen("Information/total_movement.txt", "w");
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
    FILE *file = fopen("Information/disk_scheduling.dat", "w");
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
    file = fopen("Information/total_movement.txt", "w");
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

    memcpy(sorted_requests, state->requests, state->num_requests * sizeof(int));

    // Sort the requests array
    qsort(sorted_requests, state->num_requests, sizeof(int), compare);

    // Open a file to write the positions
    FILE *file = fopen("Information/disk_scheduling.dat", "w");
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

    // Move the head to the maximum cylinder
    if (current_position < MAX_CYLINDER) {
        total_head_movement += MAX_CYLINDER - current_position;
        current_position = MAX_CYLINDER;
        fprintf(file, "%d %d\n", request_number, current_position);
        request_number++;
    }

    // Jump to the start of the disk
    total_head_movement += MAX_CYLINDER;
    current_position = 0;
    fprintf(file, "%d %d\n", request_number, current_position);
    request_number++;

    // Process remaining requests in the upward direction from the start of the disk
    for (int i = 0; i < head_index; i++) {
        total_head_movement += abs(sorted_requests[i] - current_position);
        current_position = sorted_requests[i];
        // Write the new head position to the file with request number
        fprintf(file, "%d %d\n", request_number, current_position);
        request_number++;
    }

    free(sorted_requests);
    fclose(file);

    // Write the total head movement to a separate file
    file = fopen("Information/total_movement.txt", "w");
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
    FILE *file = fopen("Information/disk_scheduling.dat", "w");
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
    file = fopen("Information/total_movement.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d", total_head_movement);
        fclose(file);
    }
    return calculate_metrics(state, total_head_movement);
}


// implemetation of clook algorithm:-

PerformanceMetrics clook(DiskState *state) {
    int total_head_movement = 0;
    int current_position = state->head_position;
    int *sorted_requests = malloc(state->num_requests * sizeof(int));

    memcpy(sorted_requests, state->requests, state->num_requests * sizeof(int));

    // Sort the requests array
    qsort(sorted_requests, state->num_requests, sizeof(int), compare);

    // Open a file to write the positions
    FILE *file = fopen("Information/disk_scheduling.dat", "w");
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

    // Jump to the lowest request without servicing the cylinders in between

    if (head_index > 0) {
        total_head_movement += abs(sorted_requests[0] - current_position);
        current_position = sorted_requests[0];
        fprintf(file, "%d %d\n", request_number, current_position);
        request_number++;
    }

    // Process remaining requests in the upward direction (from the start)
    for (int i = 1; i < head_index; i++) {
        total_head_movement += abs(sorted_requests[i] - current_position);
        current_position = sorted_requests[i];
        // Write the new head position to the file with request number
        fprintf(file, "%d %d\n", request_number, current_position);
        request_number++;
    }

    free(sorted_requests);
    fclose(file);

    // Write the total head movement to a separate file
    file = fopen("Information/total_movement.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d", total_head_movement);
        fclose(file);
    }
    return calculate_metrics(state, total_head_movement);
}

 

 // Description of each Algorithm:-
void fcfs_description() {
    printf("FCFS: First-Come, First-Served - Processes requests in the order they arrive, regardless of their location on the disk.\n");
}

void sstf_description() {
    printf("SSTF: Shortest Seek Time First - Selects the request with the shortest seek time from the current head position, minimizing the total head movement.\n");
}

void scan_description() {
    printf("SCAN: Elevator Algorithm - Moves the disk arm across the disk, servicing requests in one direction until it reaches the end, then reverses direction and services requests on the way back.\n");
}

void cscan_description() {
    printf("C-SCAN: Circular SCAN - Moves the disk arm in one direction, servicing requests until it reaches the end, then jumps back to the beginning without servicing requests on the return trip.\n");
}

void look_description() {
    printf("LOOK: Similar to SCAN, but only moves as far as the last request in each direction, then reverses direction, avoiding unnecessary movement to the disk's ends.\n");
}

void clook_description() {
    printf("C-LOOK: Circular LOOK - Similar to C-SCAN, but only moves as far as the last request in one direction, then jumps back to the beginning, avoiding unnecessary movement.\n");
}

